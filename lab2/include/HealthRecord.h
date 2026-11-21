#pragma once

#include <memory>
#include <string>
#include <vector>

#include "VetVisit.h"

class Animal;

/** @brief Owns an animal’s veterinary visits and derives its current status.
 * @ingroup animals
 */
class HealthRecord {
private:
    Animal* animal; // non-owning
    std::vector<std::unique_ptr<VetVisit>> visits;
    std::string current_status;

public:
    HealthRecord(Animal* a = nullptr, std::string status = "Healthy");
    HealthRecord(const HealthRecord&) = delete;
    HealthRecord& operator=(const HealthRecord&) = delete;
    void status_diagnosis();
    /**
     * @brief Takes ownership of a veterinary visit and notifies the animal.
     * @throws NullPointerError The visit pointer is null.
     */
    void add_visit(std::unique_ptr<VetVisit> visit);
};
