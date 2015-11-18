#ifndef VIRTUALFILE_H
#define VIRTUALFILE_H

#include <QByteArray>
#include "FileInterface.h"

namespace FSInterface
{
    class VirtualParent
    {
        friend class VirtualFile;
    protected:
        virtual void removeChild(QString name) = 0;
        virtual void renameChild(QString oldName, QString newName) = 0;
    };

    /**
     * @brief Kokonaan muistissa oleva tiedosto.
     *
     * Tiedostoa voidaan käyttää normaalien FileInterface rajapinnan metodien kautta.
     * Kuitenkin sen käyttöoikeuksien hallintaa ei toteuteta changePermissions metodilla
     * vaan setWritable-metodilla. changePermission metodille annetut oikeudet vaikuttavat
     * vain permissions kyselyn vastauksiin.
     */
    class VirtualFile : public FileInterface
    {
    public:
        /**
         * @brief Luo uuden virtuaalitiedoston, joka on tyhjä
         * @param name - Tiedoston nimi
         * @param path - Tiedoston kansio
         * @param writable - Voiko kahvaan kirjoittaa (oletuksena voi)
         * @param connection - Mihin yhteyteen tiedosto kuuluu (oletuksena nullptr)
         * @param parent - Tiedoston vanhempi (oletuksena ei ole olemassa)
         */
        VirtualFile(QString name, QString path, bool writable = true,
                    ConnectionInterface * connection = nullptr, std::weak_ptr<VirtualParent> parent = std::weak_ptr<VirtualParent>())
            : filename(name), path(path), filePermissions(std::make_shared<int>(0777)),
              parent(parent), virtualConnection(connection), readWrite(writable),
              array(std::make_shared<QByteArray>()), opened(false), maxReadSize(-1) {}
        /**
         * @brief Luo tiedoston, joka jakaa arvonsa annetun tiedoston kanssa.
         * @param Tiedosto, jota matkitaan
         */
        VirtualFile(const VirtualFile & copy)
            : filename(copy.filename), path(copy.path), filePermissions(copy.filePermissions),
              parent(copy.parent), virtualConnection(copy.virtualConnection), readWrite(copy.readWrite),
              array(copy.array), opened(false), maxReadSize(copy.maxReadSize) {}
        virtual ~VirtualFile() = default;
        virtual QString name() const { return filename; }
        virtual QString fullPath() const { return path+"/"+filename; }
        virtual long size() const { return array->size(); }
        virtual int permissions() const { return *(filePermissions.get()); }
        virtual void changePermissions(int newPermissions)
            {   if (!readWrite)
                throw AccessDenied();
                *(filePermissions.get()) = newPermissions;
             }
        virtual void remove()
            {
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
        virtual void open(Mode newMode)
            {
                if (opened)
                    throw OperationNotSupported("Yritetään avata ennestään avattua tiedostoa");
                mode = newMode;
                location = 0;
                opened = true;
                if (mode == WRITE)
                    array->clear();
            }
        virtual int read(QByteArray &target)
            {
                if (!opened)
                    throw OperationNotSupported("Yritetään lukea avaamattonta tiedostoa");
                if (mode != READ)
                    throw OperationNotSupported("Yritetään lukea kirjoitettavaksi avatusta tiedostosta");
                int count = target.length();
                if (maxReadSize > 0)
                    count = (target.length() > maxReadSize ? maxReadSize: target.length());
                // Kuitenkin lue aina maksimissaan jäljellä olevat tavut
                count = (array->length() - location < count ? array->length() - location : count);
                memcpy(target.data(), array->data()+location, count);
                location += count;
                return count;
            }
        virtual int write(const QByteArray &target)
            {
                if (!opened)
                    throw OperationNotSupported("Yritetään kirjoittaa avaamattomaan tiedostoon");
                if (mode != WRITE)
                    throw OperationNotSupported("Yritetään kirjoittaa luettavaksi avattuun tiedostoon");
                array->append(target);
                return target.size();
            }
        virtual void close()
            {
                opened = false;
            }

        /**
         * @brief Korvaa tiedoston sisältö annetulla merkkijonolla
         * @param string Tiedoston uusi sisältö
         * @pre tiedosto ei saa olla avattu
         * @post Poikkeustakuu: perus
         */
        void fillWithString(QString string)
            {
                open(WRITE);
                write(string.toUtf8());
                close();
            }

        /**
         * @brief Jatkaa tiedoston sisältöä annettuun pituuteen random datalla.
         * Mikäli annettu koko on pienempi, kuin nykyinen koko tiedosto leikataan annetun koon kohdalta.
         * Mikäli koko on sama kuin nykyinen, koko ei tehdä mitään ja mikäli koko on suurempi kuin, nykyinen koko
         * jatketaan tiedostoa sattumanvaraisesti generoidulla tavaralla annettuun kokoon.
         * @param toSize Tiedoston uusi koko
         * @pre tiedosto ei saa olla avattu
         * @post Poikkeustakuu: perus
         */
        void extendWithRandom(int toSize)
            {
                int oldSize = array->size();
                array->resize(toSize);
                for (int i = oldSize; i < toSize; ++i)
                    array->data()[i] = 0xFF && rand();
            }

        /**
         * @brief Tosi jos tiedosto on avattu
         * @pre -
         * @return Onko tiedosto avattu
         * @post Poikkeustakuu: nothrow
         */
        bool isOpened() const { return opened; }
        /**
         * @brief Tiedoston avaustila
         * @pre Tiedoston on oltava avattu
         * @return tila johon tiedosto on avattu
         * @post Poikkeustakuu: nothrow
         */
        Mode openMode() const { return mode; }
        /**
         * @brief Voiko tähän tiedostoon kirjoittaa
         * @pre -
         * @return Onnistuuko kirjoittaminen
         * @post Poikkeustakuu: nothrow
         */
        bool canWrite() const { return readWrite; }

        /**
         * @brief Tee muokkauksesta mahdollista tai ei mahdollista.
         * @param Tosi jos tiedostoa halutaan pystyä muokkaamaan kutsun jälkeen, false jos ei
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        void setWritable(bool writable)
            { readWrite = writable; }
        /**
         * @brief Asettaa suurimman mahdollisen tavu määrän, jonka read suostuu lukemaan kerralla
         * Virtuaalitiedoston toteutus read metodista lukeaa kerrallaan aina maksimissaan tällä metodilla
         * asetetun määrän. Uusilla tiedostoilla luvun maksimikoko on asetettu rajattomaksi. Rajattoman
         * lukukoon voi palauttaa antamalla tälle metodille parametriksi -1
         * @param max uusi suurimman mahdollisen lukulohkon koko
         * @pre Parametri max ei saa olla 0 tai pienempi kuin -1
         * @post Poikkeustakuu: nothrow
         */
        void setMaxReadSize(int max)
            {
                maxReadSize = max;
            }

        /**
         * @brief operator ==
         * @pre -
         * @return Tosi mikäli tiedostojen oikeudet ja sisältö ovat identtisiä
         * @post Poikkeustakuu: nothrow
         */
        bool operator==(const VirtualFile & other) const
            {
                return  this->permissions() == other.permissions()
                        &&
                        (
                            (this->array == other.array) ||
                            (
                                this->array->size() == other.array->size()
                                && memcmp(this->array->data(), other.array->data(), this->array->size()) == 0
                             )
                        );
            }
        /**
         * @brief operator !=
         * @pre -
         * @return Tosi mikäli tiedostojen oikeudet tai sisältö eivät ole identtisiä
         * @post Poikkeustakuu: nothrow
         */
        bool operator!=(const VirtualFile & other) const
            { return !(*this == other); }
    private:
        QString filename;
        QString path;
        std::shared_ptr<int> filePermissions;
        std::weak_ptr<FSInterface::VirtualParent> parent;
        FSInterface::ConnectionInterface * virtualConnection;
        bool readWrite;
        std::shared_ptr<QByteArray> array;
        bool opened;
        Mode mode;
        int location;
        int maxReadSize;
    };
}
#endif // VIRTUALFILE_H
