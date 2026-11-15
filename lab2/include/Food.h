#pragma once

#include <string>

#include "Buffet.h"

class Food : public Buffet {
private:
    std::string compound;
    int weight;

public:
    Food(double price = 0.0, std::string compound = "", int weight = 0)
        : Buffet(price), compound(compound), weight(weight) {}
    std::string get_compound() const { return compound; }
    int get_weight()            const { return weight; }
};
