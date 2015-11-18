#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>
#include <memory>

/**
 * @file
 * @brief Määrittelee etäyhteyksien käsittelyyn tarvittavan rajapinnan
 */

namespace FSInterface
{
    class DirectoryInterface;
    /**
     * @brief Palvelin yhteys
     *
     * Connection-luokka kuvastaa yhtä palvelinyhteyttä, ja siltä saa yhteyden "päähakemistoa" vastaavan
     * hakemisto-olion directory()-kutsulla (vastaavasti jokaiselta hakemistoalkiolta voi kysyä sitä
     * vastaavan yhteyden).
     *
     * Jos olette tekemässä rinnakkaisuuden lisäosia ja teette monisäikeisen ohjelman,
     * täytyy huomioida että yhteysolion operaatioita ei saa kutsua samanaikaisesti useassa säikeessä.
     */
    class ConnectionInterface
    {
    public:
        /**
         * @brief Palvelimen nimi
         *
         * @pre -
         * @return palvelimen nimen
         * @post Poikkeustakuu: nothrow
         */
        virtual QString server() const = 0;
        /**
         * @brief Aloituskansion nimi
         *
         * Kansio, johon viittaavan olion directory() palauttaa
         *
         * @pre -
         * @return kansion nimen
         * @post Poikkeustakuu: nothrow
         */
        virtual QString path() const = 0;
        /**
         * @brief Aloituskansio.
         *
         * Kansio, joka sijaitsee tällä tiedostojärjestelmällä.
         *
         * @pre -
         * @return Kansion olio.
         * @post Poikkeustakuu: nothrow
         */
        virtual std::shared_ptr<DirectoryInterface> directory() = 0;

        virtual ~ConnectionInterface() = default;
    };
}
#endif // CONNECTION_H
