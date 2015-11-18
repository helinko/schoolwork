#ifndef DIRECTORYENTRYINTERFACE_H
#define DIRECTORYENTRYINTERFACE_H

/**
 * @file
 * @brief Määrittelee rajapinnan hakemistoalkioiden käsittelyyn.
 */

#include "ConnectionInterface.h"
#include "DateTime.h"

#include <QString>

namespace FSInterface
{
    /**
     * @brief DirectoryEntryInterface edustaa elementtiä tiedostojärjestelmässä.
     *
     * Jokainen tiedostojärjestelmässä oleva tiedosto tai kansio toteuttaa DirectoryEntryInterface:n, joko
     * FileInterface:n tai DirectoryInterface:n kautta. Rajapintaa ei kuulu periyttää suoraan vaan
     * nimenomaan jomman kumman näistä kautta.
     *
     * Kaikki DirectoryEntryInterfacestä perittyjen luokkien funktiot heittävät FSError luokasta periytetyn poikkeuksen
     * virhetilanteessa. Lähtökohtaisesti kaikki virheet ovat mahdollisia kaikille funktioille, ellei muuta mainita.
     *
     * Jos olette tekemässä rinnakkaisuuden lisäosia ja teette monisäikeisen ohjelman,
     * täytyy huomioida että hakemistoalkion operaatioita ei saa kutsua samanaikaisesti useassa säikeessä.
     */
    class DirectoryEntryInterface
    {
    public:
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
        virtual QString name() const = 0;
        /**
         * @brief Nimi ja polku
         *
         * Metodi palauttaa hakemistoalkion täyden kansiopolun, ts. polun juurikansiosta hakemistoalkioon itseensä.
         * Jos esim. tiedosto/hakemisto "john" on hakemistossa "home", joka on juurihakemistossa, palautetaan "/home/john".
         *
         * @pre -
         * @return täydellinen hakemistoalkion nimi
         * @post Poikkeustakuu: nothrow
         */
        virtual QString fullPath() const = 0;

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
        virtual long size() const = 0;

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
        virtual int permissions() const = 0;

        /**
         * @brief Muuttaa hakemistoalkion käyttöoikeuksia
         *
         * Vaihtaa hakemistoalkion käyttöoikeudet. Uudet käyttöoikeudet tulee antaa samassa muodossa kuin permissions()
         * palauttaa. Mikäli muuttaminen ei onnistu koska alla oleva tiedostojärjestelmä ei tue muuttamista tai
         * koska oikeudet ei riitä heitetään FSError luokasta periytetty poikkeus. Luultavimmin OperationNotSupported
         * tai AccessDenied.
         *
         * Metodin toteuttaminen on valinnaista. Mikäli haluatte hakemistoalkioiden oikeuksien hallinasta saatavat
         * lisäpisteet on metodi toteutettava.
         *
         * @param newPermissions uudet oikeudet.
         * @pre Parametrina annetut oikeusyhdistelmä on laillinen
         * @post Poikkeustakuu: perus
         * @exception OperationNotSupported  Jos operaatiota ei ole toteutettu
         * @exception FSError Jos operaatio ei onnistunut
         */
        virtual void changePermissions(int newPermissions) = 0;

        /**
         * @brief Poistaa hakemistoalkion pysyvästi tiedostojärjestelmästä.
         *
         * @pre -
         * @post Hakemistoalkio on poistettu
         * @post Poikkeustakuu: perus
         * @exception FSError Jos operaatio ei onnistunut
         */
        virtual void remove() = 0;

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
        virtual void rename(QString newName) = 0;

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
        virtual DateTime lastModified() const = 0;

        /**
         * @brief Palauttaa hakemistoalkioon liittyvän yhteysolion, mikäli sellainen on olemassa.
         *
         * Tämä funktio palauttaa hakemistoalkioon liittyvän yhteyden tai mikäli kyse on paikallisesta hakemistoalkiosta
         * nullptr:in.
         *
         * @pre -
         * @return hakemistoalkioon liittyvä yhteys.
         * @post Poikkeustakuu: nothrow
         */
        virtual ConnectionInterface * connection() const = 0;
        virtual ~DirectoryEntryInterface() = default;
    };

    /**
     * @brief FSError on kantaluokka kaikille tiedostojärjestelmäoperaation aikaisista virheistä heitettäville
     * poikkeuksille.
     */
    class FSError
    {
    public:
        /**
         * @brief Virheilmoitusta kuvaa viesti
         *
         * @pre -
         * @return Virhettä selventävän viestin
         * @post Poikkeustakuu: nothrow
         */
        QString errorStr()
        {
            return myError;
        }
    protected:
        /**
         * @brief Rakentaja periytettyjä poikkeusluokkia varten
         *
         * Tätä rakentajaa pystyy kutsumaan vain periytetyistä luokista virheviestin asettamista varten
         *
         * @param string Poikkeusolioon talletettava viestimerkkijono.
         * @pre -
         * @post Olion errorStr()-metodin kutsuminen palauttaa tästedes annetun viestin.
         * @post Poikkeustakuu: nothrow
         */
        FSError(QString string) : myError(string) {}
    private:
        QString myError;

    };

