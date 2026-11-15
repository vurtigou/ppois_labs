#pragma once

#include "Buffet.h"

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
