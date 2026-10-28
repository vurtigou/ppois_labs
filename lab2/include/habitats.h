#pragma once

#include <string>
#include <vector>

#include "water.h"   // Для класса WaterQuality
#include "biology.h" // Для класса Animal
#include "exceptions.h"

class Tank;
class Zone;

// среда обитания
class Habitat {
private:
    std::string habitat_type;
    double volume_liters;
    WaterQuality water_quality;
    std::vector<Tank*> tanks; // non-owning

public:
    Habitat(std::string type = "", double volume = 0.0, const WaterQuality& water = WaterQuality());
    
    //проверка подходит ли среда для животного
    bool check_suitability_for_animal(const Animal& animal) const;
    double calculate_biomass() const;
    
    Animal* add_animal(Animal* animal);
    //проверка есть ли животное с такими id
    bool contains_animal(const std::string& animal_id) const;
    Animal* remove_animal(const std::string& animal_id);
    void add_tank(Tank* tank) { if(tank) tanks.push_back(tank); }
    
    // Геттеры
    const WaterQuality& get_water_quality() const { return water_quality; }
    std::string get_habitat_type() const { return habitat_type; }
};

// выстовочный зал
class Exhibit {
private:
    std::string title;
    std::string description;
    std::vector<Tank*> tanks; //non-owning
    bool is_open;

public:
    Exhibit(std::string title = "", std::string description = "", bool is_open = false);
    
    void open_exhibit();
    void close_exhibit();
    
    // Функция, которая собирает всех животных из всех танков зала
    std::vector<Animal*> get_all_animals() const; 
    
    void add_tank(Tank* tank);
    bool remove_tank(Tank* tank);
    bool is_open() const { return is_open; }
    std::string get_title() const { return title; }
    const std::vector<Tank*>& get_tanks() const { return tanks; }
};

// 3. Класс Tank
class Tank {
private:
    std::string tank_id;
    Habitat* parent_habitat;
    std::string filtration_system;
    std::vector<std::string> sensors;
    std::vector<Animal*> animals;
    Exhibit* exhibit;
    int animal_count;
    bool is_filtration_on;

public:
    Tank(std::string id = "", Habitat* parent = nullptr, std::string filtration = "");
    virtual ~Tank() = default;
    
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;
    //проверяет is_filtration_on, если false, делает true. Берет поле water quality из parent_habitat и улучшает какието его качества
    void start_filtration();
    //проверяет качество воды (как в прошлой функции). если показатели сильно ниже нормы, создает столько AnimalTransfer объектов, сколько animal_count и с помощью этих объектов (заявок на перенос животных в другие среды и танки) переносит их. вызывает start_filtration. 
    void emergency_drain(Habitat* backup_habitat); 
    bool is_water_critical() const;
    void evacuate_all_animals(Habitat* backup_habitat);
    void clear_state(); 
    
    void set_exhibit(Exhibit* ex) { exhibit = ex; }
    std::string get_tank_id() const { return tank_id; }
    Habitat* get_habitat() const { return parent_habitat; }
    const std::vector<Animal*>& get_animals() const { return animals; }
    Exhibit* get_exhibit() const { return exhibit; }
    int get_animal_count() const { return animal_count; }

    void add_animal(Animal* animal);
    bool contains_animal(const std::string& animal_id) const;
    Animal* remove_animal(const std::string& animal_id);
};

// 4. Класс Zone
class Zone {
private:
    std::string zone_name;
    std::vector<Exhibit*> exhibits; //non-owning
    int access_level; //0-закрыто для всех, 1-открыто для сотрудников_1, 2-открыто для сотрудников_2, 3-открыто для всех

public:
    Zone(std::string name = "", int access = 0);
    
    void restrict_access(int new_access_level);
    void list_tanks() const;

    void add_tank(Tank* tank);
    void remove_tank(Tank* tank, Habitat* backup_habitat); 
    void evacuate_tank_animals(Tank* tank, Habitat* backup_habitat);

    void add_exhibit(Exhibit* exhibit); 
    std::string get_zone_name() const { return zone_name; }
    const std::vector<Exhibit*>& get_exhibits() const { return exhibits; }
};

// 5. Класс QuarantineTank
class QuarantineTank : public Tank {
private:
    std::string quarantine_reason;
public:
    QuarantineTank(std::string id = "", Habitat* parent = nullptr, std::string filtration = "", std::string reason = "");

    void set_diagnosis(const std::string& diagnosis);
    void admit_animal(Animal* animal);
    Animal* release_after_check(const std::string& animal_id);
};
// 6. Класс HabitatMap
class HabitatMap {
private:
    std::vector<Zone*> zones; //non-owning
    double scale;

public:
    HabitatMap(double scale = 1.0);
    
    Tank* locate_tank(const std::string& tank_id) const;
    void add_zone(Zone* zone);
};

