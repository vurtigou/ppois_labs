#pragma once

#include "Buffet.h"

class Drinks : public Buffet {
private:
    int vol;

public:
    Drinks(double price = 0.0, int vol = 0) : Buffet(price), vol(vol) {}
    int get_vol() const { return vol; }
};
