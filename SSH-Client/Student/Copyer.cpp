#include <QQueue>
#include <QString>
#include <QByteArray>
#include <QList>
#include <memory>
#include <QDebug>

#include "Copyer.h"
#include "FileInterface.h"
#include "DirectoryInterface.h"

const char KIELLETTY_MERKKI = '/';
// Kopioinnissa käytettävän taulukon koko tavuina, eli paljonko kerrallaan luetaan ja kirjoitetaan.
const int TAULUKON_KOKO = 100*1024; // 100 kt

Copyer::Copyer() : dst_(nullptr), montako_kopioitava(0), montako_kopioitu(0), kopioitavien_koko_yht(0),
    kopioitujen_koko_yht(0)
{
}

void Copyer::addSource(std::shared_ptr<FSInterface::DirectoryEntryInterface> src)
{
    // Tiedetään, että src ei ole tyhjä. Se voi kuitenkin olla kansio tai tiedosto, eikä kansioita (vielä) tueta lähteenä.
    // Täytyy siis tarkistaa, että se on tiedosto. Se tehdään dynamic_pointer_cast-muunnoksella, joka samalla muuntaa
    // olion aliluokan tyyppiseksi. Castin poikkeuksista ei löytynyt tietoa.

    std::shared_ptr<FSInterface::FileInterface> src_ = std::dynamic_pointer_cast<FSInterface::FileInterface>(src);

    if ( src_ == nullptr ) {
        // Olio ei ollut tiedosto, eli tyyppiä ei tueta. Heitetään poikkeus, joka myös keskeyttää funktion suorituksen.
        // Jäsenmuuttujia ei ole vielä muutettu, joten vahva poikkeustakuu toteutuu edelleen.
        throw FSInterface::OperationNotSupported();
    }

    // Lisätään parametrina ollut osoitin jonoon. Qt:n mukaan tämä on poikkeusneutraali operaatio, eikä osoitinkaan
    // heitä poikkeuksia.
    kopiojono.enqueue(src_);

    // Päivitetään vielä muut jäsenmuuttujat.
    ++montako_kopioitava;
    kopioitavien_koko_yht += (*src_).size();
}

void Copyer::addTarget(std::shared_ptr<FSInterface::DirectoryInterface> dst)
{
    // Tiedetään, että dst ei ole nullptr. Voiko se kuitenkin olla jollain tavalla väärä?

    dst_ = dst;
}

void Copyer::copy()
{
    // Silmukka, joka kutsuu copyOnea niin kauan kuin kopioitavaa on.

    while (filesDone() < totalFiles()) {     
        copyOne();
    }
}

