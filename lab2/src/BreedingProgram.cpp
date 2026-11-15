#include "BreedingProgram.h"

#include <iostream>
#include <string>
#include <utility>

#include "Animal.h"
#include "exceptions.h"   // NullPointerError, InvalidActionError, SpeciesMismatchError

BreedingProgram::BreedingProgram(Species sp, std::string status)
    : species(std::move(sp)), status(std::move(status)) {}

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
