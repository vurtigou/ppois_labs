#include "Species.h"

#include <string>
#include <utility>

Species::Species(std::string name, std::string diet, double min_vol,
                 std::pair<double, double> ph, std::pair<double, double> salinity,
                 std::pair<double, double> temp, std::pair<double, double> oxygen)
    : latin_name(name), diet_type(diet), min_tank_volume(min_vol),
      ph_range(ph), salinity_range(salinity),
      temperature_range(temp), oxygen_range(oxygen) {}

bool Species::is_compatible_with_species(const Species& other) const {
    bool this_eats_other = (other.diet_type.find(this->latin_name) != std::string::npos);
    bool other_eats_this = (this->diet_type.find(other.latin_name) != std::string::npos);

    return !this_eats_other && !other_eats_this;
}
