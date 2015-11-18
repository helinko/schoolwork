#ifndef DIRECTORYINTERFACE_H
#define DIRECTORYINTERFACE_H

/**
 * @file
 * @brief Määrittelee rajapinnan hakemistojen käsittelyyn.
 */

#include "DirectoryEntryInterface.h"
#include "SymbolicLinkInterface.h"
#include <memory>
#include <QList>

namespace FSInterface
{
    class FileInterface;
    /**
     * @brief DirectoryInterface luokka kuvaa kansiota
     *
     */
    class DirectoryInterface : public DirectoryEntryInterface
    {
    public:
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
        virtual QList<QString> contents() const = 0;
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
        virtual std::shared_ptr<DirectoryEntryInterface> entry(const QString & name) const = 0;

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
        virtual bool hasChild(const QString & name) const = 0;

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
        virtual std::shared_ptr<DirectoryInterface> mkdir(const QString & name) = 0;

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
        virtual std::shared_ptr<FileInterface> mkfile(const QString & name) = 0;

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
        virtual std::shared_ptr<SymbolicLinkInterface> mksymlink(const QString &name, const QString & target) = 0;

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
        virtual long size() const = 0;

        virtual ~DirectoryInterface() = default;
    };
}
#endif // DIRECTORYINTERFACE_H
