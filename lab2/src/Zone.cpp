#include "Zone.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Animal.h"
#include "AnimalTransfer.h"
#include "Exhibit.h"
#include "Tank.h"
#include "exceptions.h"   // NullPointerError, OceanariumError, DuplicateEntityError

Zone::Zone(std::string name, int access)
    : zone_name(name), access_level(access) {}

void Zone::restrict_access(int new_access_level) {
    if (new_access_level < 0 || new_access_level > 3) {
        throw OceanariumError("Zone: Invalid access level. Must be between 0 and 3.");
    }

    access_level = new_access_level;
    std::cout << "Access level for zone '" << zone_name << "' changed to " << access_level << std::endl;

    // Если уровень доступа 0 (закрыто для всех)
    if (access_level == 0) {
        for (auto* exhibit : exhibits) {
            if (exhibit && exhibit->is_open()) {
                exhibit->close_exhibit();
            }
        }
    }
}

void Zone::list_tanks() const {
    std::cout << "--- Tanks in zone '" << zone_name << "' (Access Level: " << access_level << ") ---" << std::endl;

    for (const auto* exhibit : exhibits) {
        if (exhibit != nullptr) {
            std::cout << "  Exhibit: " << exhibit->get_title() << std::endl;
            const std::vector<Tank*>& tanks_in_exhibit = exhibit->get_tanks();
            for (const auto* tank : tanks_in_exhibit) {
                if (tank != nullptr) {
                    std::cout << "    - Tank " << tank->get_tank_id()
                              << " | Animals: " << tank->get_animal_count()
                              << " | Habitat: " << (tank->get_habitat() ? "Yes" : "None")
                              << std::endl;
                }
            }
        }
    }
    std::cout << "---------------------------------------------------" << std::endl;
}

void Zone::evacuate_tank_animals(Tank* tank, Habitat* backup_habitat) {
    if (!tank || !backup_habitat) return;
    std::vector<Animal*> animals_to_move = tank->get_animals();

    for (Animal* animal : animals_to_move) {
        if (animal != nullptr) {
            try {
                AnimalTransfer transfer(animal, backup_habitat);
                transfer.approve_transfer();
                std::cout << "Animal " << animal->get_id() << " evacuated to backup habitat." << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to evacuate " << animal->get_id() << ": " << e.what() << std::endl;
            }
        }
    }
}

void Zone::add_tank(Tank* tank) {
    if (!tank) throw NullPointerError("Zone: Cannot add null tank.");

    if (exhibits.empty()) {
        throw OceanariumError("Zone: No exhibits available to place the tank in.");
    }

    // добавляем в первый попавшийся зал в зоне
    Exhibit* target_exhibit = exhibits.front();
    target_exhibit->add_tank(tank);

    std::cout << "Tank " << tank->get_tank_id() << " added to zone " << zone_name << std::endl;
}

void Zone::remove_tank(Tank* tank, Habitat* backup_habitat) {
    if (!tank) return;

    Exhibit* owner = tank->get_exhibit();
    if (!owner) return;

    // принадлежит ли этот exhibit нашей зоне?
    bool in_this_zone = false;
    for (Exhibit* ex : exhibits) {
        if (ex == owner) { in_this_zone = true; break; }
    }
    if (!in_this_zone) return;

    // 1) сначала эвакуация — иначе танк может «осиротеть» с животными
    evacuate_tank_animals(tank, backup_habitat);

    if (tank->get_animal_count() > 0) {
        std::cerr << "WARNING: Tank " << tank->get_tank_id()
                  << " still has animals! Cannot remove safely." << std::endl;
        return;   // танк остаётся в зале, состояние не тронуто
    }

    // 2) только теперь физически убираем
    owner->remove_tank(tank);
    tank->clear_state();
    std::cout << "Tank " << tank->get_tank_id()
              << " successfully removed from zone." << std::endl;
}

void Zone::add_exhibit(Exhibit* exhibit) {
    if (!exhibit) {
        throw NullPointerError("Zone: Cannot add null exhibit.");
    }
    if (std::find(exhibits.begin(), exhibits.end(), exhibit) != exhibits.end()) {
        throw DuplicateEntityError("Zone: Exhibit is already in this zone.");
    }
    exhibits.push_back(exhibit);
}
