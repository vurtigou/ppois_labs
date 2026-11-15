#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility> 

class Visitor;
class Ticket;
class Mop;
class PlumberEquipment;
class ElectricianEquipment;

// Базовый абстрактный класс
class Employee {
protected:
    int rate;        // ставка 
    int experience;  // стаж в месяцах
    int id; 
    int amount_penalty;
    int amount_bonus;        
    std::vector<double> salaries; 
    int access_level; 

public:
    Employee(int id = 0, int exp = 0, int rate = 0, int access_level = 0);
    virtual ~Employee() = default;

    virtual std::string get_position() const = 0; 
    virtual double get_pin_rate() const = 0;
    virtual double get_bon_rate() const = 0;

    int get_rate() const { return rate; }
    int get_experience() const { return experience; }
    int get_id() const { return id; }
    int get_access_level() const { return access_level; }
    int get_amount_penalty() const { return amount_penalty; }
    int get_amount_bonus() const { return amount_bonus; }

    const std::vector<double>& get_salaries() const { return salaries; }
    void add_salary(double amount) { salaries.push_back(amount); }
    void update_last_salary(double amount) { if (!salaries.empty()) salaries.back() += amount; }
    
    void add_penalty() { amount_penalty++; }
    void add_bonus() { amount_bonus++; }
};

// Класс Уборщик
class Janitor : public Employee {
private:
    Mop* mop;
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф 
    static const double pin_rate;
    //ставка за бонус 
    static const double bon_rate;

    Janitor(int id, int exp);
    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }
    void set_mop(Mop* m);  
    void clean_area();
};

// Класс Сантехник
class Plumber : public Employee {
private:
    PlumberEquipment* equipment;
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф
    static const double pin_rate;
    //ставка за бонус
    static const double bon_rate;

    Plumber(int id, int exp, int access);
    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }
    void set_equipment(PlumberEquipment* eq); 
    bool repair_leak();
};

// Класс Электрик
class Electrician : public Employee {
private:
    ElectricianEquipment* equipment;
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф
    static const double pin_rate;
    //ставка за бонус
    static const double bon_rate;

    Electrician(int id, int exp, int access);
    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }
    void set_equipment(ElectricianEquipment* eq); 
    void inspect_circuits();
};

class Cashier : public Employee {
private:
    int tickets_sold;
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф
    static const double pin_rate;
    //ставка за бонус
    static const double bon_rate;

    Cashier(int id, int exp);
    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }
    // Метод продажи билета
    std::pair<bool, std::unique_ptr<Ticket>>
    sell_ticket(Visitor& visitor, std::unique_ptr<Ticket> ticket);
    
    //Геттер для счетчика
    int get_tickets_sold() const { return tickets_sold; }
};

// Класс Смена
class Shift {
    friend class Schedule;

private:
    Employee* employee; // ссылается на реального работника (non-owning)
    std::string start_time; // время начала смены
    std::string end_time;   // время конца смены
    int incidents;          // количество происшествий
    bool has_penalty;
    bool has_bonus;
    //сколько в итоге заработал за смену
    bool paid = false;
    mutable double shift_rate; // добавлено mutable для изменения в const-методе

public:
    Shift(Employee* emp = nullptr, std::string start = "", std::string end = "");

    // Генерирует количество происшествий от 1 до 15 случайным образом
    void generate_incidents();
    void generate_events();

    // Вычисляет ставку работника за эту смену (происшествия * ставка - ставка работника за штраф + ставка работника за  бонус)
    double calculate_shift_rate() const;
    
    int get_incidents() const { return incidents; }
    Employee* get_employee() const { return employee; }
    double get_shift_rate() const { return shift_rate; }
};

// Класс График на месяц
class Schedule {
private:
    std::vector<Shift*> shifts;      // non-owning
    std::vector<Employee*> employees;

public:
    Schedule() = default;
    void add_shift(Shift& shift);

    void calculate_monthly_salaries();
};
