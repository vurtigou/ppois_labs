#pragma once

#include <string>
#include <vector>
#include <utility> 

// Предварительные объявления классов из других пакетов
class Habitat;
class Tank;
class FeedStock; 
class QuarantineTank; 

// Предварительные объявления для классов внутри этого файла
class Animal;
class VetVisit;

class Species {
private:
    std::string latin_name;
    std::string diet_type;
    double min_tank_volume;

public:
    Species(std::string name = "", std::string diet = "", double min_vol = 0.0);
    
    bool is_compatible_with_species(const Species& other) const;
    
    std::string get_latin_name() const { return latin_name; }
    std::string get_diet_type() const { return diet_type; } 
};

class VetVisit {
private:
    Animal* animal; // non-owning
    std::string diagnosis;
    std::string treatment;
    bool is_closed;

public:
    VetVisit(Animal* a = nullptr, std::string diag = "", std::string treat = "");
    
        void diagnostic(const std::string& new_diagnosis, QuarantineTank* quarantine_tank = nullptr);
    void prescribe_treatment(const std::string& new_treatment, QuarantineTank* quarantine_tank = nullptr);
    void close_visit();
    
    std::string get_diagnosis() const { return diagnosis; }
    std::string get_treatment() const { return treatment; }
};

class HealthRecord {
private:
    Animal* animal; // non-owning
    std::vector<VetVisit*> visits; // non-owning
    std::string current_status;

public:
    HealthRecord(Animal* a = nullptr, std::string status = "Healthy");
    // меняет статус животного на поле diagnosis из VetVisit
    void mark_recovered();
    //вызывает метод prescribe_treatmen. если животное больно, вызывает mark_recovered() 
    void add_visit(VetVisit* visit);
};

class FeedingPlan {
private:
    Animal* animal; // non-owning
    std::vector<FeedStock*> feed_items; // non-owning
    double portion_grams;

public:
    FeedingPlan(Animal* a = nullptr, double portion = 0.0);
    // если изменился вес в record_weight_change, изменяет portion_grams в столько раз, во сколько изменился вес животного
    void adjust_portion(double last_weight);
    //проверяет соответствипе std::vector<FeedStock*> feed_items diet_type из поля spicies этого животного
    bool check_diet_compliance() const;
    
    //добавляет item в  std::vector<FeedStock*> feed_items
    void add_feed_item(FeedStock* item);
};

class Animal {
private:
    std::string animal_id;
    Species species;                    // Ассоциация 15 (композиция)
    Habitat* habitat;                   // Ассоциация 16 (агрегация, non-owning)
    HealthRecord health_record;         // Ассоциация 17 (композиция)
    FeedingPlan feeding_plan;           // Ассоциация 18 (композиция)
    double weight_kg;

public:
    Animal(std::string id = "", Species sp = Species(), double weight = 0.0);

    //запрет копирования (из-за указателей на самого себя внутри)
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;
    
    //TODO добавить метод добавления id животному (если id изначально = "") 
    
    // изменение веса животного. если изменился больше чем на 5% вызвать add_visit(VetVisit* visit) и mark_recovered() мз health record. ВЫЗОВ void adjust_portion();
    void record_weight_change(double new_weight);
    //изменить среду обитания на другую в полях животного TODO ВСПОМОГАТЕЛЬНАЯ
    void move_to_habitat(Habitat* new_habitat);
    //Назначает животному новый рацион. Копирует переданный план в feeding_plan (здесь нужно не забыть обновить указатель plan.animal = this внутри реализации).
    void assign_feeding_plan(const FeedingPlan& plan);

    // Геттеры
    std::string get_id() const { return animal_id; }
    double get_weight() const { return weight_kg; }
    Habitat* get_habitat() const { return habitat; }
    const Species& get_species() const { return species; } // для проверки совместимости
};

class BreedingProgram {
private:
    Species species;
    std::vector<std::pair<Animal*, Animal*>> pairs; // non-owning
    std::string status;

public:
    BreedingProgram(Species sp = Species(), std::string status = "Inactive");
    //Формирует пару из двух животных для размножения. Добавляет их в вектор pairs.
    void select_pair(Animal* parent1, Animal* parent2);
    //Назначает дату/время и запускает процесс размножения (меняет статус программы на "Active. Data: ...")
    void schedule_breeding(const std::string& data);
};

//заявка на перевод животного
class AnimalTransfer {
private:
    Animal* animal; // non-owning
    Tank* destination_tank; // non-owning

public:
    //Создает заявку. Запоминает, кого переводят и куда 
    AnimalTransfer(Animal* a = nullptr, Tank* dest = nullptr);
    //Удалить животное из старого танка (animal->get_habitat()->remove_animal(...)). Добавить в новый (destination_tank->add_animal(...)). Обновить указатель внутри животного
    void approve_transfer();
};
