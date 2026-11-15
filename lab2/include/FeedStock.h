#pragma once

#include <string>

class FeedStock {
private:
    std::string name;
    double weight_kg;       // Вес корма в килограммах

public:
    FeedStock(std::string name = "", double weight_kg = 0.0)
        : name(name), weight_kg(weight_kg) {}

    std::string get_name() const { return name; }

    double get_weight_kg() const { return weight_kg; }

    void set_name(const std::string& new_name) { name = new_name; }
    void set_weight_kg(double new_weight) { weight_kg = new_weight; }
};
