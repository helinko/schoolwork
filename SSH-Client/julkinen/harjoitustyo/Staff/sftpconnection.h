#ifndef SFTPCONNECTION_H
#define SFTPCONNECTION_H

#include <ConnectionInterface.h>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include "DirectoryEntryInterface.h"
namespace Staff
{
    class SftpConnection : public FSInterface::ConnectionInterface
    {
    public:
        SftpConnection(QString server, QString dir, LIBSSH2_SFTP * session) :
            sftp(session), serverStr(server), pathStr(dir) {}
        virtual QString server() const { return serverStr; }
        virtual QString path() const { return pathStr; }
        virtual std::shared_ptr<FSInterface::DirectoryInterface> directory();
        std::unique_ptr<FSInterface::DirectoryEntryInterface> open(char * name);
        LIBSSH2_SFTP * const sftp;
        const QString serverStr;
        const QString pathStr;
    };
}
#endif // SFTPCONNECTION_H
