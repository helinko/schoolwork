#include "mainwindow.h"
#include "ConnectionInterface.h"
#include "DirectoryInterface.h"
#include "ui_mainwindow.h"
#include "LocalDirectory.h"
#include "Copyer.h"
#include "sshconnection.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QObject>
#include <QDebug>
#include <QList>


// Ikkunan rakenne on sellainen, että oikealla puolella on paikallinen kansio, ja vasemmalla etäyhteydet.

MainWindow::MainWindow(const QList<std::shared_ptr<FSInterface::ConnectionInterface>> &connections, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), paikallinen_kansio(nullptr)

{
    // Rakentajan poikkeukset otetaan kiinni initialization.cpp:ssä.

    ui->setupUi(this);

    // Lisätään paikallinen kotihakemisto jäsenmuuttujaan

    paikallinen_kansio = std::make_shared<LocalDirectory> (); // ei parametria = kotihakemisto

    // Lisätään yhteydet omaan jäsenmuuttujaansa.

    for ( auto it = connections.begin(); it != connections.end(); ++it ) {
        std::shared_ptr<FSInterface::DirectoryInterface> lisattava = (*it)->directory();
        etakansiot.push_back(lisattava);
    }

    // Lisätään paikallisen kotihakemiston polku tekstikenttään.

    ui->paikallinenPolku->insert(paikallinen_kansio->fullPath());

    // Lisätään yhteydet Comboboxiin. Tehdään se connectionsin avulla, koska se on vähän helpompaa.

    for ( auto it = connections.begin(); it != connections.end(); ++it ) {
        ui->yhteysvalikko->addItem((*it)->path());
    }

    // Aloituskansion valinta yhteyksistä (combobox).

    QObject::connect( ui->yhteysvalikko, SIGNAL(activated(int)), this, SLOT(naytaEtatiedostot()) );

    // Näkymissä liikkuminen
    QObject::connect(ui->tiedostonakymaEta, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(meneEtakansioon(QListWidgetItem*)));
    QObject::connect(ui->tiedostonakymaPaik, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(menePaikalliseenKansioon(QListWidgetItem*)));

    // Näkymien päivitys checkboxin klikkauksen jälkeen
    QObject::connect(ui->naytaPiilotetut, SIGNAL(clicked()), this, SLOT(naytaEtatiedostot()));
    QObject::connect(ui->naytaPiilotetut, SIGNAL(clicked()), this, SLOT(naytaPaikallisetTiedostot()));

    // Kopioinnit
    QObject::connect(ui->kopioOikealle, SIGNAL(clicked()), this, SLOT(kopioiPaikalliseenKansioon()));
    QObject::connect(ui->kopioVasemmalle, SIGNAL(clicked()), this, SLOT(kopioiEtakansioon()));

    // Näytetään paikallinen kansio oletuksena

    naytaPaikallisetTiedostot();

    // Ja ensimmäinen yhteys
    if (! etakansiot.empty()) {
        naytaEtatiedostot();
    }


}
MainWindow::~MainWindow()
{
    delete ui;
}

// Funktio tulostaa etätietokoneen hakemistoalkiot vasemmanpuoleiseen näkymään

