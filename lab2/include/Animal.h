#pragma once

#include <string>

#include "FeedingPlan.h"
#include "HealthRecord.h"
#include "Species.h"

class Habitat;
class Tank;

/** @brief Animal with a species profile, weight, feeding plan, and health history.
 * @ingroup animals
 */
class Animal {
private:
    std::string animal_id;
    Species species;            // Ассоциация 15 (композиция)
    Habitat* habitat;           // Ассоциация 16 (агрегация, non-owning)
    Tank* current_tank;
    HealthRecord health_record; // Ассоциация 17 (композиция)
    FeedingPlan feeding_plan;   // Ассоциация 18 (композиция)
    double weight_kg;

public:
    Animal(std::string id = "", Species sp = Species(), double weight = 0.0);

    virtual ~Animal() = default;
    virtual bool can_be_moved_to_habitat() const { return true; }

    // запрет копирования (из-за указателей на самого себя внутри)
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;

    void set_id(const std::string& new_id);
    Tank* get_current_tank() const { return current_tank; }
    void set_current_tank(Tank* tank) { current_tank = tank; }
    /**
     * @brief Records mass in kilograms and adjusts the current feeding plan.
     * @throws InvalidActionError The weight is negative or non-finite.
     */
    virtual void record_weight_change(double new_weight);
    virtual void move_to_habitat(Habitat* new_habitat);
    /**
     * @brief Copies a feeding plan and binds the copy to this animal.
     */
    void assign_feeding_plan(const FeedingPlan& plan);

    virtual bool can_breed() const { return true; } // Обычные животные могут размножаться
    virtual void on_vet_visit() {}                  // Реакция на визит (для Cub - увеличение возраста)

    FeedingPlan& get_feeding_plan() { return feeding_plan; }
    const FeedingPlan& get_feeding_plan() const { return feeding_plan; }
    std::string const get_id() const { return animal_id; }
    double get_weight() const { return weight_kg; }
    Habitat* get_habitat() const { return habitat; }
    const Species& get_species() const { return species; }
    HealthRecord& get_health_record() { return health_record; }
};
