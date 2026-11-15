#pragma once

#include <string>

#include "Tank.h"   // наследование требует полного типа

class Animal;

class QuarantineTank : public Tank {
private:
    std::string quarantine_reason;

public:
    QuarantineTank(std::string id = "", Habitat* parent = nullptr,
                   std::string filtration = "", std::string reason = "");

    void set_diagnosis(const std::string& diagnosis);
    void admit_animal(Animal* animal);
    Animal* release_after_check(const std::string& animal_id);
};
