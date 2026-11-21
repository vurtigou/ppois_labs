#include "Animal.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "VetVisit.h"
#include "exceptions.h"   // InvalidActionError

Animal::Animal(std::string id, Species sp, double weight)
    : animal_id(std::move(id)), species(std::move(sp)), habitat(nullptr),
      current_tank(nullptr),
      health_record(this), feeding_plan(this), weight_kg(weight) {}

void Animal::set_id(const std::string& new_id) {
    if (animal_id.empty()) {
        animal_id = new_id;
        std::cout << "Animal ID successfully assigned: " << animal_id << std::endl;
    } else {
        throw InvalidActionError("Animal: Animal already has an ID (" + animal_id + ").");
    }
}

void Animal::move_to_habitat(Habitat* new_habitat) {
    // Если передали nullptr, значит животное просто покинуло среду обитания
    if (new_habitat == nullptr) {
        habitat = nullptr;
        std::cout << "Animal " << animal_id << " has left its habitat." << std::endl;
        return;
    }
    habitat = new_habitat;
    std::cout << "Animal " << animal_id << " moved to a new habitat." << std::endl;
}

void Animal::assign_feeding_plan(const FeedingPlan& plan) {
    feeding_plan = plan;
    feeding_plan.set_animal(this);
}

void Animal::record_weight_change(double new_weight) {
    if (!std::isfinite(new_weight) || new_weight < 0) {
        throw InvalidActionError("Animal: Weight must be finite and nonnegative.");
    }

    double old_weight = weight_kg;
    weight_kg = new_weight;

    if (old_weight > 0.0) {
        double change_percent = (std::abs(new_weight - old_weight) / old_weight) * 100.0;

        if (change_percent > 5.0) {
            std::cout << "WARNING: Weight changed by " << change_percent << "%. Creating vet visit..." << std::endl;
            auto visit = std::make_unique<VetVisit>(this);
            visit->diagnostic("Abnormal weight change", nullptr);
            visit->close_visit();

            health_record.add_visit(std::move(visit));
            health_record.status_diagnosis();
        }

        feeding_plan.adjust_portion(old_weight);
    }
}
