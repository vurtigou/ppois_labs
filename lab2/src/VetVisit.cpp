#include "VetVisit.h"

#include <iostream>
#include <string>

#include "Animal.h"       // Animal* передаётся в QuarantineTank::admit_animal
#include "exceptions.h"   // InvalidActionError, OceanariumError
#include "QuarantineTank.h"     // QuarantineTank

VetVisit::VetVisit(Animal* a, std::string diag, std::string treat)
    : animal(a), diagnosis(diag), treatment(treat), is_closed(false) {}

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
        return;
    }

    bool has_treatment = !treatment.empty();
    bool has_diagnosis = !diagnosis.empty();

    if (has_treatment && has_diagnosis) {
        is_closed = true;
        std::cout << "Vet visit successfully closed. Diagnosis: " << diagnosis
                  << " | Treatment: " << treatment << std::endl;
    } else {
        throw OceanariumError("VetVisit: Cannot close visit. Treatment or diagnosis is missing.");
    }
}
