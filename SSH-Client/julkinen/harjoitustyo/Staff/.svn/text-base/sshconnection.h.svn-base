#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <QTimer>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <memory>

namespace Staff
{
    class PasswordProvider
    {
    public:
        virtual bool getPassword(char * pass ,int maxlength) = 0;
    };

    class SshConnection : public QObject
    {
        Q_OBJECT

    public:
        explicit SshConnection(QString user, QString host, int port, int passwordTries,
                      PasswordProvider * passprov);
        ~SshConnection();
        void connectToServer();
        LIBSSH2_SFTP * connection();
    signals:
        void connected();
        void failed(QString error);
    private slots:
        void timeout();
    private:
        void wait();
        bool canceled;
        QString user;
        QString host;
        int port;
        int lowSock;
        PasswordProvider * provider;
        LIBSSH2_SESSION * session;
        LIBSSH2_SFTP * sftp;
        int passwordTries;
        QTimer timer;
    };
}

#endif // SSHCONNECTION_H
