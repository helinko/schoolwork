#ifndef DATETIME_H
#define DATETIME_H

/** @file
 * @brief Yksinkertainen (todella yksinkertainen) luokka päiväyksen ja kellonajan esittämiseen.
 */

#include <QString>
#include <QLatin1Char>

namespace FSInterface
{
    /**
     * @brief DateTime luokka kuvastaa yhtä ajan hetkeä.
     *
     * Luokalla voidaan esittää kellonaika ja päivämäärä käyttäen gregoriaanista kalenteria ja 24 tunnin vuorokautta.
     * Päiväyksen voi asettaa vain luomisen yhteydessä, sen jälkeen sitä ei voi muuttaa.
     */
    class DateTime
    {
    public:
        /**
         * @brief Alustaa päivämäärän asettaen kellon ajan puolille öin.
         *
         * @param year Vuosi
         * @param month Kuukausi
         * @param day Päivä
         * @pre 1 <= month <= 12
         * @pre 1 <= day <= 31
         * @post Päiväys on alustettu
         * @post Poikkeustakuu: nothrow
         */
        DateTime(int year, int month, int day) :
            year(year), month(month), day(day), hour(0), minute(0), second(0) {}
        /**
         * @brief Alustaa päivämäärän ja kellonajan
         *
         * @param year Vuosi
         * @param month Kuukausi
         * @param day Päivä
         * @param hour Tunti
         * @param minute Minuutti
         * @param second Sekunti
         * @pre 1 <= month <= 12
         * @pre 1 <= day <= 31
         * @pre 0 <= hour <= 23
         * @pre 0 <= minute <= 59
         * @pre 0 <= second <= 59
         * @post Päiväys on alustettu
         * @post Poikkeustakuu: nothrow
         */
        DateTime(int year, int month, int day, int hour, int minute, int second) :
            year(year), month(month), day(day), hour(hour), minute(minute), second(second) {}
        /**
         * @brief Vuosiluku esim. 2015
         */
        const int year;
        /**
         * @brief Kuukausi niin että, tammikuu saa arvon 1 ja joulukuu arvon 12
         */
        const int month;
        /**
         * @brief Päivä niin, että kuun ensimmäinen päivä saa arvon 1
         */
        const int day;
        /**
         * @brief Tunti niin, että vuorokauden ensimmäinen tunti saa arvon 0 ja viimenen 23
         */
        const int hour;
        /**
         * @brief Minuutti niin, että tunnin ensimmäinen minuutti saa arvon 0 ja viimeinen 59
         */
        const int minute;
        /**
         * @brief Sekunti niin, että minuutin ensimmäinen sekunti saa arvon 0 ja viimeinen 59
         */
        const int second;
        /**
         * @brief Aika ja päivä muotoiltuna merkkijonoksi
         *
         * Palauttaa arvon muodossa "pp.kk.vvvv tt:mm:ss"
         *
         * Esimerkiksi "01.12.2014 05:30:00" saadaan ajankohdalle 1. tammikuuta 2014 kello tasan puoli 6 aamulla
         *
         * @pre -
         * @return Aika merkkijonona
         * @post Poikkeustakuu: nothrow
         */
        QString toString() const
        {
            return QString("%1.%2.%3 %4:%5:%6")
                    .arg(day, 2, 10, QLatin1Char('0'))
                    .arg(month, 2, 10, QLatin1Char('0'))
                    .arg(year, 4, 10, QLatin1Char('0'))
                    .arg(hour, 2, 10, QLatin1Char('0'))
                    .arg(minute, 2, 10, QLatin1Char('0'))
                    .arg(second, 2, 10, QLatin1Char('0'));
        }
    };
}
#endif // DATETIME_H
