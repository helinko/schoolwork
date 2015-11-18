#include <QString>
#include <QtTest>
#include <QDebug>
#include <memory>

#include "Copyer.h"
#include "VirtualDirectory.h"
#include "VirtualFile.h"

class CopyerTest : public QObject
{
    Q_OBJECT

public:
    CopyerTest();

private Q_SLOTS:

    void kopioiYksi();

    void kopioiYksiRandom();
    void kopioiYksiRandom_data();

    void kopioiMonta();

    void poikkeukset();

};

CopyerTest::CopyerTest()
{
}

// Kopioidaan yksi tiedosto lyhyellä tekstillä.
void CopyerTest::kopioiYksi()
{
    Copyer kopioija;

    // Luodaan virtuaalinen tiedostorakenne. Ensin päähakemisto rakentajalla.

    std::shared_ptr<FSInterface::VirtualDirectory> paahakemisto_ptr = std::make_shared<FSInterface::VirtualDirectory> ("päähak", "/");

    // Sinne tiedosto virtualdirectoryn jäsenfunktiolla
    std::shared_ptr<FSInterface::FileInterface> tiedosto_ptr = paahakemisto_ptr->mkfile("tiedosto");

    // Muunnetaan osoittimen tyyppi
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto_ptr);

    // Kirjoitetaan tiedostoon tekstiä
    vtiedosto_ptr->fillWithString("testiteksti");

    // Lisätään tiedosto kopioitavaksi
    kopioija.addSource(vtiedosto_ptr);

    // Luodaan toinen virtuaalinen kansio, muunnetaan se ja lisätään kohdekansioksi.
    std::shared_ptr<FSInterface::DirectoryInterface> hak2_ptr = paahakemisto_ptr->mkdir("hakemisto2");
    std::shared_ptr<FSInterface::VirtualDirectory> vhak2_ptr = std::dynamic_pointer_cast<FSInterface::VirtualDirectory>(hak2_ptr);
    kopioija.addTarget(vhak2_ptr);


    // Tarkistetaan jäsenfunktioiden paluuarvot

    QCOMPARE(kopioija.totalFiles(), 1);
    QCOMPARE(kopioija.totalBytes(), vtiedosto_ptr->size());
    QCOMPARE(kopioija.filesDone(), 0);
    QCOMPARE(kopioija.bytesDone(), 0l); // nolla long-tyyppisenä

    // Suoritetaan kopiointi ja tarkistetaan paluuarvot uudelleen

    kopioija.copyOne();

    QCOMPARE(kopioija.totalFiles(), 1);
    QCOMPARE(kopioija.totalBytes(), vtiedosto_ptr->size());
    QCOMPARE(kopioija.filesDone(), 1);
    QCOMPARE(kopioija.bytesDone(), vtiedosto_ptr->size());


    // Tarkistetaan, että tiedosto löytyy sieltä mistä pitää, eli lähde- ja kohdekansiosta.
    QCOMPARE(paahakemisto_ptr->hasChild("tiedosto"), true);
    QCOMPARE(vhak2_ptr->hasChild("tiedosto"), true);
    // Tarkistetaan, ettei kummassakaan ole ylimääräisiä alkioita

    QCOMPARE(paahakemisto_ptr->contents().size(), 4); // 2 lisättyä + '.' ja '..' = 4
    QCOMPARE(vhak2_ptr->contents().size(), 3);

    // Tarkistetaan, että lähdetiedoston ja kopioidun sisällöt täsmäävät
    // Ensin täytyy ottaa osoitin kopioituun tiedostoon
    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu_ptr = vhak2_ptr->entry("tiedosto");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu_ptr);

    // Tarkistetaan, että niillä on sama nimi

    QVERIFY2(vtiedosto_ptr->name() == vkopioitu_ptr->name(), "nimet ei täsmää" );

    // Tarkastetaan sisällöt ja oikeudet

    QVERIFY2( *vtiedosto_ptr == *vkopioitu_ptr, "ei täsmää");
}

// Kopioidaan erikokoisia, satunnaisesti generoituja tiedostoja. Luodaan aluksi samanlainen rakenne kuin äsken.

