#pragma once

#include <string>
#include <vector>

#include "water.h"   // Для класса WaterQuality
#include "biology.h" // Для класса Animal
#include "exceptions.h"

class Tank;

// среда обитания
class Habitat {
private:
    std::string habitat_type;
    double volume_liters;
    
    WaterQuality water_quality;
    std::vector<Animal*> animals; // non-owning

public:
    Habitat(std::string type = "", double volume = 0.0, const WaterQuality& water = WaterQuality());
    
    //проверка подходит ли среда для животного
    bool check_suitability_for_animal(const Animal& animal) const;
    //сумма весов всех содержащихся животных
    double calculate_biomass() const;
    
    // Методы для работы с животными
    void add_animal(Animal* animal); //добавить в начале check_suitability_for_animal
    
    //проверка есть ли животное с такими id
    bool contains_animal(const std::string& animal_id) const;
    Animal* remove_animal(const std::string& animal_id);
    
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
    
    void add_tank(Tank* tank);
    bool is_open() const { return is_open; }
};

// 3. Класс Tank
class Tank {
private:
    std::string tank_id;
    Habitat habitat;
    std::string filtration_system;
    std::vector<std::string> sensors;
    Exhibit* exhibit; //non-owning

public:
    Tank(std::string id = "", const Habitat& hab = Habitat(), std::string filtration = "");
    
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;

    void start_filtration();
    void emergency_drain();
    void attach_sensor(const std::string& sensor_id);
    
    void set_exhibit(Exhibit* ex) { exhibit = ex; }
    std::string get_tank_id() const { return tank_id; }
    
    const Habitat& get_habitat() const { return habitat; }
    Exhibit* get_exhibit() const { return exhibit; }

    // Методы-обертки для работы с животными (делегируют вызовы в habitat)
    void add_animal(Animal* animal);
    bool contains_animal(const std::string& animal_id) const;
    Animal* remove_animal(const std::string& animal_id);
};

// 4. Класс Zone
class Zone {
private:
    std::string zone_name;
    std::vector<Tank*> tanks; //non-owning
    int access_level;

public:
    Zone(std::string name = "", int access = 0);
    
    void restrict_access(int new_access_level);
    void list_tanks() const;
    
    void add_tank(Tank* tank);
};

// 5. Класс QuarantineTank
class QuarantineTank {
private:
    Tank* tank; //non-owning
    std::string quarantine_reason;

public:
    QuarantineTank(Tank* t = nullptr, std::string reason = "");
    
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
