#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "habitats.h"


Habitat::Habitat(std::string type, double volume, const WaterQuality& water) 
    : habitat_type(type), volume_liters(volume), water_quality(water) {}

bool Habitat::check_suitability_for_animal(const Animal& animal) const {
    //TODO: добавить проверку воды и объема для животного
    return true; 
}

double Habitat::calculate_biomass() const {
    double total_biomass = 0.0;
    for (const auto* animal : animals) {
        total_biomass += animal->get_weight(); // TODO: добавть животному const get_weight()
    }
    return total_biomass;
}

void Habitat::add_animal(Animal* animal) {
    if (!animal) return; //TODO выыод класса исключения
    //TODO     Заменить if (!animal) return; на выброс исключения (например, OceanariumError).
    if(!check_suitability_for_animal(*animal)) return;
    //TODO: вывод сообщения
    animals.push_back(animal);
}

bool Habitat::contains_animal(const std::string& animal_id) const {
    for (const auto* animal : animals) {
        // TODO: доавить животному const get_id()
        if (animal->get_id() == animal_id) { 
            return true;
        }
    }
    return false;
}

Animal* Habitat::remove_animal(const std::string& animal_id) {
    for (auto it = animals.begin(); it != animals.end(); ++it) {
        if ((*it)->get_id() == animal_id) {
            Animal* removed_animal = *it;
            animals.erase(it);
            return removed_animal;
        }
    }
    return nullptr;
}

Exhibit::Exhibit(std::string title, std::string description, bool is_open) : title(title), description(description), is_open(is_open) {}

void Exhibit::open_exhibit() {
    if (is_open) return;   
    is_open = true;
    std::cout << "Exhibit '" << title << "' is now OPEN for visitors." << std::endl;
}

void Exhibit::close_exhibit() {
    if(!is_open) return;
    is_open = false;
    std::cout << "Exhibit '" << title << "' is now CLOSED." << std::endl;
}

void Exhibit::add_tank(Tank* tank) {
    if (tank) {
        tanks.push_back(tank);
        tank->set_exhibit(this);
    }
}

Tank::Tank(std::string id, const Habitat& hab, std::string filtration) 
    : tank_id(id), habitat(hab), filtration_system(filtration), exhibit(nullptr) {}

void Tank::start_filtration() {
    std::cout << "Filtration system (" << filtration_system << ") started for tank: " << tank_id << std::endl;
}

void Tank::emergency_drain() {
    std::cout << "WARNING! Emergency draining tank: " << tank_id << std::endl;
}

void Tank::attach_sensor(const std::string& sensor_id) {
    if (std::find(sensors.begin(), sensors.end(), sensor_id) != sensors.end()) {
        std::cout << "Sensor " << sensor_id << " is already attached to tank " << tank_id << std::endl;
        return;
    }
    sensors.push_back(sensor_id);
    std::cout << "Sensor " << sensor_id << " attached to tank " << tank_id << std::endl;
}

// Реализация методов-оберток
void Tank::add_animal(Animal* animal) {
    habitat.add_animal(animal);
}

bool Tank::contains_animal(const std::string& animal_id) const {
    return habitat.contains_animal(animal_id);
}

Animal* Tank::remove_animal(const std::string& animal_id) {
    return habitat.remove_animal(animal_id);
}


Zone::Zone(std::string name, int access) 
    : zone_name(name), access_level(access) {}

void Zone::restrict_access(int new_access_level) {
    access_level = new_access_level;
    std::cout << "Access level for zone '" << zone_name << "' changed to " << access_level << std::endl;
}

void Zone::list_tanks() const {
    std::cout << "Tanks in zone '" << zone_name << "':" << std::endl;
    for (const auto* tank : tanks) {
        std::cout << " - " << tank->get_tank_id() << std::endl;
    }
}

void Zone::add_tank(Tank* tank) {
    if (tank) {
        tanks.push_back(tank);
    }
}

QuarantineTank::QuarantineTank(Tank* t, std::string reason) 
    : tank(t), quarantine_reason(reason) {
    if (!tank) {
        throw std::invalid_argument("QuarantineTank: tank cannot be null");
    }
}

//TODO const get_id()
void QuarantineTank::admit_animal(Animal* animal) {
    if (!animal) return;
    tank->add_animal(animal);
    std::cout << "Animal " << animal->get_id() << " admitted to quarantine. Reason: " << quarantine_reason << std::endl;
}

Animal* QuarantineTank::release_after_check(const std::string& animal_id) {
    Animal* released = tank->remove_animal(animal_id);
    if (released) {
        std::cout << "Quarantine check passed. Animal " << animal_id << " released from tank " << tank->get_tank_id() << std::endl;
    } else {
        std::cout << "Animal " << animal_id << " not found in quarantine tank." << std::endl;
    }
    return released;
}

// ==========================================
// HabitatMap
// ==========================================
HabitatMap::HabitatMap(double scale) : scale(scale) {}

void HabitatMap::add_zone(Zone* zone) {
    if (zone) {
        zones.push_back(zone);
    }
}

Tank* HabitatMap::locate_tank(const std::string& tank_id) const {
    std::cout << "Searching for tank " << tank_id << " on map..." << std::endl;
    return nullptr; // Заглушка
}
