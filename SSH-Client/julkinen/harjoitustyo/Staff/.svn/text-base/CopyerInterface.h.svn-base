
#ifndef COPYERINTERFACE_H
#define COPYERINTERFACE_H

/**
 * @file
 * @brief Määrittelee kopiointiin käytettävän rajapinnan (yksikkötestauksen
 *        yhtenäistämiseksi).
 */

#include "DirectoryEntryInterface.h"
#include "DirectoryInterface.h"

namespace FSInterface
{
    /**
     * @brief Kopioijaluokka
     *
     * Rajapinnan toteuttava olio kuvastaa yhtä kopiointitapahtumaa, joka voidaan suorittaa luokan copy()-metodilla.
     * Yksi kopiointitapahtuma voi sisältää monta kopioitavaa hakemistoalkiota, mutta ne kaikki kopioidaan samaan
     * hakemistoon.
     */
    class CopyerInterface
    {
    public:
        /**
         * @brief Lisää lähdetiedosto kopioitavaksi
         *
         * Lisää tiedoston kopioitavana olevien listaan.
         *
         * @param src Kopioitavaksi lisättävä hakemistoalkio
         * @pre src ei ole tyhjä osoitin
         * @pre Kopiointia ei ole vielä aloitettu (metodia copy() tai copyOne() ei ole vielä kutsuttu)
         * @post src on lisätty kopioitavaksi
         * @post Poikkeustakuu: vahva
         * @exception OperationNotSupported Mikäli tiedosto on tyyppiä, jonka kopioimista ei tueta
         * @exception FSError operaatio epäonnistui
         */
        virtual void addSource(std::shared_ptr<DirectoryEntryInterface> src) = 0;
        /**
         * @brief Aseta kohdekansio
         *
         * Asettaa kopioinnin kohdekansion.
         *
         * @param dst Kohdekansio
         * @pre dst ei ole tyhjä osoitin
         * @pre Kopiointia ei ole vielä aloitettu (metodia copy() tai copyOne() ei ole vielä kutsuttu)
         * @post dst on tulevan kopioinnin kohdekansio
         * @post Poikkeustakuu: vahva
         * @exception FSError operaatio epäonnistui
         */
        virtual void addTarget(std::shared_ptr<DirectoryInterface> dst) = 0;
        /**
         * @brief Kopioi tiedostot
         *
         * Kopioi vielä kopioimattomat addSource() käskyllä määrätyt tiedostot addTarget() kansioon.
         *
         * @pre Kohdekansio on asetettu addTarget()-kutsulla
         * @pre filesDone() < totalFiles()
         * @post Kaikki lisätyt lähteet on kopioitu
         * @post Poikkeustakuu: perus
         * @exception FSError Kopiointi epäonnistui
         */
        virtual void copy() = 0;
        /**
         * @brief Kopioi seuraavan tiedoston/hakemiston
         *
         * Kopioi seuraavan addSource()-käskyllä määrätyn tiedoston (tai hakemiston) addTarget() kansioon.
         * Jos työ tukee hakemistojen rekursiivista kopiointia, voi toteutus itse valita kopioiko copyOne()-kutsu kokonaisen
         * hakemiston kerrallaan vai eteneekö se aina yksi tiedosto kerrallaan.
         *
         * @pre Kohdekansio on asetettu addTarget()-kutsulla
         * @pre filesDone() < totalFiles()
         * @post Yksi lähde on kopioitu (hakemiston tapauksessa joko koko hakemisto tai yksi sen sisällä oleva tiedosto)
         * @post Poikkeustakuu: perus
         * @exception FSError Kopiointi epäonnistui
         */
        virtual void copyOne() = 0;
        /**
         * @brief Kopioitavana olevien tiedostojen määrä.
         *
         * Kopioitavaksi asetettujen tiedostojen määrä. Mikäli tuetaan kansioiden rekursiivistä kopioimista
         * tämä sisältää myös kansion sisällä olevat tiedostot ja alakansiot. Myöskin luotavat kansiot lasketaan
         * määrään.
         *
         * @pre -
         * @return Tiedostojen määrä
         * @post Poikkeustakuu: vahva
         * @exception FSError operaatio epäonnistui
         */
        virtual int totalFiles() const = 0;
        /**
         * @brief Tähän mennessä valmiiksi saatujen tiedostojen määrä.
         *
         * Tähän mennessä kopioitujen tiedostojen ja luotujen kansioiden määrä.
         *
         * @pre -
         * @return Tiedostojen määrä
         * @post Poikkeustakuu: vahva
         * @exception FSError operaatio epäonnistui
         */
        virtual int filesDone() const = 0;
        /**
         * @brief Kopioitavana olevien tiedostojen yhteenlaskettu koko.
         *
         * @pre -
         * @return Tiedostojen yhteenlaskettu koko
         * @post Poikkeustakuu: vahva
         * @exception FSError operaatio epäonnistui
         */
        virtual long totalBytes() const = 0;
        /**
         * @brief Tähän mennessä valmiiksi saatujen tiedostojen yhteenlaskettu koko.
         *
         * @pre -
         * @return Tiedostojen yhteenlaskettu koko
         * @post Poikkeustakuu: vahva
         * @exception FSError operaatio epäonnistui
         */
        virtual long bytesDone() const = 0;

        virtual ~CopyerInterface() = default;
    };
}
#endif // COPYERINTERFACE_H
