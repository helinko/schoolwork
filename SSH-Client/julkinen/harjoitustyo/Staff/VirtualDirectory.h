#ifndef VIRTUALDIRECTORY_H
#define VIRTUALDIRECTORY_H

#include <QMap>
#include "DirectoryInterface.h"
#include "VirtualFile.h"

namespace FSInterface
{
    /**
     * @brief Kokonaan muistissa elävä kansio.
     * Kansio, joka elää täysin muistissa.
     */
    class VirtualDirectory : public DirectoryInterface, public VirtualParent
    {
    public:
        /**
         * @brief Luo uuden virtuaalikansion, joka on tyhjä
         * @param name - Kansion nimi
         * @param path - Kansion polku (ilman nimeä)
         * @param writable - Voiko kahvaan kirjoittaa (oletuksena voi)
         * @param connection - Mihin yhteyteen tiedosto kuuluu (oletuksena nullptr)
         * @param parent - Kansion vanhempi (oletuksena ei ole olemassa)
         */
        VirtualDirectory(QString name, QString path, bool writable = true,
                         ConnectionInterface * connection = nullptr)
            : filename(name), path(path), filePermissions(std::make_shared<int>(0777)),
              parent(), virtualConnection(connection), readWrite(writable),
              children(std::make_shared<QMap<QString, std::shared_ptr<DirectoryEntryInterface>>>()) {}
        VirtualDirectory(const VirtualDirectory & copy)
            : filename(copy.filename), path(copy.path), filePermissions(copy.filePermissions),
              parent(copy.parent), virtualConnection(copy.virtualConnection), readWrite(copy.readWrite),
              children(copy.children), selfReal(copy.lockSelf()), selfHolder(copy.lockSelf()) {}
        virtual QString name() const { return filename; }
        virtual QString fullPath() const
            {
                if (path != "")
                    return path+"/"+filename;
                else
                    return filename;
            }
        virtual int permissions() const { return *filePermissions; }
        virtual void changePermissions(int newPermissions)
            {
                if (!readWrite)
                    throw AccessDenied();
                *filePermissions = newPermissions;

            }
        virtual void remove()
            {
                if (!children->isEmpty())
                    throw DirNotEmpty();
                if (!readWrite)
                    throw AccessDenied();
                if (parent.expired())
                    throw OperationNotSupported("Virtual entry not connected to a directory");
                parent.lock()->removeChild(filename);
            }
        virtual void rename(QString newName)
            {
                if (!readWrite)
                    throw AccessDenied();
                if (!parent.expired())
                    parent.lock()->renameChild(filename,newName);
                filename = newName;
            }
        virtual DateTime lastModified() const
            { throw OperationNotSupported("virtual files doesn't support lastModified"); }
        virtual ConnectionInterface * connection() const
            { return virtualConnection; }

        virtual QList<QString> contents() const
            {
                auto result = children->keys();
                result.append(".");
                result.append("..");
                return result;
            }
        /**
         * @brief entry
         * Pääosin toimii kuten rajapinnassa on määritelty. Kuitenkin on huomattava, että mikäli
         * avaat tiedostoa "..", ei tiedoston vanhempi saa olla tuhottu.
         * Tiedostot, joiden vanhempi on asetettu luodessa arvoon nullptr, palauttavat viitteen
         * itseensä avatessa tiedostoa "..".
         *
         * @pre Mikäli tiedostolla on vanhempi, se ei saa olla tuhoutunut.
         * @param name
         * @return
         */
        virtual std::shared_ptr<DirectoryEntryInterface> entry(const QString &name) const
            {
                if ((name == ".") || (parent.expired() && name == ".."))
                    return std::shared_ptr<DirectoryEntryInterface>(new VirtualDirectory(*this));
                else if (name == "..")
                    return std::shared_ptr<DirectoryEntryInterface>(new VirtualDirectory(*(parent.lock())));
                else
                {
                    if (!hasChild(name))
                        throw FileDoesNotExist(name);
                    auto old = (*children)[name];
                    std::shared_ptr<VirtualDirectory> asDir = std::dynamic_pointer_cast<VirtualDirectory>(old);
                    if (asDir != nullptr)
                        return std::shared_ptr<DirectoryEntryInterface>(new VirtualDirectory(*asDir));
                    else
                        return std::shared_ptr<DirectoryEntryInterface>(new VirtualFile(*std::static_pointer_cast<VirtualFile>(old)));
                }
            }
        virtual bool hasChild(const QString &name) const
            {
                if (name == "." || name == "..")
                    return true;
                else
                    return children->contains((name));
            }
        virtual std::shared_ptr<DirectoryInterface> mkdir(const QString &name)
            {
                if (hasChild(name))
                    throw FileExists(name);
                auto newDir = std::shared_ptr<VirtualDirectory>(new VirtualDirectory(lockSelf(), name));
                newDir->selfReal = newDir;
                children->insert(name, newDir);
                return std::shared_ptr<DirectoryInterface>(new VirtualDirectory(*newDir));
            }
        virtual std::shared_ptr<FileInterface> mkfile(const QString &name)
            {
                if (hasChild(name))
                    throw FileExists(name);
                auto newDir = std::make_shared<VirtualFile>(name, fullPath(), readWrite, virtualConnection, lockSelf());
                children->insert(name, std::shared_ptr<DirectoryEntryInterface>(newDir));
                return std::shared_ptr<FileInterface>(new VirtualFile(*newDir));
            }
        virtual std::shared_ptr<SymbolicLinkInterface> mksymlink(const QString &, const QString &)
            { throw OperationNotSupported("Virtuaalikansiot eivät tue symbolisia linkkejä"); }

