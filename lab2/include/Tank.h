#pragma once

#include <string>
#include <vector>

class Animal;
class ControlSystem;
class Exhibit;
class Habitat;

/** @brief Houses animals and coordinates filtration and emergency evacuation.
 * @ingroup habitats
 */
class Tank {
private:
    std::string tank_id;
    Habitat* parent_habitat;
    std::string filtration_system;
    std::vector<ControlSystem*> sensors;
    std::vector<Animal*> animals;
    Exhibit* exhibit;
    int animal_count;
    bool is_filtration_on;

public:
    Tank(std::string id = "", Habitat* parent = nullptr, std::string filtration = "");
    virtual ~Tank() = default;

    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;

    // Проверяет is_filtration_on, если false, делает true.
    // Берёт water quality из parent_habitat и улучшает показатели.
    /**
     * @brief Runs operational controllers against the parent habitat’s water.
     * @throws OceanariumError The tank lacks filtration equipment or a parent habitat.
     */
    void start_filtration();
    // Проверяет качество воды. Если показатели сильно ниже нормы — создаёт
    // AnimalTransfer-заявки для животных и переносит их в backup_habitat.
    // Вызывает start_filtration.
    /**
     * @brief Attempts to evacuate the tank and restart filtration when water is critical.
     * @throws OceanariumError Animals remain after evacuation. Animals already moved stay in the backup habitat.
     */
    void emergency_drain(Habitat* backup_habitat);
    bool is_water_critical() const;
    void evacuate_all_animals(Habitat* backup_habitat);
    void clear_state();

    void set_exhibit(Exhibit* ex) { exhibit = ex; }
    std::string const& get_tank_id() const { return tank_id; }
    Habitat* get_habitat() const { return parent_habitat; }
    const std::vector<Animal*>& get_animals() const { return animals; }
    Exhibit* get_exhibit() const { return exhibit; }
    int get_animal_count() const { return animal_count; }

    void add_animal(Animal* animal);
    void add_sensor(ControlSystem* s) { if (s) sensors.push_back(s); }
    bool contains_animal(const std::string& animal_id) const;
    Animal* remove_animal(const std::string& animal_id);
};
