#pragma once

#include <string>
#include <utility>

/** @brief Species profile with diet, tank size, and water requirements.
 * @ingroup animals
 */
class Species {
private:
    std::string latin_name;
    std::string diet_type; // строка с названием еды
    double min_tank_volume;
    std::pair<double, double> ph_range;
    std::pair<double, double> salinity_range;
    std::pair<double, double> temperature_range;
    std::pair<double, double> oxygen_range;

public:
    Species(std::string name = "", std::string diet = "", double min_vol = 0.0,
            std::pair<double, double> ph = {6.5, 8.5},
            std::pair<double, double> salinity = {0.0, 35.0},
            std::pair<double, double> temp = {15.0, 25.0},
            std::pair<double, double> oxygen = {5.0, 10.0});

    bool is_compatible_with_species(const Species& other) const;

    std::string const get_latin_name() const { return latin_name; }
    double get_min_tank_volume() const { return min_tank_volume; }
    std::string const get_diet_type() const { return diet_type; }
    std::pair<double, double> get_ph_range() const { return ph_range; }
    std::pair<double, double> get_salinity_range() const { return salinity_range; }
    std::pair<double, double> get_temperature_range() const { return temperature_range; }
    std::pair<double, double> get_oxygen_range() const { return oxygen_range; }
};
