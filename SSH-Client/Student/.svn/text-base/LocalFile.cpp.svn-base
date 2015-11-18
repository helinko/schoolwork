#include "LocalFile.h"
#include <fstream>
#include <QDir>
#include <memory>
#include <QFileInfo>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QFile>
#include <QDebug>

LocalFile::LocalFile(QString polku, QObject *parent) :
    QObject(parent), tiedosto(nullptr)
{
    path = polku; //Tiedoston polku QString alkiona
    tiedosto = std::make_shared<QFile>(polku);// Luodaan jäsenmuuttujaan QFile-olio, joka kuvaa tiedostoa.

}


/**
 * @brief Avaa tiedoston.
 *
 * Ennen kuin tiedostoa voidaan lukea tai siihen voidaan kirjoitaa on sen ensin avattava.
 * Tiedosto voi olla samanaikaisesti avoinna vain joko luettavaksi tai kirjoitettavaksi.
 *
 * Avattaessa olematon tiedosto kirjoitettavaksi luodaan tiedosto heti ja on kopiointioperaation vastuulla poistaa
 * keskeneräinen tulos, mikäli työ keskeytyy. FileInterface:n toteuttavat luokat eivät sitä itse tee.
 *
 * @param mode Avataanko tiedosto luettavaksi vai kirjoitettavaksi.
 * @pre Tiedosto ei saa olla avattu
 * @post Poikkeustakuu: perus
 * @exception FileDoesNotExist Mikäli yritetään avata luettavaksi tiedostoa, jota ei ole olemassa
 * @exception FSError Jos operaatio ei onnistu esimerkiksi oikeuksien puuttumisen takia tms.
 */
void LocalFile::open(Mode mode)
{
   bool onnistuiko = false;

   if(tiedosto->isOpen()) //Tarkistetaan onko tiedosto jo auki, jos on, heitetään poikkeus.
    {
        throw FSInterface::UnknownError("Tiedosto on jo auki");
    }
    if(mode == FSInterface::FileInterface::READ) //Tarkistetaan kumpi modi on valittu, Read/Write
    {
        if(!tiedosto->exists()) // Jos luettavaa tiedostoa ei ole olemassa, heitetään poikkeus
        {
            throw FSInterface::FileDoesNotExist(name());
        }
        onnistuiko = tiedosto->open(QIODevice::ReadOnly); //Luetaan tiedosto
    }

    else if(mode == FSInterface::FileInterface::WRITE) //Tarkistetaan onko valittu kirjoitus modi.
    {
        // Nyt ei tarvitse tarkistaa, onko tiedosto olemassa, sillä jos se ei ole, se pitää luoda, ja open hoitaa sen
        // automaattisesti.
        onnistuiko = tiedosto->open(QIODevice::WriteOnly); //avataan tiedosto kirjoitusta varten
    }

    if(!onnistuiko)
    {
        // Avaaminen ei onnistunut
        throw FSInterface::UnknownError("Tiedostoa ei voitu avata.");
    }

}


    /**
     * @brief Luetaan dataa tiedostosta.
     *
     * Read-operaatio lukee dataa tiedostosta annettuun taulukkoon, palauttaa luettujen tavujen määrän
     * ja siirtyy tiedostossa eteenpäin luetun data verran.
     * Operaatio pyrkii lukemaan taulukkoon tämä koon verran tavuja, mutta moista ei taaata.
     * Luku voi jäädä lyhyeksi tiedoston loppumisen tai muiden syiden takia.
     * Kuitenkin mikäli tiedosto ei ole loppunut ja virhetilanteita ei ole tapahtunut luetaan
     * aina vähintään yksi tavu. Tiedoston lopputtua palautetaan nolla. Virhetilanteissa heitetään poikkeus.
     *
     * @param array Taulukko johon dataa luetaan
     * @pre Tiedoston on oltava avattu luettavaksi metodia kutsuttaessa, eikä vielä suljettu (katso open(), close()).
     * @pre Parametriksi annettav
     *an taulukon on sisällettävä vähintään yksi tavu.
     * @return array paljonko dataa luettiin.
     * @post Tiedostosta on luettu paluuarvon verran tavuja taulukkoon.
     * @post Poikkeustakuu: perus
     * @exception FSError Jos lukeminen ei onnistu tai keskeytyy virheeseen.
     */
