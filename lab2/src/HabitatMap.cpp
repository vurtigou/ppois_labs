#include "HabitatMap.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Animal.h"
#include "Exhibit.h"
#include "Habitat.h"
#include "Tank.h"
#include "Zone.h"
#include "exceptions.h"   // NullPointerError, DuplicateEntityError

HabitatMap::HabitatMap(double scale) : scale(scale) {}

void HabitatMap::add_zone(Zone* zone) {
    if (!zone) {
        throw NullPointerError("HabitatMap: Cannot add null zone.");
    }
    if (std::find(zones.begin(), zones.end(), zone) != zones.end()) {
        throw DuplicateEntityError("HabitatMap: Zone '" + zone->get_zone_name() + "' is already on the map.");
    }
    zones.push_back(zone);
    std::cout << "Zone '" << zone->get_zone_name() << "' successfully added to the map." << std::endl;
}

Tank* HabitatMap::locate_tank(const std::string& tank_id) const {
    std::cout << "Searching for tank '" << tank_id << "' on map..." << std::endl;

    // 1. Проходим по всем зонам
    for (const auto* zone : zones) {
        if (!zone) continue;

        // 2. Проходим по всем выставочным залам в зоне
        for (const auto* exhibit : zone->get_exhibits()) {
            if (!exhibit) continue;

            // 3. Проходим по всем танкам в зале
            for (auto* tank : exhibit->get_tanks()) {
                if (tank && tank->get_tank_id() == tank_id) {
                    // Выводим всю информацию
                    std::cout << "========================================" << std::endl;
                    std::cout << "Tank found: " << tank_id << std::endl;
                    std::cout << "Zone: " << zone->get_zone_name() << std::endl;
                    std::cout << "Exhibit: " << exhibit->get_title() << std::endl;

                    if (tank->get_habitat()) {
                        std::cout << "Habitat type: " << tank->get_habitat()->get_habitat_type() << std::endl;
                    } else {
                        std::cout << "Habitat type: None (isolated)" << std::endl;
                    }

                    std::cout << "Animal count: " << tank->get_animal_count() << std::endl;
                    std::cout << "Animals: ";
                    if (tank->get_animal_count() == 0) {
                        std::cout << "None";
                    } else {
                        for (const auto* animal : tank->get_animals()) {
                            if (animal) {
                                std::cout << animal->get_id() << " ";
                            }
                        }
                    }
                    std::cout << "\n========================================" << std::endl;

                    return tank;
                }
            }
        }
    }

    std::cout << "Tank '" << tank_id << "' was not found on the map." << std::endl;
    return nullptr;
}
