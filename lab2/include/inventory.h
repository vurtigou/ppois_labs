#pragma once
#include <string>

class Visitor;

class FeedStock {};

class Headphones {
private:
    Visitor* visitor;
public:
    Headphones() : visitor(nullptr) {}
    bool is_assigned() const { return visitor != nullptr; }
    void set_visitor(Visitor* v) { visitor = v; }
    Visitor* get_visitor() const { return visitor; }
};

class EmployeesEquipment {};
class Mop : public EmployeesEquipment {};
class PlumberEquipment : public EmployeesEquipment {};
class ElectricianEquipment : public EmployeesEquipment {};

// Базовый класс для всей еды и напитков
class Buffet {
protected:
    double price;
    static double cash_register; // Статическая касса

public:
    Buffet(double price = 0.0) : price(price) {}
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