int LocalFile::read(QByteArray &array)
{
    // Tarkistetaan, onko tiedostossa vielä luettavaa.
    if (tiedosto->atEnd()) {
        // Tiedosto lopussa -> palautetaan nolla jotta kutsuminen loppuu
        return 0;
    }

    // Otetaan alkuperäinen koko talteen
    long koko = array.size();

    // Nykyinen toteutus toimisi ilman tätäkin, mutta jos taulukkoa ei lueta täyteen, tässä palautettaisiin
    // väärä tavumäärä.
    array.resize(0);

    // Luetaan taulukkoon sen alkuperäisen koon verran dataa.
    array = tiedosto->read(koko);

    return array.size();

}
/**
 * @brief Kirjoita dataa tiedostoon.
 *
 * Write-operaatio kirjoittaa dataa tiedoston loppuun annetusta taulukosta ja palauttaa
 * kirjoitettujen tavujen määrän. Operaatio pyrkii kirjoittamaan koko taulukon kerralla, mutta tämän
 * onnistumista ei taata. Kirjoitettu määrä voi olla pienempi kuin taulukon koko. Tässä tilanteessa tiedostoon
 * on kirjoitettu palautetun tavumäärän verran dataa taulukon alusta. Mikäli palautettu arvo
 * vastaa taulukon kokoa koko taulukko on kirjoitettu. Virhetilanteissa heitetään poikkeus.
 *
 * @param array Taulukko, jonka sisältö kirjoitetaan tiedoston loppuun
 * @pre Tiedoston on oltava avattu kirjoitettavaksi metodia kutsuttaessa, eikä vielä suljettu (katso open(), close()).
 * @pre Parametriksi annettavan taulukon on sisällettävä vähintään yksi tavu.
 * @return Montako tavua kirjoitettiin.
 * @post Tiedostoon on kirjoitettu paluuarvon verran tavuja taulukon alusta.
 * @post Poikkeustakuu: perus
 * @exception FSError Jos kirjoittaminen ei onnistu tai keskeytyy virheeseen.
 */
int LocalFile::write(const QByteArray &array)
{

     //Kirjoitetaan dataa tiedostoon.
    if(!tiedosto->isOpen()) //Jos tiedosto ei ole auki, heitetään poikkeus
    {
        throw FSInterface::UnknownError("Tiedosto ei ole auki");
    }
    else
    {
      return  tiedosto->write(array); //Lisätään dataa tiedostoon
    }
}

/**
 * @brief Sulje tiedosto
 *
 * Sulkee tiedoston ja kirjoittaa mahdollisissa puskureissa olevan datan tiedostoon. Suljettu tiedosto
 * on mahdollista avata uudelleen luettavaksi tiedoston alusta.
 *
 * Periytettyjen luokkien on huolehdittava, että avatun tiedosto-olion tuhoaminen johtaa
 * samoihin operaatioihin kuin close().
 *
 * Metodin ei sallita heittävän poikkeuksia, jotta sen käyttö olioiden purkamisessa olisi turvallista. Mahdolliset
 * virheet on käsiteltävä metodin sisällä tai jätettävä huomiotta.
 *
 * @pre -
 * @post Tiedosto on suljettu
 * @post Poikkeustakuu: nothrow
 */
void LocalFile::close()
{
    //Suljetaan tiedosto.
    tiedosto->close();
}

/**
 * @brief Hakemistoalkion nimi
 *
 * Metodi palauttaa hakemistoalkion nimen ilman kansio-osaa.
 *
 * Esimerkiksi tiedostolle
 * "/home/john/file.txt" palautetaan "file.txt"
 *
 * @pre -
 * @return hakemistoalkion nimen
 * @post Poikkeustakuu: nothrow
 */
QString LocalFile::name() const
{
    QStringList palat = path.split("/"); //hajoitetaan polu paloijin "/"- merkin välein
    QString nimi = palat.value(palat.length() -1);//haetaan viimeinen pala, joka on tiedoston nimi
    return nimi;
}


QString LocalFile::fullPath() const
{
    return path; //Palautetaan tiedoston polku
}

LocalFile::~LocalFile() //VirtuaaliPurkaja
{

}

/**
 * @brief Hakemistoalkion koko tavuina
 *
 * @pre -
 * @return Hakemistoalkion koko tavuina
 * @post Poikkeustakuu: vahva
 * @exception OperationNotSupported Mikäli koon ilmoittaminen ei ole loogista (symboliset linkit) tai sen laskeminen olisi hyvin
 * työlästä (kansiot)
 * @exception FSError Jos koon laskeminen ei onnistu (esim. hakemistoalkio on jo poistettu)
 */
long LocalFile::size() const
{
    //QByteArray tavut = tiedosto.toUtf8();
    if(tiedosto->size() < 0) //jos tiedoston koko on alle 0, heitetään poikkeus
    {
        throw FSInterface::OperationNotSupported();
    }
    else
    {
        return tiedosto->size(); //Jos poikkeustilanteita ei sattunut, palautetaan tiedoston koko tavuina.
    }
}

