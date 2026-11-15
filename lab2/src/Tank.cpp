#include "Tank.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Animal.h"
#include "AnimalTransfer.h"
#include "Habitat.h"
#include "exceptions.h"   // NullPointerError, OceanariumError
#include "ControlSystem.h" 
#include "Exhibit.h"       

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
    animals.clear();          // Удаляем всех животных из вектора
    animal_count = 0;         // Сбрасываем счетчик
    sensors.clear();          // Удаляем датчики
    exhibit = nullptr;        // Отвязываем от зала
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
