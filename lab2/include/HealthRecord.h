#pragma once

#include <memory>
#include <string>
#include <vector>

#include "VetVisit.h"

class Animal;

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
    void add_visit(std::unique_ptr<VetVisit> visit);
};