    /**
     * @brief Operaatiota ei ole tuettu
     *
     * Yritettiin suorittaa operaatio jota ei ole tuettu alla olevassa järjestelmässä
     * pyydetyille tiedostotyypeille.
     */
    class OperationNotSupported : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        OperationNotSupported() : FSError("Operation not supported") {}
        /**
         * @brief Luo OperationNotSupported-poikkeuksen
         *
         * @param operation Toteuttamattoman operaation nimi
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        OperationNotSupported(QString operation) : FSError("Operation not supported: "+operation) {}
    };

    /**
     * @brief Hakemistoalkiota ei ole olemassa.
     *
     * Yritettiin käyttää hakemistoalkiota, jota ei ollut olemassa.
     * Hakemistoalkio voi olla poistettu ohjelman tai jonkun toisen toimesta
     * tai sitä ei ollutkaan.
     */
    class FileDoesNotExist : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        FileDoesNotExist(): FSError("File doesn't exists") {}
        /**
         * @brief Luo FileDoesNotExist
         *
         * @param file Puuttuvan hakemistoalkion nimi
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        FileDoesNotExist(QString file) : FSError("File doesn't exists: "+file) {}
    };

    /**
     * @brief Hakemistoalkio ei ole kansio.
     *
     * Normaalille tiedostolle yritettiin suorittaa kansio-operaatiota tai hakemistoalkion polun osana
     * oleva komponentti ei ole kansio.
     */
    class NotADir : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        NotADir() : FSError("Not a directory") {}
        /**
         * @brief Alusta NotADir
         *
         * @param file Ongelman aiheuttanut hakemistoalkio
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        NotADir(QString file) : FSError("Not a directory: "+file) {}
    };

    /**
     * @brief Hakemistoalkio on kansio.
     *
     * Yritettiin suorittaa normaalille tiedostolle tarkoitettuja operaatiota kansiolle.
     */
    class IsADir : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        IsADir() : FSError("Is directory") {}
        /**
         * @brief Alusta IsADir -virhe
         *
         * @param file Ongelman aiheuttanut hakemistoalkio
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        IsADir(QString file) : FSError("Is a directory: "+file) {}
    };

    /**
     * @brief Hakemistoalkio on jo olemassa.
     *
     * Yritettiin luoda hakemistoalkio, joka on jo ennestään olemassa.
     */
    class FileExists : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        FileExists() : FSError("File exists") {}
        /**
         * @brief Alusta FileExists -virhe
         *
         * @param file Hakemistoalkio, joka on jo olemassa
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        FileExists(QString file) : FSError("File exists:" +file) {}
    };

    /**
     * @brief Lupaa ei annettu.
     *
     * Yritettiin tehdä jotain johon ei pystytä. Oikeudet eivät riittäneet tai järjestelmä ei tue operaatiota.
     */
    class AccessDenied : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        AccessDenied() : FSError("Access Denied") {}
    };

    /**
     * @brief Kansio ei ole tyhjä
     *
     * Yritetiin poistaa kansio, jossa on vielä hakemistoalkioita.
     */
    class DirNotEmpty : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        DirNotEmpty() : FSError("Dir not empty") {}
        /**
         * @brief Alusta DirNotEmpty- virhe
         *
         * @param path Kansio, joka ei ole tyhjä.
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        DirNotEmpty(QString path) : FSError("Dir not empty: "+path) {}
    };

    /**
     * @brief Tuntematon virhe
     *
     * Tapahtui virhe, jota ei pystytty yksilöimään muihin määritettyihin virhetiloihin. Tämä voi johtua myös
     * siitä, että palvelin kieltäytyi ilmoittamasta, mikä meni pieleen. Tämä voi myöskin tarkoittaa ongelmaa
     * SSH-yhteydessä.
     *
     * Mukana oleva virhe viesti saattaa tarjota lisäapua (tai sit ei)
     */
    class UnknownError : public FSError
    {
    public:
        /**
         * @brief Oletusrakentaja.
         *
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        UnknownError() : FSError("IO Error") {}
        /**
         * @brief UnknownError
         *
         * @param message Virhetilaa kuvaava viesti.
         * @pre -
         * @post Poikkeustakuu: nothrow
         */
        UnknownError(QString message) : FSError("IO Error: "+message) {}
    };
}

#endif // DIRECTORYENTRYINTERFACE_H