void Copyer::copyOne()
{

    // Tiedetään, että kohdekansio on olemassa ja kopioitavia tiedostoja on jäljellä.

    // Tarkistetaan ensin, ettei kopioitavan tiedoston nimessä ole /-merkkiä, sillä se estää uuden tiedoston luomisen.

    // Koska tuetaan vain tiedostojen kopiointia, tiedetään, että jonossa olevat ovat sitä tyyppiä.


    std::shared_ptr<FSInterface::FileInterface> kopioitava = kopiojono.dequeue();
    QString kopioitavan_nimi = kopioitava->name();

    if ( kopioitavan_nimi.indexOf(KIELLETTY_MERKKI) != -1 ) {
        throw FSInterface::UnknownError("Nimessä oli kielletty merkki");
    }

    // Tarkistetaan, onko kohdekansiossa samanniminen tiedosto kuin src.

    std::shared_ptr<FSInterface::DirectoryInterface> kohdekansio = dst_;
    if ( kohdekansio->hasChild(kopioitavan_nimi) ) {
        // Samanniminen tiedosto löytyi, heitetään poikkeus. Copyone-funktion poikkeustakuu on perus, joten siivousta ei edellytetä.
        throw FSInterface::FileExists(kopioitavan_nimi);
    }

    // Avataan lähdetiedosto luettavaksi. Jos jotain tapahtuu, kutsutaan close-funktiota, sillä openin poikkeustakuu on vain perus.
    // Toisin sanoen tiedosto, jota yritettiin avata, saattoi jäädä johonkin outoon tilaan. Kuitenkin closen pitäisi onnistua sille.
    // Lopuksi heitetään poikkeus eteenpäin.

    try {
        kopioitava->open(FSInterface::FileInterface::READ);
    }
    catch (...) {
        // Periaatteessa kaikkien poikkeusten sieppaaminen on ehkä vähän kyseenalaista, mutta niissä kaikissa pitäisi kuitenkin
        // tehdä nämä samat toimenpiteet.
        kopioitava->close();
        throw; // Sama poikkeus eteenpäin.
    }


    // Luetaan tiedostosta sisältöä taulukkoon niin kauan, että readin paluuarvo on nolla. Periaatteessa taulukon koko voitaisiin
    // asettaa niin suureksi, että kopioitava tiedosto mahtuu siihen kokonaan. QByteArray tallennetaan kuitenkin yhteen jatkuvaan
    // muistipaikkaan, joten useiden megatavujen kokoinen taulukko ei ehkä ole kovin järkevä.

    QByteArray tiedot;

    try {
        tiedot.resize(TAULUKON_KOKO);
    }
    catch (...) {
        // Jos taulukon resize epäonnistuu ja poikkeus heitetään, täytyy tiedosto sulkea.
        kopioitava->close();
        throw;
    }

    // Luodaan seuraavaksi kohdetiedosto kohdekansioon. Aiemmin on tarkistettu, että sen nimistä tiedostoa ei siellä ole.

    std::shared_ptr<FSInterface::FileInterface> kohdetiedosto = kohdekansio->mkfile(kopioitavan_nimi);


    // Avataan se kirjoitettavaksi.

    try {
        kohdetiedosto->open(FSInterface::FileInterface::WRITE);
    }
    catch (...) {
        kohdetiedosto->close();
        // Koska luominen epäonnistui, poistetaan mahdollinen tiedosto, koska sen tilasta ei ole tietoa.
        kohdetiedosto->remove();
        throw;
    }

    // Doxygenin mukaan readia kutsutaan niin kauan, kunnes se palauttaa nollan (tai heittää poikkeuksen).
    int luettu_yht = 0;

    try {
        int luettu = kopioitava->read(tiedot);

        while ( luettu != 0 ) {
            // Nyt ei tiedetä etukäteen, kuinka monella kutsulla write kirjoittaa koko taulukon tiedostoon. Sitä täytyy siis toistaa,
            // kunnes yhteensä on kirjoitettu taulukon alkuperäinen tavumäärä. Jos taulukko ei ole tullu täyteen, leikataan siitä ylimääräiset
            // pois, koska se muuten sekoittaa kirjoitusta.

            tiedot.truncate(luettu);

            int kirjoitettu = 0;
            int kirjoitettu_talla_kertaa = 0;

            while ( kirjoitettu < luettu ) {
                kirjoitettu_talla_kertaa = kohdetiedosto->write(tiedot);
                kirjoitettu += kirjoitettu_talla_kertaa;

                // Nyt taulukosta täytyy poistaa jo kirjoitettu osuus. Poistetaan indeksistä 0 alkaen "kirjoitettu_talla_kertaa" verran dataa.
                tiedot.remove(0, kirjoitettu_talla_kertaa);
            }
            luettu_yht += luettu;

            // Palautetaan tiedot alkuperäiseen kokoon
            try {
                tiedot.resize(TAULUKON_KOKO);
            }
            catch (...) {
                kopioitava->close();
                kohdetiedosto->close();
                kohdetiedosto->remove();
                throw;
            }

            luettu = kopioitava->read(tiedot);
        }
    }
    catch (...) {
        // Jos poikkeus heitetään, täytyy molemmat tiedostot sulkea ja kohdetiedosto poistaa
        kopioitava->close();
        kohdetiedosto->close();
        kohdetiedosto->remove();
        throw;
    }

    // Suljetaan tiedostot (close on nothrow).

    kopioitava->close();
    kohdetiedosto->close();

    // Ja päivitetään loput jäsenmuuttujat.
    ++montako_kopioitu;

    // Tässä taas käytetään taulukon kokoa, koska se kertoo kuinka paljon oikeasti kirjoitettiin.
    kopioitujen_koko_yht += luettu_yht;
}

int Copyer::totalFiles() const
{
    // Palautetaan kopioitavaksi asetettujen tiedostojen määrä. Tämä ei muutu kesken kopioinnin.

    return montako_kopioitava;
}

int Copyer::filesDone() const
{
    // Palautetaan jo kopioitujen tiedostojen määrä. Tämä päivittyyn jokaisen kopion jälkeen.

    return montako_kopioitu;
}

long Copyer::totalBytes() const
{
    // Kopioitavien koko, kuten niiden määrä, päivittyy kopioitavaksi lisätessä, mutta ei vähene yksittäisten kopiointien
    // jälkeen.

    return kopioitavien_koko_yht;
}

long Copyer::bytesDone() const
{
    return kopioitujen_koko_yht;
}


