#include <iostream>
#include <cmath>
#include <cstdlib> 
#include <ctime> 
#include "biology.h"
#include "habitats.h"
#include "exceptions.h"

Species::Species(std::string name, std::string diet, double min_vol) : latin_name(name), diet_type(diet), min_tank_volume(min_vol) {}

bool Species::is_compatible_with_species(const Species& other) const {
    bool this_eats_other = (other.diet_type.find(this->latin_name) != std::string::npos);
    bool other_eats_this = (this->diet_type.find(other.latin_name) != std::string::npos);

    return !this_eats_other && !other_eats_this;
}

VetVisit::VetVisit(Animal* a, std::string diag, std::string treat) : animal(a), diagnosis(diag), treatment(treat), is_closed(false) {}

void VetVisit::diagnostic(const std::string& new_diagnosis, QuarantineTank* quarantine_tank) {
    diagnosis = new_diagnosis;

    if (new_diagnosis != "Healthy" && new_diagnosis != "Cured") {
        if (quarantine_tank != nullptr) {
            quarantine_tank->set_diagnosis(new_diagnosis);
        }
        
        prescribe_treatment("Prescribed based on diagnosis: " + new_diagnosis, quarantine_tank);
    } else {
        treatment = ""; 
    }
}

void VetVisit::prescribe_treatment(const std::string& new_treatment, QuarantineTank* quarantine_tank) {
    this->treatment = new_treatment;

    if (quarantine_tank != nullptr && this->animal != nullptr) {
        quarantine_tank->admit_animal(this->animal);
    }
}

void VetVisit::close_visit() {
    if (is_closed) {
	throw InvalidActionError("VetVisit: Visit is already closed.");
    }
    if (diagnosis == "Healthy" || diagnosis == "Cured") {
        is_closed = true;
        std::cout << "Vet visit successfully closed (animal is healthy)." << std::endl;
        
        if (animal) {
            animal->get_health_record().add_visit(this);
        }
        return;
    }

    bool has_treatment = !treatment.empty();
    bool has_diagnosis = !diagnosis.empty();

    if (has_treatment && has_diagnosis) {
        is_closed = true;
        std::cout << "Vet visit successfully closed. Diagnosis: " << diagnosis 
                  << " | Treatment: " << treatment << std::endl;
                  
        if (animal) {
            animal->get_health_record().add_visit(this);
        }
    } else {
        throw OceanariumError("VetVisit: Cannot close visit. Treatment or diagnosis is missing.");
    }
}

HealthRecord::HealthRecord(Animal* a, std::string status) 
    : animal(a), current_status(status) {}
    
void HealthRecord::add_visit(VetVisit* visit) {
    if (!visit) {
        throw NullPointerError("HealthRecord: Cannot add a null visit.");
    }
    visits.push_back(std::unique_ptr<VetVisit>(visit));

    if (animal) {
        animal->on_vet_visit(); 
    }
}

void HealthRecord::status_diagnosis() {
    if (visits.empty()) {
        current_status = "Healthy";
        std::cout << "No visits recorded. Status: " << current_status << std::endl;
        return;
    }

    VetVisit* last_visit = visits.back().get();
    
    if (last_visit != nullptr) {
        current_status = last_visit->get_diagnosis();
        std::cout << "Health record updated. New status: " << current_status << std::endl;
    }
}

FeedingPlan::FeedingPlan(Animal* a, double portion) 
    : animal(a), portion_grams(portion) {}

void FeedingPlan::adjust_portion(double last_weight) {
    if (last_weight > 0.0 && animal != nullptr) {
        double current_weight = animal->get_weight();
        
        if (current_weight > 0.0) {
            double weight_ratio = current_weight / last_weight; 
            portion_grams *= weight_ratio; 
            
            std::cout << "Feeding portion adjusted to: " << portion_grams << " grams." << std::endl;
        }
    } else {
        throw OceanariumError("FeedingPlan: Invalid last weight or animal is null.");
    }
}

void FeedingPlan::add_feed_item(FeedStock* item) {
    if (item != nullptr) {
        feed_items.push_back(item);
    }
}

bool FeedingPlan::check_diet_compliance() const {
    if (!animal) {
        throw NullPointerError("FeedingPlan: Animal is null, cannot check diet.");
    }
    std::string animal_diet = animal->get_species().get_diet_type();
    if (feed_items.empty()) return true; 

    for (const auto* item : feed_items) {
        if (item == nullptr) continue;

        // Получаем название корма как строку (например, "Squid") TODO СДЕЛАТЬ в классе FeedStock метод get_name()
        std::string food_name = item->get_name(); 
        if (animal_diet.find(food_name) == std::string::npos) return false;
    }

    return true;
}

