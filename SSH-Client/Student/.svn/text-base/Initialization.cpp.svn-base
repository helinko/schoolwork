#include "Initialization.h"
#include "mainwindow.h"

#include <QList>
#include <QMainWindow>
#include <QString>
#include <QMessageBox>

#include "ConnectionInterface.h"

namespace FSInterface
{

QMainWindow * startMain(const QList<std::shared_ptr<ConnectionInterface>> &connections)
{
    try {
        return new MainWindow(connections);
    }
    // Otetaan kiinni rakentajassa tapahtuneet sekalaiset poikkeukset. Niistä ei voida toipua, vaan ohjelma suljetaan virheilmoituksen jälkeen.
    catch (FSInterface::FSError &viesti) {
        // Tulostetaan error
        QString tuloste = viesti.errorStr();
        tuloste.prepend("Tapahtui virhe: ");
        tuloste.append(". Ohjelma suljetaan.");
        QMessageBox::critical(nullptr, "Fatal error", tuloste);

        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &viesti) {
        // Tulostetaan error
        QString tuloste = viesti.what();
        tuloste.prepend("Tapahtui virhe: ");
        tuloste.append(". Ohjelma suljetaan.");
        QMessageBox::critical(nullptr, "Fatal error", tuloste);
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        QMessageBox::critical(nullptr, "Fatal error", "Tapahtui odottamaton virhe.");
        std::exit(EXIT_FAILURE);
    }

}

}
