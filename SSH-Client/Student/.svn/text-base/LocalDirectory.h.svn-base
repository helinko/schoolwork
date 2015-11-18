#ifndef LOCALDIRECTORY_H
#define LOCALDIRECTORY_H

#include <QObject>
#include <QDir>
#include "DirectoryInterface.h"

class LocalDirectory : public QObject, public FSInterface::DirectoryInterface
{
    Q_OBJECT
public:
    // Rakentajalle voi antaa parametrina tiedostopolun, joka on oletuksena käyttäjän kotihakemiston polku.
    explicit LocalDirectory(QString polku = QDir::homePath(), QObject *parent = 0);



    // DirectoryInterface interface

    /**
     * @brief Kansion sisältö.
     *
     * Palauttaa kaikkien kansiossa olevien hakemistoalkioiden nimet. Tämä sisältää myös kansioon itseensä ja kansion
     * vanhempaan viittaavat "." ja ".." oliot.
     *
     * @pre -
     * @return Kansion sisältö
     * @post Poikkeustakuu: vahva
     * @exception FSError Jos operaatio ei onnistunut
     */
    virtual QList<QString> contents() const;
    /**
     * @brief Palauttaa viitteen kansiossa olevaan tiedostoon.
     *
     * Luo viite kansiossa sijaitsevaan hakemistoalkioon. Mikäli pyydetty nimi viittaa alikansioon, palauta uusi
     * DirectoryInterface luokan toteutus. Mikäli nimi viittaa normaaliin tiedostoon, palauta FileInterface luokan toteutus.
     * Mikäli nimi on ".." palauta viite kansion vanhempaan, tai tiedostojärjestelmän juuren kohdalla kansioon itseensä.
     * Mikäli nimi on "." palauta viite kansioon itseensä.
     *
     * @param name Alialkion nimi.
     * @pre Nimi ei saa sisältää kansioiden erottamiseen käytettäviä "/"-merkkejä.
     * @return Uusi FileEntryInterface-rajapinnan toteuttava olio.
     * @post Poikkeustakuu: vahva
     * @exception FileDoesNotExist Mikäli nimen osoittamaa alkiota ei löydy
     * @exception FSError Jos operaatio ei onnistunut
     */
    virtual std::shared_ptr<FSInterface::DirectoryEntryInterface> entry(const QString &name) const;
    /**
     * @brief Onko kansiolla kysytyn niminen lapsi.
     *
     * Tarkastaa löytyykö kansiolta pyydetyn niminen lapsi. Operaatio vastaa sitä, että luettaisiin kansion
     * koko sisältö contents()-metodilla ja tarkistettaisiin löytyykö nimeä tuloksista.
     *
     * @param name Tarkistettava nimi
     * @pre Nimi ei saa sisältää kansioiden erottamiseen käytettäviä "/"-merkkejä.
     * @return True jos lapsi löytyy, muuten false
     * @post Poikkeustakuu: vahva
     * @exception FSError Jos tietoja ei saada tiedostojärjestelmältä
     */
    virtual bool hasChild(const QString &name) const;
    /**
     * @brief Luo uusi alikansio.
     *
     * Luo kansiolle pyydetyn nimisen uuden alikansion.
     *
     * @param name  Luotavan kansion nimi
     * @pre Nimi ei saa sisältää kansioiden erottamiseen käytettäviä "/"-merkkejä.
     * @return Uusi kansio
     * @post Poikkeustakuu: perus
     * @exception FSError Jos pyydetyn niminen alikansio tai -tiedosto on olemassa tai luominen ei onnistu esimerkiksi
     * oikeuksien puuttumisen takia
     */
    virtual std::shared_ptr<FSInterface::DirectoryInterface> mkdir(const QString &name);
    /**
     * @brief Luo tiedosto
     *
     * Luo kansiolle pyydetyn nimisen tiedoston.
     * Toisin kuin mkdir:ssa, mkfile:n ei tarvitse luoda fyysistä tiedostoa heti vaan tiedosto on sallittua luoda
     * vasta kun se avataan kirjoitettavaksi.
     *
     * @param name Luotavan tiedoston nimi
     * @pre Nimi ei saa sisältää kansioiden erottamiseen käytettäviä "/"-merkkejä.
     * @return Uusi tiedosto
     * @post Poikkeustakuu: perus
     * @exception FSError Jos pyydetyn niminen alikansio tai -tiedosto on olemassa tai luominen ei onnistu esimerkiksi
     * oikeuksien puuttumisen takia
     */
    virtual std::shared_ptr<FSInterface::FileInterface> mkfile(const QString &name);
    /**
     * @brief Luo symbolinen linkki
     *
     * Luo kansioon symbolisen linkin. Tämä on valinnainen operaatio.
     *
     * @param name Linkin nimi
     * @param target Linkin kohde
     * @pre Nimi ei saa sisältää kansioiden erottamiseen käytettäviä "/"-merkkejä.
     * @return Uusi symbolinen linkki
     * @post Poikkeustakuu: vahva
     * @exception FSError Jos pyydetyn niminen alkiansio tai -tiedosto on olemassa tai luominen ei onnistu esimerkiksi
     * oikeuksien puuttumisen takia.
     * @exception OperationNotSupported Jos symbolisia linkkejä ei tueta.
     */
    virtual std::shared_ptr<FSInterface::SymbolicLinkInterface> mksymlink(const QString &name, const QString &target );

    // DirectoryEntryInterface interface

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
    virtual QString name() const;
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
    virtual QString fullPath() const;

    // Tämä Doxygen-info on DirectoryInterfacesta

    /**
     * @brief Palauttaa kansion koon tai heittää poikkeuksen.
     *
     * Mikäli toteuttava luokka tukee kansion koon laskemista, tämä funktio palauttaa kansion
     * ja sen sisältämien tiedostojen ja alikansioiden koon tavuina.
     *
     * @pre -
     * @return kansion ja sen alikansioiden koko tavuina
     * @post Poikkeustakuu: vahva
     * @exception OperationNotSupported Mikäli koon laskemista ei ole toteutettu
     * @exception FSError Jos koon laskeminen ei onnistu (esim. hakemisto on jo poistettu)
     */
    virtual long size() const;

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
    virtual int permissions() const;

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
    virtual void changePermissions(int newPermissions);

    /**
     * @brief Poistaa hakemistoalkion pysyvästi tiedostojärjestelmästä.
     *
     * @pre -
     * @post Hakemistoalkio on poistettu
     * @post Poikkeustakuu: perus
     * @exception FSError Jos operaatio ei onnistunut
     */
    virtual void remove();

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
    virtual void rename(QString newName);

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
    virtual FSInterface::DateTime lastModified() const;

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
    virtual FSInterface::ConnectionInterface * connection() const;

    virtual ~LocalDirectory();

private:
    QString muodosta_polku(QString alkion_nimi) const;

    std::shared_ptr<QDir> hakemisto;


};

#endif // LOCALDIRECTORY_H