/**
 * @brief Palauttaa hakemistoalkion käyttöoikeudet
 *
 * Palauttaa hakemistoalkion käyttöoikeudet perinteisessä numeraalisessa Unix-muodossa.
 * Katso http://en.wikipedia.org/wiki/File_system_permissions#Notation_of_traditional_Unix_permissions
 *
 * Oikeuksista palautetaan ainoastaan alimmat 9-bittiä. Eli omistajan, ryhmän ja muiden luku-, kirjoitus- ja
 * suoritusoikeudet.
 *
 * Tämän metodin toteuttaminen on valinnaista.
 * @pre -
 * @return hakemistoalkion oikeudet Unix-mallisessa numeerisessa muodossa
 * @post Poikkeustakuu: vahva
 * @exception OperationNotSupported Jos operaatiota ei ole toteutettu
 * @exception FSError Jos oikeuksien lukeminen ei onnistu (esim. hakemistoalkio on jo poistettu)
 */
int LocalFile::permissions() const
{
    throw FSInterface::OperationNotSupported();
}

void LocalFile::changePermissions(int newPermissions)
{
    throw FSInterface::OperationNotSupported();
}
/**
 * @brief Poistaa hakemistoalkion pysyvästi tiedostojärjestelmästä.
 *
 * @pre -
 * @post Hakemistoalkio on poistettu
 * @post Poikkeustakuu: perus
 * @exception FSError Jos operaatio ei onnistunut
 */
void LocalFile::remove()
{
   //Poistetaan tiedosto
   tiedosto->remove();
   if (tiedosto->isOpen()) //jos tiedosto on poiston jälkeen vielä auki, heitetään poikkeus.
   {
       throw FSInterface::UnknownError("poisto epaonnistui");
   }
}

/**
 * @brief Uudelleennimeää hakemistoalkion.
 *
 * Nimeää tiedoston uudelleen nykyisen kansion sisällä.
 *
 * @param newName Uusi nimi
 * @pre Uusi nimi ei saa sisältää "/"-merkkejä.
 * @post Hakemistoalkio on muutettu annetun nimiseksi
 * @post Poikkeustakuu: perus
 * @exception FileExists Mikäli annetun uuden nimen omaava hakemistoalkio on jo olemassa
 * @exception FSError Jos operaatio ei onnistunut
 */
void LocalFile::rename(QString newName)
{
    //uudelleennimetään tiedosto
    if(newName == name()) //Jos vanha ja uusi nimi on sama, heitetään poikkeus
    {
        throw FSInterface::FileExists();
    }
    else{
        if(QFile::rename(name(), newName)) //uudelleennimetään tiedosto
        {
            throw FSInterface::UnknownError("Nimeäminen epaonnistui"); //Jos uudelleennimeys ei onnistunut, heitetään poikkeus.
        }
    }
}

/**
 * @brief Palauttaa hakemistoalkion edellisen muokkausajan.
 *
 * Palauttaa hakemistoalkion edellisen muokkausajan sillä tarkkuudella, jonka alla oleva tiedostojärjestelmä
 * sallii. Jos tiedostojärjestelmä tai -palvelin ei pysty tarjoamaan muokkausaikaa palautetaan hakemistoalkion
 * luomisaika. Jos sekään ei ole mahdollista heitetään poikkeus OperationNotSupported.
 *
 * Ajan on oltava Gregorianisen kalenterin mukaisessa täydellisessä paikallisessa ajassa.
 *
 * @pre -
 * @return muokkausajan.
 * @post Poikkeustakuu: vahva
 * @exception OperationNotSupported jos operaatiota ei ole toteutettu
 * @exception FSError Jos operaatio ei onnistunut
 */
FSInterface::DateTime LocalFile::lastModified() const
{
    // Otetaan ensin QFileInfon avulla tieto viimeisestä muokkausajasta, joka on muodossa QDateTime. Koska kyseessä
    // on hakemisto, palautettava aika on sen luomisaika.

    QFileInfo alkio = QFileInfo(fullPath());

    // Luodaan QDateTime-tyyppinen olio, ja sijoitetaan siihen lastModified-funktion paluuarvo.

    QDateTime aika_ja_pvm = QDateTime(alkio.lastModified());

    QTime aika = QTime(aika_ja_pvm.time());
    QDate pvm = QDate(aika_ja_pvm.date());

    return FSInterface::DateTime(pvm.year(), pvm.month(), pvm.day(), aika.hour(), aika.minute(), aika.second());

}

FSInterface::ConnectionInterface *LocalFile::connection() const
{
    // Kyseessä paikallinen alkio, joten palautetaan vain nullptr.
    return nullptr;
}
