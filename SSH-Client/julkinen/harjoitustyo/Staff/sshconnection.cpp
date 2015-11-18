#include "sshconnection.h"
#include <QCoreApplication>
#include <QTimer>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>

using namespace Staff;

SshConnection::SshConnection(QString user, QString host, int port,
                             int passwordTries, PasswordProvider *passprov):
    QObject(),
    canceled(false), user(user), host(host), port(port), provider(passprov), session(0),
    passwordTries(passwordTries)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.setInterval(7500);
    timer.setSingleShot(true);
}

SshConnection::~SshConnection()
{
    if (session)
    {
        libssh2_session_disconnect(session, "Quit");
        libssh2_session_free(session);
        close(lowSock);
    }
}

LIBSSH2_SFTP * SshConnection::connection()
{
    return sftp;
}

void SshConnection::timeout()
{
    canceled = true;
    emit failed("Failed to connect to host: Connection timed out");
}

void SshConnection::wait()
{
    QCoreApplication::processEvents();
    if (!canceled)
    {
        fd_set fds;
        struct timeval time;
        time.tv_sec = 0;
        time.tv_usec = 150;
        FD_ZERO(&fds);
        FD_SET(lowSock, &fds);
        select(1, &fds, &fds, nullptr, &time);
    }
}

void SshConnection::connectToServer()
{
    struct addrinfo hints;
    struct addrinfo *addrRes, *addr;
    bool authenticated;
    char * userauthlist;
    QByteArray name;

    // Start the timer
    timer.start();
    // Process events
    QCoreApplication::processEvents();

    // If we were already canceled return
    if (canceled)
        return;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICSERV;

    // Find addressess
    char portString[10];
    snprintf(portString, 10, "%i", port);
    int res = getaddrinfo(host.toUtf8().data(), portString, &hints, &addrRes );
    if (res < 0)
    {
        emit failed(gai_strerror(res));
        return;
    }

    // Try to connect to them
    bool sockConnected = false;
    for (addr=addrRes; addr != nullptr ; addr = addr->ai_next)
    {
        QCoreApplication::processEvents();
        if (canceled)
        {
            freeaddrinfo(addrRes);
            return;
        }
        // Make me a socket
        lowSock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (lowSock < 0)
            continue;
        // Set it to non-blocking mode
        fcntl(lowSock, F_SETFL, fcntl(lowSock, F_GETFL, 0)|O_NONBLOCK);

        // Try to connect it to the host
        res = ::connect(lowSock, addr->ai_addr, addr->ai_addrlen);
        if ((res != 0) && (errno != EINPROGRESS))
        {
            close(lowSock);
            continue;
        }

        // loop until we are connected
        while (res != 0)
        {
            wait();
            if (canceled)
            {
                freeaddrinfo(addrRes);
                goto closeSocket;
            }
            socklen_t size = sizeof(res);
            if (getsockopt(lowSock, SOL_SOCKET, SO_ERROR, &res, &size) != 0)
            {
                failed("Get socket option failed");
                freeaddrinfo(addrRes);
                goto closeSocket;
            }
            if ((res < 0) && (res != EINPROGRESS))
            {
                close(lowSock);
                continue;
            }
        }
        // We got it
        sockConnected = true;
        break;
    }

    freeaddrinfo(addrRes);
    if (!sockConnected)
    {
        emit failed(strerror(errno));
        return;
    }

    // Set it to blocking mode
    fcntl(lowSock, F_SETFL, fcntl(lowSock, F_GETFL, 0)&~O_NONBLOCK);
    session = libssh2_session_init();


    libssh2_session_set_timeout(session, 10000);

    QCoreApplication::processEvents();
    if (canceled)
        goto releaseSession;
    if (libssh2_session_handshake(session, lowSock))
    {
        emit failed("Failed to initialize ssh connection");
        goto releaseSession;
    }

    QCoreApplication::processEvents();
    if (canceled)
        goto releaseSession;
    name = user.toUtf8();
    userauthlist = libssh2_userauth_list(session, name.constData(), name.size());
    if (userauthlist == 0)
    {
        emit failed("Can't fetch authentication modes");
        goto releaseSession;
    }

    QCoreApplication::processEvents();
    if (canceled)
        goto releaseSession;
    
    authenticated = false;
    if (strstr(userauthlist, "publickey"))
    {
        // Try ssh-agent authentication
        LIBSSH2_AGENT * agent = libssh2_agent_init(session);
        if (agent)
        {
            struct libssh2_agent_publickey * key = 0;
            if (libssh2_agent_connect(agent))
                goto agentFree;
            if (libssh2_agent_list_identities(agent))
                goto agentDisconnect;
            while (!libssh2_agent_get_identity(agent, &key, key))
            {
                if (!libssh2_agent_userauth(agent, name.constData(), key))
                {
                    authenticated = true;
                    break;
                }
            }
            agentDisconnect:
                libssh2_agent_disconnect(agent);
            agentFree:
                libssh2_agent_free(agent);
        }
    }
    if (!authenticated && strstr(userauthlist, "password"))
    {
		timer.stop();
        while (passwordTries)
        {
            char pass[1000];
            if (!provider->getPassword(pass, 1000))
                break;
            int res = libssh2_userauth_password(session, name.constData(), pass);
            memset(pass, 0, 1000);
            if (!res)
            {
                authenticated = true;
                break;
            }

        }
    }

    if (!authenticated)
    {
        emit failed("Authentication failed");
        goto releaseSession;
    }

    QCoreApplication::processEvents();
    if (canceled)
        goto releaseSession;

    sftp = libssh2_sftp_init(session);
    if (!sftp)
    {
        emit failed("Couldn't initialize sftp-session");
        goto releaseSession;
    }
	timer.stop();

    emit connected();
    return;

releaseSession:
    libssh2_session_free(session);
    session = 0;

closeSocket:
    close(lowSock);
    freeaddrinfo(addrRes);
	timer.stop();
}
