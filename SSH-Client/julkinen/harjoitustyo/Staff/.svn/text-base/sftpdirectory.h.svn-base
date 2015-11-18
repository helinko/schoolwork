#ifndef SFTPDIRECTORY_H
#define SFTPDIRECTORY_H

#include <memory>
#include "DirectoryInterface.h"
#include "sftpfileentry.h"
namespace Staff
{
    class SftpDirectory : public SftpFileEntry, public FSInterface::DirectoryInterface
    {
    public:
        SftpDirectory(SftpConnection * session, char * name);
        virtual QString name() const { return SftpFileEntry::name(); }
        virtual QString fullPath() const { return SftpFileEntry::fullPath(); }
        virtual int permissions() const {return SftpFileEntry::permissions(); }
        virtual void changePermissions(int newPermissions)
            {return SftpFileEntry::changePermissions(newPermissions); }
        virtual void rename(QString newName) { return SftpFileEntry::rename(newName); }
        virtual FSInterface::DateTime lastModified() const { return SftpFileEntry::time(); }
        virtual ~SftpDirectory() {}
        virtual void remove();
        virtual QList<QString> contents() const;
        virtual std::shared_ptr<DirectoryEntryInterface> entry(const QString &name) const;
        virtual bool hasChild(const QString &name) const;
        virtual std::shared_ptr<DirectoryInterface> mkdir(const QString &name);
        virtual std::shared_ptr<FSInterface::FileInterface> mkfile(const QString &name);
        virtual std::shared_ptr<FSInterface::SymbolicLinkInterface> mksymlink(const QString &name, const QString &target);
        virtual long size() const { throw FSInterface::OperationNotSupported("Directory size"); }
        virtual FSInterface::ConnectionInterface * connection() const { return session; }
    private:
        char * childName(const QString & name) const;
    };
}
#endif // SFTPDIRECTORY_H
