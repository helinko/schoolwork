#ifndef SFTPFILEENTRY_H
#define SFTPFILEENTRY_H

#include "DirectoryEntryInterface.h"
#include "DateTime.h"
#include "libssh2_sftp.h"
#include "sftpconnection.h"

namespace Staff
{
    class SftpFileEntry
    {
    public:
        SftpFileEntry(SftpConnection * session, char * name);
        QString name() const;
        QString fullPath() const;
        int permissions() const;
        void changePermissions(int newPermissions);
        void rename(QString newName);
        void remove();
        virtual ~SftpFileEntry() {free(namestr);}
        void stat(LIBSSH2_SFTP_ATTRIBUTES *buf) const;
        FSInterface::DateTime time() const;
    protected:
        SftpConnection * const session;
        char * namestr;
        void handleError(int errorcode,const QString & operation, bool pointer=false) const;
    };
}
#endif // SFTPFILEENTRY_H
