#ifndef COPYER_H
#define COPYER_H

#include "CopyerInterface.h"

#include <QObject>
#include <QQueue>

/**
 * @brief Kopioijaluokka
 *
 * Rajapinnan toteuttava olio kuvastaa yhtä kopiointitapahtumaa, joka voidaan suorittaa luokan copy()-metodilla.
 * Yksi kopiointitapahtuma voi sisältää monta kopioitavaa hakemistoalkiota, mutta ne kaikki kopioidaan samaan
 * hakemistoon.
 */

class Copyer : public QObject, public FSInterface::CopyerInterface
{
    Q_OBJECT

// Funktiot ja doxygen-info tuotu CopyerInterface-rajapinnasta.
public:
    Copyer();

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

    virtual void addSource(std::shared_ptr<FSInterface::DirectoryEntryInterface> src);

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

    virtual void addTarget(std::shared_ptr<FSInterface::DirectoryInterface> dst);

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

    virtual void copy();

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

    virtual void copyOne();

    /**
     * @brief Kopioitavana olevien tiedostojen määrä.
     *
     * Kopioitavaksi asetettujen tiedostojen määrä. Mikäli tuetaan kansioiden rekursiivistä kopioimista
     * tämä sisältää myös kansion sisällä olevat tiedostot ja alakansiot. Myöskin luotavat kansiot lasketaan
     * määrään.
     *
     * Nollataan copy-käskyn jälkeen, copyOnen jälkeen ei muutoksia.
     *
     * @pre -
     * @return Tiedostojen määrä
     * @post Poikkeustakuu: vahva
     * @exception FSError operaatio epäonnistui
     */

    virtual int totalFiles() const;

    /**
     * @brief Tähän mennessä valmiiksi saatujen tiedostojen määrä.
     *
     * Tähän mennessä kopioitujen tiedostojen ja luotujen kansioiden määrä.
     *
     * Nollataan copy-käskyn jälkeen.
     *
     * @pre -
     * @return Tiedostojen määrä
     * @post Poikkeustakuu: vahva
     * @exception FSError operaatio epäonnistui
     */

    virtual int filesDone() const;

    /**
     * @brief Kopioitavana olevien tiedostojen yhteenlaskettu koko.
     *
     * Nollataan copy-käskyn jälkeen, copyOnen jälkeen ei muutoksia.
     *
     * @pre -
     * @return Tiedostojen yhteenlaskettu koko
     * @post Poikkeustakuu: vahva
     * @exception FSError operaatio epäonnistui
     */

    virtual long totalBytes() const;
    /**
     * @brief Tähän mennessä valmiiksi saatujen tiedostojen yhteenlaskettu koko. Nollataan copy-käskyn jälkeen.
     *
     * @pre -
     * @return Tiedostojen yhteenlaskettu koko
     * @post Poikkeustakuu: vahva
     * @exception FSError operaatio epäonnistui
     */

    virtual long bytesDone() const;

signals:
public slots:

private:

    // Lähdetiedosto-osoittimien jono. Kun vain tiedostojen kopiointi on sallittu, oliot toteuttavat
    // FileInterfacen.
    QQueue<std::shared_ptr<FSInterface::FileInterface>> kopiojono;

    // Kopioinnin kohdekansio
    std::shared_ptr<FSInterface::DirectoryInterface> dst_;

    // Tänne tulee todnäk vielä tiedostojen määrään ja kokoon liittyviä jäsenmuuttujia.
    int montako_kopioitava;
    int montako_kopioitu;
    long kopioitavien_koko_yht;
    long kopioitujen_koko_yht;
};

#endif // COPYER_H
