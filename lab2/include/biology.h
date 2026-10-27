#pragma once

#include <string>
#include <vector>
#include <utility> // Для std::pair

// Предварительные объявления классов из других пакетов
class Habitat;
class Tank;
class FeedStock; 

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
    
    //могут ли жить 2 животного в одном танке (не могут, если разных один ест другого)
    bool is_compatible_with_species(const Species& other) const;
    
    std::string get_latin_name() const { return latin_name; }
    std::string get_diet_type() const { return diet_type; } 
};

class VetVisit {
private:
    Animal* animal; // non-owning
    std::string diagnosis;
    std::string treatment;

public:
    VetVisit(Animal* a = nullptr, std::string diag = "", std::string diag = "");
    
//назначение нового лечения
    void prescribe_treatment(const std::string& new_treatment, const std::string& new_diagnosis);
    //проверяет назначено ли лечение и есть ли диагноз (диагноз или статус  вылечен), если да закрывает визит
    void close_visit();
};

class HealthRecord {
private:
    Animal* animal; // non-owning
    std::vector<VetVisit*> visits; // non-owning
    std::string current_status;

public:
    HealthRecord(Animal* a = nullptr, std::string status = "Healthy");
    //вызывает метод prescribe_treatmen
    void add_visit(VetVisit* visit);
    // меняет статус животного на поле diagnosis из VetVisit
    void mark_recovered();
};

class FeedingPlan {
private:
    Animal* animal; // non-owning
    std::vector<FeedStock*> feed_items; // non-owning
    double portion_grams;

public:
    FeedingPlan(Animal* a = nullptr, double portion = 0.0);
    //проверяет вес животного. если изменился, изменяет portion_grams в столько раз, во сколько изменился вес животного
    void adjust_portion();
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

    // изменение веса животного. если изменился больше чем на 5% вызвать add_visit(VetVisit* visit) и mark_recovered() мз health record
    void record_weight_change(double new_weight);
    //изменить среду обитания на другую, если в другой среде обитания прошла проверка на подходимость животному
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