        virtual long size() const
            { throw OperationNotSupported("Virtuaalikansiot eivät tue koon laskentaa"); }

        /**
         * @brief Voiko tähän tiedostoon kirjoittaa
         * @return Onnistuuko kirjoittaminen
         * @post Poikkeustakuu: nothrow
         */
        bool canWrite() const { return readWrite; }
        /**
         * @brief Tee muokkauksesta mahdollista tai ei mahdollista.
         * @param Tosi jos tiedostoa halutaan pystyä muokkaamaan kutsun jälkeen, false jos ei
         */
        void setWritable(bool writable)
            { readWrite = writable; }
        bool operator==(const VirtualDirectory & other) const
            {
                if (children->keys() != other.children->keys() || this->permissions() != other.permissions())
                    return false;
                foreach(QString str, children->keys())
                {
                    std::shared_ptr<VirtualDirectory> mineAsDir = std::dynamic_pointer_cast<VirtualDirectory>(children->value(str));
                    std::shared_ptr<VirtualDirectory> otherAsDir = std::dynamic_pointer_cast<VirtualDirectory>(other.children->value(str));
                    if ( (mineAsDir != nullptr) && (otherAsDir != nullptr) )
                    {
                        if (*mineAsDir != *otherAsDir)
                            return false;
                    }
                    else if (mineAsDir != nullptr || otherAsDir != nullptr)
                        return false;
                    else if (  *std::static_pointer_cast<VirtualFile>(children->value(str)) !=
                               *std::static_pointer_cast<VirtualFile>(other.children->value(str)) )
                        return false;
                }
                return true;
            }
        bool operator!=(const VirtualDirectory & other) const
            { return ! ((*this) == other); }
    protected:
        virtual void removeChild(QString name)
            {
                if (children->contains(name))
                    children->remove(name);
            }
        virtual void renameChild(QString oldName, QString newName)
        {
            if (children->contains(newName))
                throw FileExists(newName);
            if (!children->contains(oldName))
                throw FileDoesNotExist(oldName);
            (*children)[newName] = (*children)[oldName];
            children->remove(oldName);
        }
    private:
        VirtualDirectory(std::shared_ptr<VirtualDirectory> parent, QString name)
            : filename(name), path(parent->fullPath()), filePermissions(std::make_shared<int>(0777)),
              parent(parent), virtualConnection(parent->connection()), readWrite(parent->readWrite),
              children(std::make_shared<QMap<QString, std::shared_ptr<DirectoryEntryInterface>>>()) {}

        VirtualDirectory() {}
        std::shared_ptr<VirtualDirectory> lockSelf() const
        {
            // If we never had anything real in selfReal make a holder copy to hold and put it there
            // Now the copy held in the holder will survive as long as us or any of our copies survive
            // and that should be long enough for all of our children.
            if (selfReal.expired())
            {
                VirtualDirectory * p = new VirtualDirectory();
                p->filename = filename;
                p->path = path;
                p->filePermissions = filePermissions;
                p->virtualConnection = virtualConnection;
                p->readWrite = readWrite;
                p->children = children;

                selfHolder = std::shared_ptr<VirtualDirectory>(p);
                selfHolder->selfReal = selfHolder;
                selfReal = selfHolder;
            }
            return selfReal.lock();
        }

        QString filename;
        QString path;
        std::shared_ptr<int> filePermissions;
        std::weak_ptr<FSInterface::VirtualDirectory> parent;
        FSInterface::ConnectionInterface * virtualConnection;
        bool readWrite;
        std::shared_ptr<QMap<QString, std::shared_ptr<DirectoryEntryInterface>>> children;
        mutable std::weak_ptr<VirtualDirectory> selfReal;
        mutable std::shared_ptr<VirtualDirectory> selfHolder;
    };
}

#endif // VIRTUALDIRECTORY_H
