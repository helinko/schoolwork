#include "SymbolicLinkInterface.h"
#include "sftpfileentry.h"

namespace Staff
{
    class SftpLink : public SftpFileEntry, public FSInterface::SymbolicLinkInterface
    {
    public:
        SftpLink(SftpConnection * session, char * name);
        virtual ~SftpLink() = default;
        virtual QString content() const;
        virtual std::shared_ptr<DirectoryEntryInterface> target() const;
        virtual QString name() const { return SftpFileEntry::name(); }
        virtual QString fullPath() const { return SftpFileEntry::fullPath(); }
        virtual long size() const { throw FSInterface::OperationNotSupported("Link size"); }
        virtual int permissions() const { return 0777; }
        virtual void changePermissions(int)
            { throw FSInterface::OperationNotSupported("Link permission change"); }
        virtual void remove() { SftpFileEntry::remove(); }
        virtual void rename(QString name)  { SftpFileEntry::rename(name); }
        virtual FSInterface::DateTime lastModified() const { return SftpFileEntry::time(); }
        virtual FSInterface::ConnectionInterface * connection() const { return session; }
    };
}
