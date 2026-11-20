#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Species.h"

class Animal;

/** @brief Stores selected parent pairs and the breeding schedule.
 * @ingroup animals
 */
class BreedingProgram {
private:
    Species species;
    std::vector<std::pair<Animal*, Animal*>> pairs; // non-owning
    std::string status;

public:
    BreedingProgram(Species sp = Species(), std::string status = "Inactive");

    /**
     * @brief Registers two distinct, eligible animals of the same species.
     * @note Both animals must outlive their registration in this program.
     */
    void select_pair(Animal* parent1, Animal* parent2);
    void schedule_breeding(const std::string& data);
};
