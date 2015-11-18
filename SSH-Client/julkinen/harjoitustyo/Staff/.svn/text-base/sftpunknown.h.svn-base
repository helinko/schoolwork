#ifndef SFTPUNKNOWN_H
#define SFTPUNKNOWN_H

#include <memory>
#include "DirectoryEntryInterface.h"
#include "sftpfileentry.h"

namespace Staff
{
    class SftpUnknown : public SftpFileEntry, public FSInterface::DirectoryEntryInterface
    {
    public:
        SftpUnknown(SftpConnection * session, char * name) : SftpFileEntry(session,name) {}
        virtual QString name() const { return SftpFileEntry::name(); }
        virtual QString fullPath() const { return SftpFileEntry::fullPath(); }
        virtual int permissions() const { return SftpFileEntry::permissions(); }
        virtual void changePermissions(int newPermissions)
            { SftpFileEntry::changePermissions(newPermissions); }
        virtual void rename(QString newName) { return SftpFileEntry::rename(newName); }
        virtual FSInterface::DateTime lastModified() const { return SftpFileEntry::time(); }
        virtual ~SftpUnknown() = default;
        virtual long size() const { throw FSInterface::OperationNotSupported("Unknown direntry size"); }
        virtual void remove() { SftpFileEntry::remove(); }
        virtual FSInterface::ConnectionInterface * connection() const { return session; }
    };
}
#endif // SFTPUNKNOWN_H
