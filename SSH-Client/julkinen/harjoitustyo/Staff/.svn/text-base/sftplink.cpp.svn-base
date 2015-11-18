#include "sftplink.h"
#include <QByteArray>

using namespace Staff;
using namespace FSInterface;
SftpLink::SftpLink(SftpConnection * session, char * name)
    : SftpFileEntry(session, name)
{
}

QString SftpLink::content() const
{
    QByteArray data;
    data.resize(512);
    int res;
    while (0 > (res = libssh2_sftp_readlink(session->sftp, namestr, data.data(), data.size())))
    {
        if (res == LIBSSH2_ERROR_EAGAIN)
        {
            // Try again
        } else if (res == LIBSSH2_ERROR_BUFFER_TOO_SMALL)
        {
            data.resize(data.size()<<1);
        } else
        {
            handleError(res, "readlink");
        }
    }
    return QString::fromUtf8(data.data());
}

std::shared_ptr<DirectoryEntryInterface> SftpLink::target() const
{
    QByteArray data;
    data.resize(512);
    int res;
    while (0 > (res = libssh2_sftp_realpath(session->sftp, namestr, data.data(), data.size())))
    {
        if (res == LIBSSH2_ERROR_EAGAIN)
        {
            // Try again
        } else if (res == LIBSSH2_ERROR_BUFFER_TOO_SMALL)
        {
            data.resize(data.size()<<1);
        } else
        {
            handleError(res, "readlink");
        }
    }
    return session->open(data.data());
}
