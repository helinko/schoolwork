#include "LocalDirectory.h"
#include "DirectoryEntryInterface.h"
#include "LocalFile.h"
#include "DateTime.h"

#include <QDir>
#include <memory>
#include <QFileInfo>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QDebug>

const char EROTIN = '/';

LocalDirectory::LocalDirectory(QString polku, QObject *parent) :
    QObject(parent), hakemisto(nullptr)
{

    // Täytyykö olemassaolo tarkistaa?

    // Luodaan jäsenmuuttujaan QDir-olio, joka kuvaa hakemistoa.

    hakemisto = std::make_shared<QDir>(polku);

}

QString LocalDirectory::muodosta_polku(QString alkion_nimi) const
{
    QString polku = fullPath(); // Nykyisen hakemiston polku
    polku.append(EROTIN);
    polku.append(alkion_nimi);

    return polku;
}

QList<QString> LocalDirectory::contents() const
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // QDirin entryList-funktio palauttaa sisällön QStringListin muodossa. Sille voi lisätä parametrina
    // suodattimia, joista ainakaan nyt ei mikään ole käytössä.

    return hakemisto->entryList();
}


std::shared_ptr<FSInterface::DirectoryEntryInterface> LocalDirectory::entry(const QString &name) const
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Tarkistetaan ensin, löytyykö kyseistä alkiota. Jos ei, heitetään poikkeus.

    if ( ! hasChild(name) ) {
        throw FSInterface::FileDoesNotExist(name);
    }
    // Muodostetaan hakemistoalkion polku

    QString polku = muodosta_polku(name);

    // Tarkistetaan, onko hakemistoalkio hakemisto vai tiedosto, ja palautetaan sitä vastaava olio.

    QFileInfo alkio = QFileInfo(polku);

    if ( alkio.isDir() ) {
        // Luodaan uusi LocalDirectory-tyyppinen olio shared_ptr:n päähän.
        std::shared_ptr<LocalDirectory> uusi = std::make_shared<LocalDirectory>(polku);
        return uusi;
    }
    else if ( alkio.isFile() ){
        std::shared_ptr<LocalFile> uusi = std::make_shared<LocalFile>(polku);
        return uusi;
    }
    else {
        // Jos alkio ei ole hakemisto eikä tiedosto, heitetään poikkeus.
        throw FSInterface::UnknownError("tuntematon hakemistoalkio");
    }
}

bool LocalDirectory::hasChild(const QString &name) const
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();
    // QDirissä on vastaava funktio exists(name), joten käytetään suoraan sitä.
    return hakemisto->exists(name);
}

std::shared_ptr<FSInterface::DirectoryInterface> LocalDirectory::mkdir(const QString &name)
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Tarkistetaan, onko halutun niminen kansio jo olemassa. Jos mkdirin paluuarvo on false,
    // heitetään myös poikkeus.

    if (hasChild(name) or (! hakemisto->mkdir(name)) ) {
        throw FSInterface::FileExists(name);
    }

    // Päivitetään muutokset olioon.
    hakemisto->refresh();

    // Muodostetaan tiedostopolku

    QString polku = muodosta_polku(name);

    // Luodaan uutta kansiota vastaava olio shared_ptr:n päähän ja palautetaan se.

    auto uusi = std::make_shared<LocalDirectory>(polku);

    return uusi;
}

std::shared_ptr<FSInterface::FileInterface> LocalDirectory::mkfile(const QString &name)
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Muodostetaan uuden tiedoston polku.
    QString polku = muodosta_polku(name);

    // Luodaan LocalFile-tyyppinen olio ja palautetaan se.

    std::shared_ptr<LocalFile> uusi = std::make_shared<LocalFile>(polku);

    return uusi;
}

std::shared_ptr<FSInterface::SymbolicLinkInterface> LocalDirectory::mksymlink(const QString &name, const QString &target)
{
    // Tätä ei ainakaan vielä tueta, joten heitetään poikkeus
    throw FSInterface::OperationNotSupported("Symbolisen linkin luominen");
}

QString LocalDirectory::name() const
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    return hakemisto->dirName();
}

QString LocalDirectory::fullPath() const
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Palautetaan QDirin avulla tiedoston koko polku
    return hakemisto->canonicalPath(); // jos symboliset linkit halutaan mukaan, niin absolutePath().
}


long LocalDirectory::size() const
{
    // Koon laskemista ei ainakaan vielä tueta, joten heitetään poikkeus.
    throw FSInterface::OperationNotSupported("Kansion koon laskeminen");
}

int LocalDirectory::permissions() const
{
    // Ei tueta ainakaan vielä, eli heitetään poikkeus.
    throw FSInterface::OperationNotSupported("Oikeuksien kysyminen");
}

void LocalDirectory::changePermissions(int newPermissions)
{
    throw FSInterface::OperationNotSupported("Oikeuksien muuttaminen");
}

void LocalDirectory::remove()
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Poistetaan oliota vastaava hakemisto. Tällä hetkellä tuetaan ainoastaan tyhjien kansioiden poistamista.
    // Jos kansio ei ole tyhjä, rmdir palauttaa falsen.

    if ( ! hakemisto->rmdir(fullPath()) ) {
        throw FSInterface::DirNotEmpty(fullPath());
    }
    hakemisto->refresh();

}

void LocalDirectory::rename(QString newName)
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Käytetään QDirin rename-funktiota. Jos se palauttaa falsen, on nimeäminen epäonnistunut, mutta sen syystä
    // ei ole varmuutta.

    if (! hakemisto->rename(name(), newName)) {
        throw FSInterface::UnknownError("uudelleennimeäminen epäonnistui");
    }
    hakemisto->refresh();
}

FSInterface::DateTime LocalDirectory::lastModified() const
{
    // Päivitetään ensin tiedostojärjestelmä
    hakemisto->refresh();

    // Otetaan ensin QFileInfon avulla tieto viimeisestä muokkausajasta, joka on muodossa QDateTime. Koska kyseessä
    // on hakemisto, palautettava aika on sen luomisaika.

    QFileInfo alkio = QFileInfo(fullPath());

    // Luodaan QDateTime-tyyppinen olio, ja sijoitetaan siihen lastModified-funktion paluuarvo.

    QDateTime aika_ja_pvm = QDateTime(alkio.lastModified());

    QTime aika = QTime(aika_ja_pvm.time());
    QDate pvm = QDate(aika_ja_pvm.date());

    return FSInterface::DateTime(pvm.year(), pvm.month(), pvm.day(), aika.hour(), aika.minute(), aika.second());
}

FSInterface::ConnectionInterface *LocalDirectory::connection() const
{
    // Kyseessä paikallinen alkio, joten palautetaan vain nullptr.
    return nullptr;
}

LocalDirectory::~LocalDirectory()
{
    // Koska on käytetty shared_ptr:ää, mitään olioita ei ilmeisesti tarvitse tuhota.
}