void CopyerTest::kopioiYksiRandom()
{
    Copyer kopioija;
    std::shared_ptr<FSInterface::VirtualDirectory> paahakemisto_ptr = std::make_shared<FSInterface::VirtualDirectory> ("päähak", "/");
    std::shared_ptr<FSInterface::FileInterface> tiedosto_ptr = paahakemisto_ptr->mkfile("tiedosto");
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto_ptr);

    std::shared_ptr<FSInterface::DirectoryInterface> hak2_ptr = paahakemisto_ptr->mkdir("hakemisto2");
    std::shared_ptr<FSInterface::VirtualDirectory> vhak2_ptr = std::dynamic_pointer_cast<FSInterface::VirtualDirectory>(hak2_ptr);


    // Kirjoitetaan tiedostoon tekstiä. Haetaan koko-parametrille arvo data-osiosta.
    QFETCH(int, koko);
    vtiedosto_ptr->extendWithRandom(koko);

    // Lisätään tiedosto kopioitavaksi ja hakemisto kohdekansioksi.
    kopioija.addSource(vtiedosto_ptr);
    kopioija.addTarget(vhak2_ptr);

    // Tarkistetaan jäsenfunktioiden paluuarvot

    QCOMPARE(kopioija.totalFiles(), 1);
    QCOMPARE(kopioija.totalBytes(), vtiedosto_ptr->size());
    QCOMPARE(kopioija.filesDone(), 0);
    QCOMPARE(kopioija.bytesDone(), 0l); // nolla long-tyyppisenä

    // Suoritetaan kopiointi ja tarkistetaan paluuarvot uudelleen

    kopioija.copyOne();

    QCOMPARE(kopioija.totalFiles(), 1);
    QCOMPARE(kopioija.totalBytes(), vtiedosto_ptr->size());
    QCOMPARE(kopioija.filesDone(), 1);
    QCOMPARE(kopioija.bytesDone(), vtiedosto_ptr->size());


    // Tarkistetaan, että tiedosto löytyy sieltä mistä pitää, eli lähde- ja kohdekansiosta.
    QCOMPARE(paahakemisto_ptr->hasChild("tiedosto"), true);
    QCOMPARE(vhak2_ptr->hasChild("tiedosto"), true);

    // Tarkistetaan, ettei kummassakaan ole ylimääräisiä alkioita

    QCOMPARE(paahakemisto_ptr->contents().size(), 4); // 2 lisättyä + '.' ja '..' = 4
    QCOMPARE(vhak2_ptr->contents().size(), 3);

    // Tarkistetaan, että lähdetiedoston ja kopioidun sisällöt täsmäävät
    // Ensin täytyy ottaa osoitin kopioituun tiedostoon
    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu_ptr = vhak2_ptr->entry("tiedosto");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu_ptr);

    // Tarkistetaan, että niillä on sama nimi

    QVERIFY2(vtiedosto_ptr->name() == vkopioitu_ptr->name(), "nimet ei täsmää" );

    // Tarkastetaan sisällöt ja oikeudet

    QVERIFY2( *vtiedosto_ptr == *vkopioitu_ptr, "ei täsmää");
}


void CopyerTest::kopioiYksiRandom_data()
{

    // Luodaan sarake, eli muuttuja jolle arvoja halutaan antaa.
    QTest::addColumn<int>("koko");

    QTest::newRow("1b") << 1;
    QTest::newRow("1k") << 1024;
    QTest::newRow("10k") << 10*1024;
    QTest::newRow("100k") << 100*1024;
    QTest::newRow("vähän alle 100k") << 100*1024 - 1; // Taulukon koko tasan 100k
    QTest::newRow("vähän yli 100k") << 100*1024 + 1;
    QTest::newRow("300k") << 300*1024;
    QTest::newRow("1M") << 1024*1024;

    // Testattu myös jopa 200M kokoisella tiedostolla, mutta sitä ei jaksa joka kerta odotella kun tietää
    // sen menevän läpi.
}

// Luodaan 5 satunnaista erikokoista tiedostoa, kopioidaan ensin yksi ja sitten loput. Ei ehkä mikään kaunein toteutus, mutta näin
// se oli nopein tehdä aiempien perusteella.

