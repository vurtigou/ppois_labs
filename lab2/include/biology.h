#pragma once

#include <string>
#include <vector>
#include <utility> 
#include <memory>
#include "inventory.h" //для FeedStock

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
    std::string diet_type; //строка с названием еды
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
    std::vector<std::unique_ptr<VetVisit>> visits;
    std::string current_status;

public:
    HealthRecord(Animal* a = nullptr, std::string status = "Healthy");
    HealthRecord(const HealthRecord&) = delete;
    HealthRecord& operator=(const HealthRecord&) = delete;
    void status_diagnosis();
    void add_visit(VetVisit* visit);
};

class FeedingPlan {
private:
    Animal* animal; // non-owning
    std::vector<FeedStock*> feed_items; // non-owning
    double portion_grams;

public:
    FeedingPlan(Animal* a = nullptr, double portion = 0.0);
    void adjust_portion(double last_weight);
    void add_feed_item(FeedStock* item);
    bool check_diet_compliance() const;
    void calculate_portion();

    void set_animal(Animal* a) { animal = a; }
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
    
    virtual ~Animal() = default;

    //запрет копирования (из-за указателей на самого себя внутри)
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;
    
    void set_id(const std::string& new_id); 
    virtual void record_weight_change(double new_weight);
    //TODO ВСПОМОГАТЕЛЬНАЯ
    virtual void move_to_habitat(Habitat* new_habitat);
    void assign_feeding_plan(const FeedingPlan& plan);
    
    virtual bool can_breed() const { return true; } // Обычные животные могут размножаться
    virtual void on_vet_visit() {}                  // Реакция на визит (для Cub - увеличение возраста)

    std::string get_id() const { return animal_id; }
    double get_weight() const { return weight_kg; }
    Habitat* get_habitat() const { return habitat; }
    const Species& get_species() const { return species; }
    HealthRecord& get_health_record() { return health_record; }
};

class Cub : public Animal {
private:
    int age;

public:
    // Конструктор принимает ID родителя, чтобы сгенерировать свой ID
    Cub(const std::string& parent_id, Species sp = Species(), double weight = 0.0);

    // Переопределяем методы родителя
    void record_weight_change(double new_weight) override;
    void move_to_habitat(Habitat* new_habitat) override;
    
    // Детеныши не могут спариваться
    bool can_breed() const override { return false; }
    
    // При каждом визите к врачу возраст увеличивается
    void on_vet_visit() override { age++; }
    
    int get_age() const { return age; }
};

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
