#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Species.h"

class Animal;

class BreedingProgram {
private:
    Species species;
    std::vector<std::pair<Animal*, Animal*>> pairs; // non-owning
    std::string status;

public:
    BreedingProgram(Species sp = Species(), std::string status = "Inactive");

    void select_pair(Animal* parent1, Animal* parent2);
    void schedule_breeding(const std::string& data);
};
