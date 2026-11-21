#include "HealthRecord.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "Animal.h"
#include "VetVisit.h"
#include "exceptions.h"   // NullPointerError

HealthRecord::HealthRecord(Animal* a, std::string status)
    : animal(a), current_status(std::move(status)) {}

void HealthRecord::add_visit(std::unique_ptr<VetVisit> visit) {
    if (!visit) {
        throw NullPointerError("HealthRecord: Cannot add a null visit.");
    }
    visits.push_back(std::move(visit)); // Передаем владение
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