void FeedingPlan::calculate_portion() {
    if (!animal) {
        throw NullPointerError("FeedingPlan: Animal is null, cannot calculate portion.");
    }

    double weight = animal->get_weight();

    if (dynamic_cast<Cub*>(animal) != nullptr) {
        portion_grams = weight * 0.03; // 3% от массы
    } 
    else if (weight > 10.0) {
        portion_grams = weight * 0.10; // 10% от массы
    } 
    else {
        portion_grams = weight * 0.05; // 5% от массы
        std::cout << "Portion calculated: " << portion_grams << " grams (5% of mass)." << std::endl;
    }
}

Animal::Animal(std::string id, Species sp, double weight) 
    : animal_id(id), species(sp), habitat(nullptr), 
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
    if (new_weight < 0) throw OceanariumError("Animal: Weight cannot be negative.");
    
    double old_weight = weight_kg;
    weight_kg = new_weight;
    
    if (old_weight > 0.0) { 
        double change_percent = (std::abs(new_weight - old_weight) / old_weight) * 100.0;

        if (change_percent > 5.0) {
            std::cout << "WARNING: Weight changed by " << change_percent << "%. Creating vet visit..." << std::endl;

            VetVisit* visit = new VetVisit(this);
            try {
                visit->diagnostic("Abnormal weight change", nullptr);
                visit->close_visit();
            } catch (...) {
                delete visit; // Защита от утечки памяти
                throw;
            }
            health_record.status_diagnosis(); 
        }
    }
    feeding_plan.adjust_portion(old_weight);
}

Cub::Cub(const std::string& parent_id, Species sp, double weight)
    // Генерируем ID: ID_родителя + "-Cub-" + случайное число от 1 до 100
    : Animal(parent_id + "-Cub-" + std::to_string(std::rand() % 100 + 1), sp, weight), age(0) {}

void Cub::record_weight_change(double new_weight) {
    double old_weight = get_weight();
    Animal::record_weight_change(new_weight); 

    if (old_weight > 0 && new_weight != old_weight) {
        double change_percent = (std::abs(new_weight - old_weight) / old_weight) * 100.0;
        if (change_percent <= 5.0) {
            std::cout << "Cub weight changed slightly. Creating mandatory vet visit..." << std::endl;
            VetVisit* visit = new VetVisit(this);
            try {
                visit->diagnostic("Routine cub weight check", nullptr);
                visit->close_visit(); 
            } catch (...) {
                delete visit; // Защита от утечки памяти
                throw;
            }
        }
    }
}

void Cub::move_to_habitat(Habitat* new_habitat) {
    throw InvalidActionError("Cub: Cannot be moved to another habitat! It must stay with its parents.");
}

BreedingProgram::BreedingProgram(Species sp, std::string status) 
    : species(sp), status(status) {}

void BreedingProgram::select_pair(Animal* parent1, Animal* parent2) {
    if (!parent1 || !parent2) {
        throw NullPointerError("BreedingProgram: Cannot select a pair with null animals.");
    }

    if (!parent1->can_breed() || !parent2->can_breed()) {
        throw InvalidActionError("BreedingProgram: One or both animals are cubs and cannot breed.");
    }

    std::string species1 = parent1->get_species().get_latin_name();
    std::string species2 = parent2->get_species().get_latin_name();
    if (species1 != species2) {
        throw SpeciesMismatchError("BreedingProgram: Cannot breed animals of different species (" + species1 + " and " + species2 + ").");
    }

    pairs.push_back({parent1, parent2});
    std::cout << "Pair selected for breeding. Species: " << species1 << std::endl;
}

void BreedingProgram::schedule_breeding(const std::string& data) {
    status = "Active. Date: " + data;
    std::cout << "Breeding scheduled for: " << data << std::endl;
}

AnimalTransfer::AnimalTransfer(Animal* a, Habitat* dest) 
    : animal(a), destination_habitat(dest) {}

void AnimalTransfer::approve_transfer() {
    if (!animal || !destination_habitat) {
        throw NullPointerError("AnimalTransfer: Animal or destination habitat is null.");
    }

    Habitat* old_habitat = animal->get_habitat();
    if (!old_habitat) {
        throw OceanariumError("AnimalTransfer: Animal has no current habitat.");
    }

    if (old_habitat == destination_habitat) {
        throw InvalidActionError("AnimalTransfer: Animal is already in this habitat.");
    }

    if (!destination_habitat->check_suitability_for_animal(*animal)) {
        throw UnsuitableHabitatError("AnimalTransfer: Destination habitat is not suitable for animal " + animal->get_id());
    }

    Animal* removed = old_habitat->remove_animal(animal->get_id());
    if (!removed) {
        throw EntityNotFoundError("AnimalTransfer: Animal not found in its current habitat.");
    }
    // Используем try-catch на случай, если вдруг в новом танке не окажется места
    try {
        destination_habitat->add_animal(animal);
    } catch (...) {
        // Откат изменений: если добавить не удалось, возвращаем животное обратно в старую среду
        old_habitat->add_animal(animal);
        throw; 
    }

    std::cout << "Transfer approved. Animal " << animal->get_id() 
              << " successfully moved to new habitat." << std::endl;
}
