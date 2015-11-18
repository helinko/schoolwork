#ifndef SYMBOLICLINKINTERFACE_H
#define SYMBOLICLINKINTERFACE_H

#include "DirectoryEntryInterface.h"

#include <memory>

namespace FSInterface
{
    /**
     * @brief Symbolinen linkki
     *
     * DirectoryEntry:t, jotka ovat symbolisia linkkejä, toteuttavat tämän
     * rajapinnan.
     */
    class SymbolicLinkInterface : public DirectoryEntryInterface
    {
    public:
        /**
         * @brief Linkin sisältö
         *
         * @pre -
         * @return Linkkiin tallennetun tiedostoviittauksen muuttamattomana.
         * @post Poikkeustakuu: vahva
         * @exception FSError Jos operaatio ei onnistu
         */
        virtual QString content() const = 0;
        /**
         * @brief Linkin kohdetiedosto
         *
         * Palauttaa linkin kohteen. Mikäli linkin päässä on toinen symbolinen linkki sitä saatetaan seurata eteenpäin.
         * Jos kohdetta ei löydy, heitetään poikkeus. Jos linkit muodostavat silmukan ja jonossa olevia
         * linkkejä seurattiin, toimintaa ei ole määritelty.
         *
         * @pre -
         * @return Tiedosto, johon tämä linkki osoittaa.
         * @post Poikkeustakuu: vahva
         * @exception FSError Jos operaatio ei onnistu
         */
        virtual std::shared_ptr<DirectoryEntryInterface> target() const = 0;

        virtual ~SymbolicLinkInterface() = default;
    };
}
#endif // SYMBOLICLINKINTERFACE_H
