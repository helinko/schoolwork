#ifndef SFTPFILE_H
#define SFTPFILE_H

#include "FileInterface.h"
#include "sftpfileentry.h"

namespace Staff
{
    class SftpFile : public SftpFileEntry, public FSInterface::FileInterface
    {
    public:
        SftpFile(SftpConnection * session, char * name);
        virtual QString name() const { return SftpFileEntry::name(); }
        virtual QString fullPath() const { return SftpFileEntry::fullPath(); }
        virtual int permissions() const {return SftpFileEntry::permissions(); }
        virtual void changePermissions(int newPermissions)
            {return SftpFileEntry::changePermissions(newPermissions); }
        virtual void rename(QString newName) { return SftpFileEntry::rename(newName); }
        virtual FSInterface::DateTime lastModified() const { return SftpFileEntry::time(); }
        virtual ~SftpFile();
        virtual long size() const;
        virtual void remove() { SftpFileEntry::remove(); }
        virtual void open(FSInterface::FileInterface::Mode mode);
        virtual int read(QByteArray &array);
        virtual int write(const QByteArray &array);
        virtual void close();
        virtual FSInterface::ConnectionInterface * connection() const { return session; }
    private:
        LIBSSH2_SFTP_HANDLE * openHandle;
    };
}
#endif // SFTPFILE_H
