#pragma once

#include <string>

#include "Tank.h"   // наследование требует полного типа

class Animal;

/** @brief Holds animals under observation and releases them after a check.
 * @ingroup habitats
 */
class QuarantineTank : public Tank {
private:
    std::string quarantine_reason;

public:
    QuarantineTank(std::string id = "", Habitat* parent = nullptr,
                   std::string filtration = "", std::string reason = "");

    void set_diagnosis(const std::string& diagnosis);
    /**
     * @brief Moves the animal into quarantine when movement and tank capacity allow it.
     */
    void admit_animal(Animal* animal);
    /**
     * @brief Removes the animal from quarantine and returns a non-owning pointer.
     * @throws EntityNotFoundError The ID is not registered in this quarantine tank.
     */
    Animal* release_after_check(const std::string& animal_id);
    /**
     * @brief Releases a quarantined animal directly into the destination habitat.
     * @details If the destination rejects the animal during admission checks, it remains in quarantine.
     */
    Animal* release_after_check(const std::string& animal_id, Habitat& destination);
};
