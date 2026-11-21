#include "Habitat.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Animal.h"
#include "Tank.h"
#include "exceptions.h"   // NullPointerError, InvalidActionError, DuplicateEntityError,
                          // UnsuitableHabitatError, OceanariumError
#include "WaterQuality.h"

Habitat::Habitat(std::string type, double volume, const WaterQuality& water)
    : habitat_type(type), volume_liters(volume), water_quality(water) {}

bool Habitat::check_suitability_for_animal(const Animal& animal) const {
    // 1. Получаем нормы для вида этого животного
    std::pair<double, double> ph_norm   = animal.get_species().get_ph_range();
    std::pair<double, double> sal_norm  = animal.get_species().get_salinity_range();
    std::pair<double, double> temp_norm = animal.get_species().get_temperature_range();
    std::pair<double, double> oxy_norm  = animal.get_species().get_oxygen_range();

    // 2. Получаем текущие показатели воды в этой среде обитания
    double current_ph   = water_quality.get_ph();
    double current_sal  = water_quality.get_salinity();
    double current_temp = water_quality.get_temperature();
    double current_oxy  = water_quality.get_oxygen_level();

    // 3. Проверяем каждый показатель (входит ли текущее значение в интервал [min, max])
    bool ph_ok   = (current_ph   >= ph_norm.first   && current_ph   <= ph_norm.second);
    bool sal_ok  = (current_sal  >= sal_norm.first  && current_sal  <= sal_norm.second);
    bool temp_ok = (current_temp >= temp_norm.first && current_temp <= temp_norm.second);
    bool oxy_ok  = (current_oxy  >= oxy_norm.first  && current_oxy  <= oxy_norm.second);

    return volume_liters >= animal.get_species().get_min_tank_volume()
        && ph_ok && sal_ok && temp_ok && oxy_ok;
}

double Habitat::calculate_biomass() const {
    double total_biomass = 0.0;
    for (const auto* tank : tanks) {
        if (tank != nullptr) {
            const std::vector<Animal*>& tank_animals = tank->get_animals();
            for (const auto* animal : tank_animals) {
                if (animal != nullptr) {
                    total_biomass += animal->get_weight();
                }
            }
        }
    }
    return total_biomass;
}

Animal* Habitat::add_animal(Animal* animal) {
    if (!animal)
        throw NullPointerError("Habitat: Cannot add null animal.");

    if (!animal->can_be_moved_to_habitat()) {
        throw InvalidActionError("Habitat: Animal " + animal->get_id()
                                 + " cannot be moved to a habitat.");
    }

    if (contains_animal(animal->get_id()))
        throw DuplicateEntityError("Habitat: Animal " + animal->get_id() + " already in this habitat.");

    if (!check_suitability_for_animal(*animal)) {
        throw UnsuitableHabitatError("Habitat: Unsuitable environment for animal " + animal->get_id());
    }

    Tank* target_tank = nullptr;
    for (auto* tank : tanks) {
        if (tank && tank->get_animal_count() < 10) { target_tank = tank; break; }
    }
    if (!target_tank) {
        if (tanks.empty()) throw OceanariumError("Habitat: No tanks available.");
        throw OceanariumError("Habitat: All tanks are full (max 10 animals per tank).");
    }

    Habitat* prev_habitat = animal->get_habitat();
    Tank* prev_tank = animal->get_current_tank();

    target_tank->add_animal(animal);
    if (prev_habitat != nullptr && prev_habitat != this) {
        prev_habitat->remove_animal(animal->get_id());
    } else if (prev_habitat == nullptr && prev_tank != nullptr) {
        // Животное было в карантине (или просто в отсоединенном танке)
        prev_tank->remove_animal(animal->get_id());
    }

    animal->move_to_habitat(this);

    std::cout << "Animal " << animal->get_id() << " added to tank "
              << target_tank->get_tank_id() << " in habitat." << std::endl;
    return animal;
}

bool Habitat::contains_animal(const std::string& animal_id) const {
    for (const auto* tank : tanks) {
        if (tank != nullptr) {
            if (tank->contains_animal(animal_id)) {
                return true;
            }
        }
    }
    return false;
}

Animal* Habitat::remove_animal(const std::string& animal_id) {
    for (auto* tank : tanks) {
        if (tank && tank->contains_animal(animal_id)) {
            Animal* removed = tank->remove_animal(animal_id);   // сначала убрать из танка
            if (removed) removed->move_to_habitat(nullptr);     // потом обнулить среду
            return removed;
        }
    }
    return nullptr;
}

void Habitat::add_tank(Tank* tank) {
    if (!tank)
        throw NullPointerError("Habitat: Cannot add null tank.");
    if (tank->get_habitat() != this)
        throw OceanariumError("Habitat: Tank " + tank->get_tank_id()
                              + " belongs to a different habitat.");
    if (std::find(tanks.begin(), tanks.end(), tank) != tanks.end())
        return;
    tanks.push_back(tank);
}
