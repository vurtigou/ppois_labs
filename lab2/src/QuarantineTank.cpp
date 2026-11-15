#include "QuarantineTank.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

#include "Animal.h"
#include "Habitat.h"
#include "exceptions.h"   // NullPointerError, InvalidActionError,
                          // OceanariumError, EntityNotFoundError

QuarantineTank::QuarantineTank(std::string id, Habitat* parent,
                               std::string filtration, std::string reason)
    : Tank(std::move(id), parent, std::move(filtration)),
      quarantine_reason(std::move(reason)) {}

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

Animal* QuarantineTank::release_after_check(const std::string& animal_id, Habitat& destination) {
    const auto& animals = get_animals();
    const auto found = std::find_if(animals.begin(), animals.end(), [&](const Animal* animal) {
        return animal->get_id() == animal_id;
    });
    if (found == animals.end()) {
        throw EntityNotFoundError("QuarantineTank: Animal " + animal_id + " not found in quarantine tank.");
    }
    return destination.add_animal(*found);
}