void MainWindow::naytaEtatiedostot() {

    try {

        // Tyhjennetään ensin näkymä vanhoista tiedostoista.

        ui->tiedostonakymaEta->clear();

        // Otetaan aloituskansio valikon indeksin perusteella. Kansiot ovat samassa järjestyksessä valikossa
        // ja jäsenmuuttujassa.

        int kansion_indeksi = ui->yhteysvalikko->currentIndex();

        std::shared_ptr<FSInterface::DirectoryInterface> avoin_kansio = etakansiot.at(kansion_indeksi);

        // Otetaan sen sisältö (hakemistoalkioiden nimet) listaan.
        QList<QString> sisalto = avoin_kansio->contents();

        for ( auto it = sisalto.begin(); it != sisalto.end(); ++it ) {

            // Jos checkboxissa ei ole merkkiä, skipataan kaikki pisteellä alkavat paitsi '.' ja '..'
            if(!ui->naytaPiilotetut->isChecked())
            {
                if(it->at(0) == QChar('.') && (it->size()>1) &&(it->at(1) != QChar('.')))
                {
                    continue;
                }
            }
            // Lisätään alkio näkymään
            ui->tiedostonakymaEta->addItem(*it);
        }

        // Päivitetään lopuksi polku Comboboxiin.

        ui->yhteysvalikko->setItemText(kansion_indeksi, avoin_kansio->fullPath());
    }
    // Jos tässä otetaan kiinni UnknownError, on todennäköisesti yhteydessä käynyt jotain odottamatonta. Suljetaan se ja jatketaan ohjelmaa
    // ilman sitä.
    catch (FSInterface::UnknownError) {
        poistaYhteys();
        tulostaError("Odottamaton virhe, yhteys katkaistiin.");
    }
    // Otetaan kiinni funktiossa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        tulostaFatalError(viesti.errorStr());
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        tulostaFatalError(viesti.what());
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        tulostaFatalError("Odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }

}

// Funktio tulostaa oman tietokoneen hakemistoalkiot oikeanpuoleiseen näkymään

void MainWindow::naytaPaikallisetTiedostot()
{
    try {
        // Tyhjennetään ensin näkymä.

        ui->tiedostonakymaPaik->clear();

        // Otetaan paikallisen hakemiston sisältö listaan.

        QList<QString> sisalto = paikallinen_kansio->contents();

        // Lisätään sisältö näkymään samoin kuin äskeisessä.
        for ( auto it = sisalto.begin(); it != sisalto.end(); ++it ) {
            if(!ui->naytaPiilotetut->isChecked())
            {
                if(it->at(0) == QChar('.') && (it->size()>1) &&(it->at(1) != QChar('.')))
                {
                    continue;
                }
            }
            ui->tiedostonakymaPaik->addItem(*it);
        }
        // Päivitetään polku tekstikenttään.

        ui->paikallinenPolku->clear();
        ui->paikallinenPolku->insert(paikallinen_kansio->fullPath());
    }
    // Otetaan kiinni funktiossa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        tulostaFatalError(viesti.errorStr());
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        tulostaFatalError(viesti.what());
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        tulostaFatalError("Odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }
}

// Liikkuminen etätiedostonäkymässä

void MainWindow::meneEtakansioon(QListWidgetItem* alkio)
{
    try {

        // Tiedostonäkymästä saadaan parametrina tieto siitä, mitä alkiota on tuplaklikattu. Otetaan sen nimi muuttujaan.

        QString alkion_nimi = alkio->text();

        // Otetaan yhteysvalikon mukainen kansio tarkasteluun.

        int kansion_indeksi = ui->yhteysvalikko->currentIndex();

        std::shared_ptr<FSInterface::DirectoryInterface> kansio = etakansiot.at(kansion_indeksi);

        std::shared_ptr<FSInterface::DirectoryEntryInterface> hakemistoalkio = nullptr;
        try {
            hakemistoalkio = kansio->entry(alkion_nimi);
        }
        catch (FSInterface::FileDoesNotExist) {
            // Tiedostoa ei löytynyt. Jos poikkeus heitetään, ei mitään muutoksia ole tehty.
            tulostaError("Tiedostoa ei löytynyt");
            return;
        }
        // Tämä alkio voi siis olla kansio tai tiedosto. Jos se ei ole kansio, ei tehdä mitään muutoksia.

        std::shared_ptr<FSInterface::DirectoryInterface> uusi_kansio = std::dynamic_pointer_cast<FSInterface::DirectoryInterface>(hakemistoalkio);

        if (uusi_kansio == nullptr ) {
            // Alkio ei ollut kansio.
            return;
        }
        // Jos kohdekansioon ei ole oikeuksia, se saadaan tietää vasta kun kutsutaan contents-funktiota naytaEtatiedostot-funktiossa.
        // Testataan siis jo nyt, ettei sieltä tule poikkeusta.
        try {
            uusi_kansio->contents();
        }
        catch (FSInterface::AccessDenied) {
            tulostaError("Kansioon ei ole oikeuksia.");
            return;
        }
        // Jos se on kansio, vaihdetaan se avoimeksi kansioksi aiemman tilalle.
        etakansiot.replace(kansion_indeksi, uusi_kansio);
        // Päivitetään näkymä
        naytaEtatiedostot();
    }

    // Jos tässä otetaan kiinni UnknownError, on todennäköisesti yhteydessä käynyt jotain odottamatonta. Suljetaan se ja jatketaan ohjelmaa
    // ilman sitä.
    catch (FSInterface::UnknownError) {
        poistaYhteys();
        tulostaError("Odottamaton virhe, yhteys katkaistiin.");
    }
    // Otetaan kiinni funktiossa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        tulostaFatalError(viesti.errorStr());
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        tulostaFatalError(viesti.what());
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        tulostaFatalError("Odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }
}

// Liikkuminen paikallisessa näkymässä

void MainWindow::menePaikalliseenKansioon(QListWidgetItem *alkio)
{
    try {
        // Otetaan alkion nimi talteen parametrista.

        QString alkion_nimi = alkio->text();

        // Otetaan sitä vastaava olio entry-funktiolla.

        std::shared_ptr<FSInterface::DirectoryEntryInterface> hakemistoalkio = nullptr;
        try {
            hakemistoalkio = paikallinen_kansio->entry(alkion_nimi);
        }
        catch (FSInterface::FileDoesNotExist) {
            // Tiedostoa ei löytynyt. Jos poikkeus heitetään, ei mitään muutoksia ole tehty.
            tulostaError("Tiedostoa ei löytynyt");
            return;
        }


        // Muunnetaan se directoryInterface-muotoon.

        std::shared_ptr<LocalDirectory> kohdekansio = std::dynamic_pointer_cast<LocalDirectory>(hakemistoalkio);

        if (kohdekansio == nullptr ) {
            // Alkio ei ollut kansio -> ei tehdä mitään.
            return;
        }

        QList<QString> lista = kohdekansio->contents();

        // Jos se on kansio, vaihdetaan se avoimeksi kansioksi aiemman tilalle. Jos sen sisältö on tyhjä (ei edes '.' tai '..'), todennäköisesti lukuoikeudet puuttuvat.
        // Joka tapauksessa silloin jäätäisiin jumiin tyhjään kansioon, joten tulostetaan virheilmoitus ja pysytään aiemmassa kansiossa.

        if (! lista.empty()) {
            // Lista ei ollut tyhjä, edetään normaalisti.
            paikallinen_kansio = kohdekansio;

            // Päivitetään näkymä.
            naytaPaikallisetTiedostot();
        }
        else {
            tulostaError("Kansion alkioita ei voida näyttää. Tämä johtuu todennäköisesti lukuoikeuksien puuttumisesta");
        }
    }
    // Otetaan kiinni funktiossa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        tulostaFatalError(viesti.errorStr());
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        tulostaFatalError(viesti.what());
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        tulostaFatalError("Odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }


}

// Kopiointi vasemmasta näkymästä oikeaan

void MainWindow::kopioiEtakansioon() {

    try {
        QMessageBox msg;

        // Laitetaan Labeliin teksti, jotta käyttäjä tietää kopioinnin olevan käynnissä.
        ui->kopioiLabel->setText("Kopiointi käynnissä");

        // Luodaan ensin Copyer-olio
        Copyer kopioija;

        // Lisätään kohdekansio kopioijaan. Otetaan valikosta 2 kohdekansion indeksi ja sitä kautta olio jäsenmuuttujasta

        int kohdehakemiston_indeksi = ui->yhteysvalikko->currentIndex();

        std::shared_ptr<FSInterface::DirectoryInterface> kohdekansio = etakansiot.at(kohdehakemiston_indeksi);

        // Lisätään kansio kohteeksi.
        kopioija.addTarget(kohdekansio);

        // Otetaan muuttujaan kaikki valitut alkiot
        QList<QListWidgetItem*> kopioitavat = ui->tiedostonakymaPaik->selectedItems();

        // Lisätään ne yksitellen kopioitavaksi for-silmukassa ja kopioidaan samantien. Tällöin, jos poikkeuksia tulee, keskeytetään kopiointi.
        // Kuitenkin poikkeusta edeltävät tiedostot on saatu kopioitua.

        for (auto it = kopioitavat.begin(); it != kopioitavat.end(); ++it) {
            // Otetaan alkiota vastaava olio entry-funktiolla, sen nimen perusteella

            // Ensin alkion nimi tiedostonäkymästä
            QString alkion_nimi = (*it)->text();

            // Sitten itse alkioon viittaava olio
            std::shared_ptr<FSInterface::DirectoryEntryInterface> hakemistoalkio = paikallinen_kansio->entry(alkion_nimi);

            // Jos hakemistoalkio on kansio, ei kopiointia vielä tueta. Tällöin addSource heittää poikkeuksen.


            // Lisätään tiedosto kopioitavaksi
            try {
                kopioija.addSource(hakemistoalkio);
            }
            // Jos kyseessä oli kansio tai muu tuntematon alkio, otetaan poikkeus kiinni ja tulostetaan ilmoitus.
            catch (FSInterface::OperationNotSupported &viesti) {
                tulostaError(viesti.errorStr());
                // Siirrytään seuraavaan kopioitavaan
                continue;
            }

            // Kopioidaan lisätty tiedosto. Jos samanniminen on olemassa kohdekansiossa, copyOne heittää poikkeuksen.
            try {
                kopioija.copyOne();
            }
            catch (FSInterface::FileExists) {
                tulostaError("Kohdekansiossa on samanniminen tiedosto");
            }
        }
        // Poistetaan "kopiointi käynnissä"-teksti
        ui->kopioiLabel->clear();

        // Ilmoitetaan, montako tiedostoa on kopioitu.
        int kopioitu = kopioija.filesDone();
        QString kopioitu_str;
        kopioitu_str.setNum(kopioitu);

        if (kopioitu == 1) {
            msg.information(this,tr(" "), tr("1 tiedosto kopioitu"));
        }
        else if (kopioitu > 1){ // Ei turhaan tulosteta, jos mitään ei kopioitu
            msg.information(this,tr(" "), kopioitu_str + tr(" tiedostoa kopioitu"));
        }


        // Päivitetään lopuksi etätiedostonäkymä, jotta kopioidut tiedostot näkyvät siellä heti.

        naytaEtatiedostot();
    }
    // Jos tässä otetaan kiinni UnknownError, on todennäköisesti yhteydessä käynyt jotain odottamatonta. Suljetaan se ja jatketaan ohjelmaa
    // ilman sitä.
    catch (FSInterface::UnknownError) {
        poistaYhteys();
        tulostaError("Odottamaton virhe, yhteys katkaistiin.");
    }
    // Otetaan kiinni funktiossa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        tulostaFatalError(viesti.errorStr());
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        tulostaFatalError(viesti.what());
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        tulostaFatalError("Odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }
}


// Kopiointi oikeasta näkymästä vasempaan.

void MainWindow::kopioiPaikalliseenKansioon() {
    try {

        //Ilmoitetaan käyttäjää kopioinnista.
        QMessageBox msg;
        ui->kopioiLabel->setText("Kopiointi käynnissä");

        // Luodaan ensin copyer-olio
        Copyer kopioija;

        // Lisätään kohdekansio (eli paikallinen kansio) kopioijaan.

        kopioija.addTarget(paikallinen_kansio);

        // Otetaan muuttujaan kaikki valitut alkiot
        QList<QListWidgetItem*> kopioitavat = ui->tiedostonakymaEta->selectedItems();

        // Lisätään ne yksitellen kopioitavaksi for-silmukassa ja kopioidaan samantien. Tällöin, jos poikkeuksia tulee, keskeytetään kopiointi.
        // Kuitenkin poikkeusta edeltävät tiedostot on saatu kopioitua.

        for (auto it = kopioitavat.begin(); it != kopioitavat.end(); ++it) {
            // Otetaan alkiota vastaava olio entry-funktiolla, sen nimen perusteella

            // Ensin alkion nimi etätiedostonäkymästä
            QString alkion_nimi = (*it)->text();

            // Valitaan oikea etäkansio
            int kansion_indeksi = ui->yhteysvalikko->currentIndex();

            std::shared_ptr<FSInterface::DirectoryInterface> kansio = etakansiot.at(kansion_indeksi);

            // Sitten alkioon viittaava olio etäkansion avulla
            std::shared_ptr<FSInterface::DirectoryEntryInterface> hakemistoalkio = kansio->entry(alkion_nimi);

            // Jos hakemistoalkio on kansio, ei kopiointia vielä tueta. Tällöin addSource heittää poikkeuksen.

            // Lisätään tiedosto kopioitavaksi
            try {
                kopioija.addSource(hakemistoalkio);
            }
            // Jos kyseessä oli kansio, otetaan poikkeus kiinni ja tulostetaan ilmoitus.
            catch (FSInterface::OperationNotSupported &viesti) {
                tulostaError(viesti.errorStr());
                // Siirrytään seuraavaan kopioitavaan
                continue;
            }

            // Kopioidaan lisätty tiedosto. Jos samanniminen on olemassa kohdekansiossa, copyOne heittää poikkeuksen.
            try {
                kopioija.copyOne();
            }
            catch (FSInterface::FileExists &viesti) {
                tulostaError(viesti.errorStr());
            }
            catch (FSInterface::AccessDenied) {
                tulostaError("Tiedostoon ei ollut oikeuksia");
            }

        }
        // Poistetaan "kopiointi käynnissä"-teksti
        ui->kopioiLabel->clear();

        // Ilmoitetaan, montako tiedostoa on kopioitu.
        int kopioitu = kopioija.filesDone();
        QString kopioitu_str;
        kopioitu_str.setNum(kopioitu);

        if (kopioitu == 1) {
            msg.information(this,tr(" "), tr("1 tiedosto kopioitu"));
        }
        else if (kopioitu > 1){ // Ei turhaan tulosteta, jos mitään ei kopioitu
            msg.information(this,tr(" "), kopioitu_str + tr(" tiedostoa kopioitu"));
        }

        // Päivitetään paikallinen näkymä, jotta kopioidut tiedostot näkyvät siellä heti.
        naytaPaikallisetTiedostot();
    }

    // Jos tässä otetaan kiinni UnknownError, on todennäköisesti yhteydessä käynyt jotain odottamatonta. Suljetaan se ja jatketaan ohjelmaa
    // ilman sitä.
    catch (FSInterface::UnknownError) {
        poistaYhteys();
        tulostaError("Odottamaton virhe, yhteys katkaistiin.");
    }

    // Otetaan kiinni funktiossa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        tulostaFatalError(viesti.errorStr());
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        tulostaFatalError(viesti.what());
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        tulostaFatalError("Odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }


}

// Virheilmoitusten tulostus kootusti, jotta niitä on helpompi muokata jälkeenpäin
void MainWindow::tulostaError(QString viesti)
{
    // Lisätään teksti viestin alkuun
    viesti.prepend("Tapahtui virhe: ");

    QMessageBox::warning(this, tr("Error"), viesti);
}

void MainWindow::tulostaFatalError(QString viesti)
{
    // Error, joka sulkee ohjelman
    viesti.prepend("Tapahtui virhe: ");
    viesti.append(". Ohjelma suljetaan.");

    QMessageBox::critical(this, tr("Fatal error"), viesti);

}
void MainWindow::poistaYhteys()
{
    // Otetaan valittu yhteys käsittelyyn.
    int yhteys = ui->yhteysvalikko->currentIndex();

    // Poistetaan yhteys jäsenmuuttujasta
    etakansiot.remove(yhteys);

    // Poistetaan se yhteysvalikosta
    ui->yhteysvalikko->removeItem(yhteys);

    // Valitaan nykyiseksi yhteydeksi ensimmäinen, jos sellainen on
    if (ui->yhteysvalikko->count() == 0) {
        // Jos yhteyksiä ei ole, tyhjennetään näkymä
        ui->tiedostonakymaEta->clear();
    }
    else {
        // Muutoin valitaan ensimmäinen yhteys
        ui->yhteysvalikko->setCurrentIndex(0);
        // Päivitetään näkymä
        naytaEtatiedostot();
    }
}

