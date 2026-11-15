#pragma once
#include <string>
#include "exceptions.h"

class Visitor;

class FeedStock {
private:
    std::string name; 
    double weight_kg;       // Вес корма в килограммах

public:
    // Конструктор
    FeedStock(std::string name = "", double weight_kg = 0.0)
        : name(name), weight_kg(weight_kg) {}

    std::string get_name() const { return name; }

    // Геттерыы
    double get_weight_kg() const { return weight_kg; }

    void set_name(const std::string& new_name) { name = new_name; }
    void set_weight_kg(double new_weight) { weight_kg = new_weight; }
};

class Headphones {
private:
    Visitor* visitor;
public:
    Headphones() : visitor(nullptr) {}
    bool is_assigned() const { return visitor != nullptr; }
    void set_visitor(Visitor* v) { visitor = v; }
    Visitor* get_visitor() const { return visitor; }
};

// Базовый класс для всего рабочего инвентаря
class EmployeesEquipment {
protected:
    int durability;      // Прочность (от 0 до 100)
    int max_durability;
    bool is_equipped;    // В руках ли у сотрудника (защита от одновременного использования)

public:
    EmployeesEquipment(int max_dur = 100) 
        : durability(max_dur), max_durability(max_dur), is_equipped(false) {}
    
    virtual ~EmployeesEquipment() = default;

    // Общие методы для всех инструментов
    void wear_out(int amount);
    virtual void repair();
    virtual void use() = 0; // Чисто виртуальный метод использования

    int get_durability() const { return durability; }
    bool is_broken() const { return durability <= 0; }
    bool get_is_equipped() const { return is_equipped; }
    void set_equipped(bool status) { is_equipped = status; }
};

class Mop : public EmployeesEquipment {
private:
    int dirt_level;     // Загрязненность (0 - чистая, 100 - грязная)
    int water_level;    // Уровень воды в ведре (0 - сухо, 100 - полно)

public:
    Mop(int max_dur = 50) : EmployeesEquipment(max_dur), dirt_level(0), water_level(100) {}

    void clean();
    void wash(); // Помыть швабру
    void refill_water(); // Налить воды
    void use() override;

    int get_dirt_level() const { return dirt_level; }
    int get_water_level() const { return water_level; }
};

class PlumberEquipment : public EmployeesEquipment {
private:
    int pressure_gauge; // Показатель давления в системе (для поиска утечек)
    bool has_wrench;    // Есть ли разводной ключ (если сломался - false)

public:
    PlumberEquipment(int max_dur = 100) : EmployeesEquipment(max_dur), pressure_gauge(10), has_wrench(true) {}

    void check_pressure(); // Проверить давление
    bool fix_leak();       // Починить протечку (тратит прочность)
    void use() override;
    void repair() override;

    int get_pressure_gauge() const { return pressure_gauge; }
    bool get_has_wrench() const { return has_wrench; }
};

class ElectricianEquipment : public EmployeesEquipment {
private:
    int insulation_level; // Уровень изоляции (если упадет до 0 - ударит током)
    int battery_level;    // Заряд аккумулятора для приборов

public:
    ElectricianEquipment(int max_dur = 80) : EmployeesEquipment(max_dur), insulation_level(100), battery_level(100) {}

    void test_circuit();       // Проверить цепь (тратит заряд)
    void replace_insulation(); // Заменить изоляцию
    void recharge_battery();   // Зарядить батарею
    void use() override;

    int get_insulation_level() const { return insulation_level; }
    int get_battery_level() const { return battery_level; }
};

// Базовый класс для всей еды и напитков
class Buffet {
protected:
    double price;
    static double cash_register; // Статическая касса

public:
    explicit Buffet(double price = 0.0);
    virtual ~Buffet() = default;

    double get_price() const { return price; }
    
    // Методы для работы с кассой
    static double get_cash_register() { return cash_register; }
    static void add_to_cash_register(double amount) { cash_register += amount; }
};

class Drinks : public Buffet {
private:
    int vol;
public:
    Drinks(double price = 0.0, int vol = 0) : Buffet(price), vol(vol) {}
    int get_vol() const { return vol; }
};

class AlcoholDrinks : public Buffet {
private:
    int vol;
    int degrees;
public:
    AlcoholDrinks(double price = 0.0, int vol = 0, int degrees = 0) 
        : Buffet(price), vol(vol), degrees(degrees) {}
    int get_vol() const { return vol; }
    int get_degrees() const { return degrees; }
};

class Food : public Buffet {
private:
    std::string compound;
    int weight;
public:
    Food(double price = 0.0, std::string compound = "", int weight = 0) 
        : Buffet(price), compound(compound), weight(weight) {}
    std::string get_compound() const { return compound; }
    int get_weight() const { return weight; }
};
