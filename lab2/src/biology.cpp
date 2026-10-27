#include <iostream>
#include "biology.h"
#include "habitats.h"

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
        std::cout << "Visit is already closed." << std::endl;
        return;
    }

    if (diagnosis == "Healthy" || diagnosis == "Cured") {
        is_closed = true;
        std::cout << "Vet visit successfully closed (animal is healthy)." << std::endl;
        return;
    }

    bool has_treatment = !treatment.empty();
    bool has_diagnosis = !diagnosis.empty();

    if (has_treatment && has_diagnosis) {
        is_closed = true;
        std::cout << "Vet visit successfully closed. Diagnosis: " << diagnosis 
                  << " | Treatment: " << treatment << std::endl;
    } else {
        std::cout << "Cannot close visit: ";
        if (!has_treatment) std::cout << "Treatment is not prescribed. ";
        if (!has_diagnosis) std::cout << "Diagnosis is missing. ";
        std::cout << std::endl;
    }
}
