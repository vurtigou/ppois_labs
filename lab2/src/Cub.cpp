#include "Cub.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "VetVisit.h"
#include "exceptions.h"   // InvalidActionError

Cub::Cub(const std::string& parent_id, Species sp, double weight)
    // Генерируем ID: ID_родителя + "-Cub-" + случайное число от 1 до 100
    : Animal(parent_id + "-Cub-" + std::to_string(std::rand() % 100 + 1), std::move(sp), weight), age(0) {}

void Cub::record_weight_change(double new_weight) {
    double old_weight = get_weight();
    Animal::record_weight_change(new_weight);

    if (old_weight > 0 && new_weight != old_weight) {
        double change_percent = (std::abs(new_weight - old_weight) / old_weight) * 100.0;
        if (change_percent <= 5.0) {
            std::cout << "Cub weight changed slightly. Creating mandatory vet visit..." << std::endl;
            auto visit = std::make_unique<VetVisit>(this);
            visit->diagnostic("Routine cub weight check", nullptr);
            visit->close_visit();
            get_health_record().add_visit(std::move(visit));
            get_health_record().status_diagnosis();
        }
    }
}

void Cub::move_to_habitat(Habitat* new_habitat) {
    if (new_habitat && get_habitat() && new_habitat != get_habitat()) {
        throw InvalidActionError("Cub: Cannot be moved to another habitat! It must stay with its parents.");
    }
    // Покидание среды разрешено (для очистки / удаления)
    Animal::move_to_habitat(new_habitat);
}
