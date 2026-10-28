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
    if (!animal) {
        throw NullPointerError("Habitat: Cannot add null animal.");
    }

    Tank* target_tank = nullptr;
    for (auto* tank : tanks) {
        if (tank != nullptr && tank->get_animal_count() < 10) {
            target_tank = tank;
            break; 
        }
    }

    if (target_tank == nullptr) {
        throw OceanariumError("Habitat: All tanks are full (max 10 animals per tank).");
    }
    animal->move_to_habitat(this);
    target_tank->add_animal(animal);

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
        if (tank != nullptr && tank->contains_animal(animal_id)) {
            Animal* target = nullptr; 
            for (auto* a : tank->get_animals()) {
                if (a->get_id() == animal_id) { 
                    target = a; 
                    break; 
                }
            }
            
            if (target != nullptr) {
                target->move_to_habitat(nullptr);
                return tank->remove_animal(animal_id);
            }
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

std::vector<Animal*> Exhibit::get_all_animals() const {
    std::vector<Animal*> all_animals;
    for (const auto* tank : tanks) {
        if (tank != nullptr) {
            const std::vector<Animal*>& tank_animals = tank->get_animals();
            // Добавляем всех животных из этого танка в общий список. Метод insert вставляет элементы из диапазона [begin, end) в конец вектора.
            all_animals.insert(all_animals.end(), tank_animals.begin(), tank_animals.end());
        }
    }

    return all_animals;
}

void Exhibit::add_tank(Tank* tank) {
    if (!tank) throw NullPointerError("Exhibit: Cannot add null tank.");
    // Проверка на дубликат
    if (std::find(tanks.begin(), tanks.end(), tank) != tanks.end()) return;
    
    tanks.push_back(tank);
    tank->set_exhibit(this); // Привязываем танк к залу
}

bool Exhibit::remove_tank(Tank* tank) {
    auto it = std::find(tanks.begin(), tanks.end(), tank);
    if (it != tanks.end()) {
        tanks.erase(it);
        tank->set_exhibit(nullptr); // Отвязываем танк от зала
        return true;
    }
    return false;
}

Tank::Tank(std::string id, Habitat* parent, std::string filtration) 
    : tank_id(id), parent_habitat(parent), filtration_system(filtration), 
      exhibit(nullptr), animal_count(0), is_filtration_on(false) {}

void Tank::start_filtration() {
    if (filtration_system.empty()) {
        throw OceanariumError("Tank " + tank_id + ": No filtration system installed!");
    }
    if (is_filtration_on) {
        std::cout << "Filtration is already running in tank " << tank_id << std::endl;
        return;
    }

    is_filtration_on = true;
    std::cout << "Filtration system started for tank: " << tank_id << std::endl;
    if (parent_habitat != nullptr) {
        // TODO: Для этого в Habitat нужно добавить метод improve_water_quality()
        // parent_habitat->improve_water_quality(15.0); // например, +15% к чистоте
        std::cout << "Water quality in habitat improved. Filtration system ended." << std::endl;
    }
    is_filtration_on = false;
}

bool Tank::is_water_critical() const {
    if (parent_habitat == nullptr) return false;
    
    // TODO: Здесь нужно проверить параметры воды (кислород, температура, pH).
    // Например: return parent_habitat->get_water_quality().get_oxygen() < 2.0;
    
    // Пока заглушка для демонстрации логики
    return true; // Допустим, вода критическая
}

void Tank::evacuate_all_animals(Habitat* backup_habitat) {
    std::vector<Animal*> animals_to_move = animals; 

    for (Animal* animal : animals_to_move) {
        if (animal != nullptr) {
            try {
                AnimalTransfer transfer(animal, backup_habitat);
                transfer.approve_transfer(); 
                
                std::cout << "Animal " << animal->get_id() << " evacuated from tank " << tank_id << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to evacuate " << animal->get_id() << ": " << e.what() << std::endl;
            }
        }
    }
}

void Tank::emergency_drain(Habitat* backup_habitat) {
    if (!backup_habitat) {
        throw NullPointerError("EmergencyDrain: Backup habitat is null.");
    }
    std::cout << "Checking water quality for tank " << tank_id << "..." << std::endl;
    if (!is_water_critical()) {
        std::cout << "Water is fine. No emergency drain needed." << std::endl;
        return;
    }
    std::cout << "CRITICAL WATER QUALITY! Starting emergency evacuation..." << std::endl;
    evacuate_all_animals(backup_habitat);
    if (animal_count > 0) {
        throw OceanariumError("EmergencyDrain: Could not evacuate all animals. Drain aborted.");
    }

    // TODO Сливаем воду (или обнуляем ее)
    // parent_habitat->set_water_quality(WaterQuality(0, 0, 0));
    std::cout << "WARNING! Tank " << tank_id << " has been drained." << std::endl;
    start_filtration();
}

void Tank::clear_state() {
    animals.clear();       // Удаляем всех животных из вектора
    animal_count = 0;      // Сбрасываем счетчик
    sensors.clear();       // Удаляем датчики
    exhibit = nullptr;     // Отвязываем от зала
    is_filtration_on = false; // Выключаем фильтрацию
    // ID и parent_habitat не трогаем, чтобы сохранить идентичность танка
    std::cout << "Tank " << tank_id << " state has been cleared." << std::endl;
}

bool Tank::contains_animal(const std::string& animal_id) const {
    for (const auto* animal : animals) {
        if (animal != nullptr) {
            if (animal->get_id() == animal_id) {
                return true;
            }
        }
    }
    return false;
}

void Tank::add_animal(Animal* animal) {
    if (!animal) return;
    if (contains_animal(animal->get_id())) return;
    
    animals.push_back(animal); 
    animal_count++;
}

Animal* Tank::remove_animal(const std::string& animal_id) {
    for (auto it = animals.begin(); it != animals.end(); ++it) {
        if ((*it)->get_id() == animal_id) {
            Animal* removed = *it;
            animals.erase(it);
            animal_count--;
            return removed;
        }
    }
    return nullptr;
}

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
    
    //добавляем в первый попавшийся зал в зоне
    Exhibit* target_exhibit = exhibits.front(); 
    target_exhibit->add_tank(tank);
    
    std::cout << "Tank " << tank->get_tank_id() << " added to zone " << zone_name << std::endl;
}