void CopyerTest::kopioiMonta()
{
    Copyer kopioija;

    // Luodaan ensin päähakemisto

    std::shared_ptr<FSInterface::VirtualDirectory> paahakemisto_ptr = std::make_shared<FSInterface::VirtualDirectory> ("päähak", "/");

    // Tehdään sinne muutama tiedosto

    std::shared_ptr<FSInterface::FileInterface> tiedosto1_ptr = paahakemisto_ptr->mkfile("tiedosto1");
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto1_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto1_ptr);

    std::shared_ptr<FSInterface::FileInterface> tiedosto2_ptr = paahakemisto_ptr->mkfile("tiedosto2");
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto2_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto2_ptr);

    std::shared_ptr<FSInterface::FileInterface> tiedosto3_ptr = paahakemisto_ptr->mkfile("tiedosto3");
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto3_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto3_ptr);

    std::shared_ptr<FSInterface::FileInterface> tiedosto4_ptr = paahakemisto_ptr->mkfile("tiedosto4");
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto4_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto4_ptr);

    std::shared_ptr<FSInterface::FileInterface> tiedosto5_ptr = paahakemisto_ptr->mkfile("tiedosto5");
    std::shared_ptr<FSInterface::VirtualFile> vtiedosto5_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(tiedosto5_ptr);

    // Kirjoitetaan niihin jotain satunnaista

    vtiedosto1_ptr->extendWithRandom(1000);
    vtiedosto2_ptr->extendWithRandom(200);
    vtiedosto3_ptr->extendWithRandom(4000);
    vtiedosto4_ptr->extendWithRandom(20);
    vtiedosto5_ptr->extendWithRandom(10);

    // Lisätään tiedostot kopiojonoon

    kopioija.addSource(vtiedosto1_ptr);
    kopioija.addSource(vtiedosto2_ptr);
    kopioija.addSource(vtiedosto3_ptr);
    kopioija.addSource(vtiedosto4_ptr);
    kopioija.addSource(vtiedosto5_ptr);


    // Luodaan kohdehakemisto ja asetetaan se kohdekansioksi
    std::shared_ptr<FSInterface::DirectoryInterface> hak2_ptr = paahakemisto_ptr->mkdir("hakemisto2");
    std::shared_ptr<FSInterface::VirtualDirectory> vhak2_ptr = std::dynamic_pointer_cast<FSInterface::VirtualDirectory>(hak2_ptr);
    kopioija.addTarget(vhak2_ptr);

    // Tarkistetaan jäsenfunktioiden paluuarvot

    QCOMPARE(kopioija.totalFiles(), 5);
    long total_size = vtiedosto1_ptr->size() + vtiedosto2_ptr->size() +vtiedosto3_ptr->size() + vtiedosto4_ptr->size() + vtiedosto5_ptr->size();
    QCOMPARE(kopioija.totalBytes(), total_size);
    QCOMPARE(kopioija.filesDone(), 0);
    QCOMPARE(kopioija.bytesDone(), 0l); // nolla long-tyyppisenä

    // kopioidaan yksi ja tarkastetaan jäsenmuuttujat

    kopioija.copyOne();

    QCOMPARE(kopioija.totalFiles(), 5);
    QCOMPARE(kopioija.totalBytes(), total_size);
    QCOMPARE(kopioija.filesDone(), 1);
    QCOMPARE(kopioija.bytesDone(), vtiedosto1_ptr->size());

    // Suoritetaan kopiointi ja tarkistetaan paluuarvot uudelleen

    kopioija.copy();

    // Nämä nollaantuvat nyt.
    QCOMPARE(kopioija.totalFiles(), 0);
    QCOMPARE(kopioija.totalBytes(), 0l);
    QCOMPARE(kopioija.filesDone(), 0);
    QCOMPARE(kopioija.bytesDone(), 0l);


    // Tarkistetaan, ettei kummassakaan ole ylimääräisiä alkioita

    QCOMPARE(paahakemisto_ptr->contents().size(), 8); // 6 lisättyä + '.' ja '..' = 8
    QCOMPARE(vhak2_ptr->contents().size(), 7); // 5 kopioitua + '.' ja '..' = 7

    // Tarkistetaan, että lähdetiedoston ja kopioidun sisällöt täsmäävät
    // Ensin täytyy ottaa osoittimet kopioituihin tiedostoihin

    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu1_ptr = vhak2_ptr->entry("tiedosto1");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu1_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu1_ptr);
    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu2_ptr = vhak2_ptr->entry("tiedosto2");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu2_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu2_ptr);
    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu3_ptr = vhak2_ptr->entry("tiedosto3");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu3_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu3_ptr);
    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu4_ptr = vhak2_ptr->entry("tiedosto4");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu4_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu4_ptr);
    std::shared_ptr<FSInterface::DirectoryEntryInterface> kopioitu5_ptr = vhak2_ptr->entry("tiedosto5");
    std::shared_ptr<FSInterface::VirtualFile> vkopioitu5_ptr = std::dynamic_pointer_cast<FSInterface::VirtualFile>(kopioitu5_ptr);


    // Tarkistetaan, että niillä on sama nimi

//    QVERIFY2(vtiedosto_ptr->name() == vkopioitu_ptr->name(), "nimet ei täsmää" );

    // Tarkastetaan sisällöt ja oikeudet

    QVERIFY2( *vtiedosto1_ptr == *vkopioitu1_ptr, "ei täsmää");
    QVERIFY2( *vtiedosto2_ptr == *vkopioitu2_ptr, "ei täsmää");
    QVERIFY2( *vtiedosto3_ptr == *vkopioitu3_ptr, "ei täsmää");
    QVERIFY2( *vtiedosto4_ptr == *vkopioitu4_ptr, "ei täsmää");
    QVERIFY2( *vtiedosto5_ptr == *vkopioitu5_ptr, "ei täsmää");

}

void CopyerTest::poikkeukset()
{
    Copyer kopioija;

    // Luodaan päähakemisto.

    std::shared_ptr<FSInterface::VirtualDirectory> paahakemisto_ptr = std::make_shared<FSInterface::VirtualDirectory> ("päähak", "/");

    // Jos hakemisto lisätään lähteeksi, täytyy tulla poikkeus.
    QVERIFY_EXCEPTION_THROWN(kopioija.addSource(paahakemisto_ptr), FSInterface::OperationNotSupported);
}

QTEST_APPLESS_MAIN(CopyerTest)

#include "CopyerTest.moc"
