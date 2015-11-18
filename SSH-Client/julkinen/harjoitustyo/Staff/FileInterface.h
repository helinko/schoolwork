#ifndef FILEINTERFACE_H
#define FILEINTERFACE_H

/**
 * @file
 * @brief Määrittelee rajapinnan tiedostojen käsittelyyn.
 */

#include "DirectoryEntryInterface.h"

#include <QByteArray>

namespace FSInterface
{
    /**
     * @brief Normaali tiedosto
     *
     * Luokka kuvastaa normaalia tiedostoa, jota voi lukea ja johon voi kirjoittaa.
     *
     * Periytettyjen luokkien on huolehdittava, että avatun tiedosto-olion tuhoaminen johtaa
     * samoihin operaatioihin kuin close().
     */
    class FileInterface : public DirectoryEntryInterface
    {
    public:
        /**
         * @brief Avausmoodi
         *
         * Tiedosto voidaan avata joko luettavaksi tai kirjoitettavaksi. Tällä annetaan tieto kummaksi.
         */
        enum Mode {
            /**
             * Avaa luettavaksi
             */
            READ,
            /**
              Avaa kirjoitettavaksi
            */
            WRITE
        };

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
        virtual void open(Mode mode) = 0;
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
        virtual int read(QByteArray & array) = 0;
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
        virtual int write(const QByteArray & array) = 0;
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
        virtual void close() = 0;
        virtual ~FileInterface() = default;
    };
}
#endif // FILEINTERFACE_H
