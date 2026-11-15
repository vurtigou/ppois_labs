#pragma once

#include <string>
#include <vector>

#include "WaterQuality.h"   // WaterQuality — поле-значение, нужен полный тип

class Animal;
class Tank;

// среда обитания
class Habitat {
private:
    std::string habitat_type;
    double volume_liters;
    WaterQuality water_quality;
    std::vector<Tank*> tanks; // non-owning

public:
    Habitat(std::string type = "", double volume = 0.0, const WaterQuality& water = WaterQuality());

    // проверка подходит ли среда для животного
    bool check_suitability_for_animal(const Animal& animal) const;
    double calculate_biomass() const;

    Animal* add_animal(Animal* animal);
    // проверка есть ли животное с такими id
    bool contains_animal(const std::string& animal_id) const;
    Animal* remove_animal(const std::string& animal_id);
    void add_tank(Tank* tank);

    // Геттеры
    const WaterQuality& get_water_quality() const { return water_quality; }
    WaterQuality&       get_water_quality()       { return water_quality; }
    std::string get_habitat_type() const { return habitat_type; }
};
