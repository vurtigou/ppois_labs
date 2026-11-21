#pragma once

#include "Buffet.h"

/** @brief Alcoholic drink with a recorded volume and alcohol content.
 * @ingroup inventory
 */
class AlcoholDrinks : public Buffet {
private:
    int vol;
    int degrees;

public:
    AlcoholDrinks(double price = 0.0, int vol = 0, int degrees = 0)
        : Buffet(price), vol(vol), degrees(degrees) {}
    int get_vol()     const { return vol; }
    int get_degrees() const { return degrees; }
};
