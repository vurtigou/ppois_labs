#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include "habitats.h"
#include "water.h"


Habitat::Habitat(std::string type, double volume, const WaterQuality& water) 
    : habitat_type(type), volume_liters(volume), water_quality(water) {}

bool Habitat::check_suitability_for_animal(const Animal& animal) const {
        // 1. Получаем нормы для вида этого животного
    std::pair<double, double> ph_norm = animal.get_species().get_ph_range();
    std::pair<double, double> sal_norm = animal.get_species().get_salinity_range();
    std::pair<double, double> temp_norm = animal.get_species().get_temperature_range();
    std::pair<double, double> oxy_norm = animal.get_species().get_oxygen_range();

    // 2. Получаем текущие показатели воды в этой среде обитания
    double current_ph = water_quality.get_ph();
    double current_sal = water_quality.get_salinity();
    double current_temp = water_quality.get_temperature();
    double current_oxy = water_quality.get_oxygen_level();

    // 3. Проверяем каждый показатель (входит ли текущее значение в интервал [min, max])
    bool ph_ok = (current_ph >= ph_norm.first && current_ph <= ph_norm.second);
    bool sal_ok = (current_sal >= sal_norm.first && current_sal <= sal_norm.second);
    bool temp_ok = (current_temp >= temp_norm.first && current_temp <= temp_norm.second);
    bool oxy_ok = (current_oxy >= oxy_norm.first && current_oxy <= oxy_norm.second);

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
            if (removed) removed->move_to_habitat(nullptr);      // потом обнулить среду
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

Exhibit::Exhibit(std::string title, std::string description, bool is_open) : title(title), description(description), opened(is_open) {}

void Exhibit::open_exhibit() {
    if (opened) return;   
    opened = true;
    std::cout << "Exhibit '" << title << "' is now OPEN for visitors." << std::endl;
}

void Exhibit::close_exhibit() {
    if (!opened) return;
    opened = false;
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
    if (std::find(tanks.begin(), tanks.end(), tank) != tanks.end()) return;

    // отвязать от прежнего зала, иначе получим «двойное членство»
    Exhibit* prev = tank->get_exhibit();
    if (prev && prev != this) {
        prev->remove_tank(tank);
    }

    tanks.push_back(tank);
    tank->set_exhibit(this);
}

bool Exhibit::remove_tank(Tank* tank) {
    auto it = std::find(tanks.begin(), tanks.end(), tank);
    if (it == tanks.end()) return false;

    tanks.erase(it);
    // обнуляем привязку только если она всё ещё указывает на нас
    if (tank->get_exhibit() == this) {
        tank->set_exhibit(nullptr);
    }
    return true;
}

Tank::Tank(std::string id, Habitat* parent, std::string filtration) 
    : tank_id(std::move(id)), parent_habitat(parent), filtration_system(std::move(filtration)), 
      exhibit(nullptr), animal_count(0), is_filtration_on(false) {}

void Tank::start_filtration() {
    if (filtration_system.empty())
        throw OceanariumError("Tank " + tank_id + ": No filtration system installed!");
    if (is_filtration_on) {
        std::cout << "Filtration is already running in tank " << tank_id << std::endl;
        return;
    }
    if (parent_habitat == nullptr)
        throw OceanariumError("Tank " + tank_id + ": No parent habitat.");

    is_filtration_on = true;
    try {
        std::cout << "Filtration system started for tank: " << tank_id << std::endl;

        for (ControlSystem* system : sensors) {
            if (!system) continue;

            system->checking();
            if (!system->get_is_good_working()) {
                std::cout << "  Warning: control system " << system->get_id()
                          << " is broken, skipped." << std::endl;
                continue;
            }
            system->improving();
        }

        if (parent_habitat->get_water_quality().is_within_norm())
            std::cout << "Water quality in habitat is now within norm." << std::endl;
        else
            std::cout << "Water quality is still out of norm "
                         "(some systems broken or out of resources)." << std::endl;
    } catch (...) {
        // Гарантируем сброс флага, даже если датчик бросил исключение.
        is_filtration_on = false;
        throw;
    }
    is_filtration_on = false;
}


bool Tank::is_water_critical() const {
    if (parent_habitat == nullptr) return false;
    return !parent_habitat->get_water_quality().is_within_norm();
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
    if (!backup_habitat)
        throw NullPointerError("EmergencyDrain: Backup habitat is null.");

    std::cout << "Checking water quality for tank " << tank_id << "..." << std::endl;
    if (!is_water_critical()) {
        std::cout << "Water is fine. No emergency drain needed." << std::endl;
        return;
    }

    std::cout << "CRITICAL WATER QUALITY! Starting emergency evacuation..." << std::endl;
    evacuate_all_animals(backup_habitat);
    if (animal_count > 0) {
        // Часть животных могла уже уехать в backup; танк остаётся как есть,
        // но не сливаем воду — иначе оставшиеся животные погибнут.
        throw OceanariumError(
            "EmergencyDrain: Could not evacuate all animals. Drain aborted. "
            "Some animals may already have been moved to backup habitat.");
    }

    std::cout << "WARNING! Tank " << tank_id << " has been drained." << std::endl;

    // start_filtration может бросить (например, фильтр не установлен) —
    // танк уже осушен и без животных, не даём исключению «потеряться».
    try {
        start_filtration();
    } catch (const std::exception& e) {
        std::cerr << "Warning: filtration could not be restarted after drain: "
                  << e.what() << std::endl;
    }
}


void Tank::clear_state() {
    for (Animal* a : animals) {
        if (a) a->set_current_tank(nullptr);
    }
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
    if (contains_animal(animal->get_id())) return;   // дубликат — молча игнор
    if (animal_count >= 10)
        throw OceanariumError(
            "Tank " + tank_id + ": capacity exceeded (max 10 animals).");

    animals.push_back(animal);
    animal->set_current_tank(this);
    animal_count++;
}

Animal* Tank::remove_animal(const std::string& animal_id) {
    for (auto it = animals.begin(); it != animals.end(); ++it) {
        if ((*it)->get_id() == animal_id) {
            Animal* removed = *it;
            animals.erase(it);
            if (removed->get_current_tank() == this) removed->set_current_tank(nullptr);
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

QuarantineTank::QuarantineTank(std::string id, Habitat* parent, std::string filtration, std::string reason) 
    : Tank(std::move(id), parent, std::move(filtration)), quarantine_reason(std::move(reason)) {}
    
void QuarantineTank::set_diagnosis(const std::string& diagnosis) {
    quarantine_reason = diagnosis;
}

void QuarantineTank::admit_animal(Animal* animal) {
    if (!animal)
        throw NullPointerError("QuarantineTank: Animal pointer cannot be null.");
    if (contains_animal(animal->get_id())) return; 
    
    if (!animal->can_be_moved_to_habitat()) {
        throw InvalidActionError("QuarantineTank: Animal " + animal->get_id() + " cannot be moved to quarantine.");
    }
    if (get_animal_count() >= 10)
        throw OceanariumError("QuarantineTank: capacity exceeded (max 10).");
    Habitat* current_habitat = animal->get_habitat();
    Tank* current_tank = animal->get_current_tank();

    if (current_habitat != nullptr) {
        current_habitat->remove_animal(animal->get_id());
    } else if (current_tank != nullptr) {
        // Это решает проблему, если животное пытаются засунуть во ВТОРОЙ карантин
        current_tank->remove_animal(animal->get_id());
    }

    add_animal(animal);                 
    animal->move_to_habitat(nullptr); // Обнуляем среду на всякий случай

    std::cout << "Animal " << animal->get_id()
              << " admitted to quarantine. Reason: " << quarantine_reason << std::endl;
}

Animal* QuarantineTank::release_after_check(const std::string& animal_id) {
    Animal* released = remove_animal(animal_id); 
    if (!released) {
        throw EntityNotFoundError(
            "QuarantineTank: Animal " + animal_id + " not found in quarantine tank.");
    }
    
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
