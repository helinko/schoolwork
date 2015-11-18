#ifndef INITIALIZATION_H
#define INITIALIZATION_H

/**
 * @file
 * @brief Määrittelee opiskelijoiden koodin alustuksen suorittavan funktion rajapinnan.
 */

#include <QList>
#include <QPair>
#include <QMainWindow>
#include <ConnectionInterface.h>

/**
 * @brief
 * Kaikki kurssin määrittelemät rajapinnat löytyvät FSInterface-nimiavaruudesta.
 */
namespace FSInterface
{
    /**
     * @brief Luo ohjelman pääikkuna.
     *
     * Funktiota kutsutaan SSH-yhteyksien avaamisen jälkeen luomaan ohjelman pääikkuna.
     * Funktiolle annetaan parametriksi viittaukset kaikkiin avattuihin ssh-yhteyksiin.
     *
     * @param connections Lista yhteyksiä. Yksi jokaista avattua SSH-yhteyttä kohti
     * @return Pääikkunan, jonka ohjelman alustuskoodi voi näyttää.
     */
    QMainWindow * startMain(const QList<std::shared_ptr<ConnectionInterface>> & connections);
}
#endif // INITIALIZATION_H
