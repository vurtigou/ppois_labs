#pragma once

#include <string>
#include <vector>

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
    virtual double get_pin_rate()    const = 0;
    virtual double get_bon_rate()    const = 0;

    int get_rate()           const { return rate; }
    int get_experience()     const { return experience; }
    int get_id()             const { return id; }
    int get_access_level()   const { return access_level; }
    int get_amount_penalty() const { return amount_penalty; }
    int get_amount_bonus()   const { return amount_bonus; }

    const std::vector<double>& get_salaries() const { return salaries; }
    void add_salary(double amount) { salaries.push_back(amount); }
    void update_last_salary(double amount) { if (!salaries.empty()) salaries.back() += amount; }

    void add_penalty() { amount_penalty++; }
    void add_bonus()   { amount_bonus++; }
};