void Zone::remove_tank(Tank* tank, Habitat* backup_habitat) {
    if (!tank) return;

    bool found = false;
    for (Exhibit* ex : exhibits) {
        if (ex && ex->remove_tank(tank)) {
            found = true;
            break;
        }
    }

    if (!found) return;
    
    evacuate_tank_animals(tank, backup_habitat);
    
    //проверка на детеныша (его нельзя эвакуировать)
    if (tank->get_animal_count() > 0) {
        std::cerr << "WARNING: Tank " << tank->get_tank_id() 
                  << " still has animals! Cannot clear state safely." << std::endl;
        return; 
    }

    tank->clear_state();
    std::cout << "Tank " << tank->get_tank_id() << " successfully removed from zone." << std::endl;
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

QuarantineTank::QuarantineTank(std::string id, Habitat* parent, std::string filtration, std::string reason) 
    : Tank(id, parent, filtration), quarantine_reason(reason) {}
    
void QuarantineTank::set_diagnosis(const std::string& diagnosis) {
    quarantine_reason = diagnosis;
}

void QuarantineTank::admit_animal(Animal* animal) {
    if (!animal) {
        throw NullPointerError("QuarantineTank: Animal pointer cannot be null.");
    }
    Habitat* current_habitat = animal->get_habitat();
    if (current_habitat != nullptr) {
        current_habitat->remove_animal(animal->get_id());
    }
    add_animal(animal);
    animal->move_to_habitat(nullptr);

    std::cout << "Animal " << animal->get_id() 
              << " admitted to quarantine. Reason: " << quarantine_reason << std::endl;
}

Animal* QuarantineTank::release_after_check(const std::string& animal_id) {
    Animal* released = remove_animal(animal_id); 
    if (!released) {
        throw EntityNotFoundError(
            "QuarantineTank: Animal " + animal_id + " not found in quarantine tank.");
    }
    // Пусть вызывающий код сам решит, куда посадить животное:
    //   Animal* a = qtank.release_after_check("A-1");
    //   some_habitat->add_animal(a);  // <- вот здесь habitat установится
    
    std::cout << "Quarantine check passed. Animal " << animal_id 
              << " released from tank " << get_tank_id() << std::endl;
    return released;
}

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
            for (const auto* tank : exhibit->get_tanks()) {
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
                    
                    return const_cast<Tank*>(tank); 
                }
            }
        }
    }
    
    std::cout << "Tank '" << tank_id << "' was not found on the map." << std::endl;
    return nullptr;
}
