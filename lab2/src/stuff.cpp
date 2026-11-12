#include "stuff.h"
#include "exceptions.h"
#include <cstdlib> // Для rand()

// Инициализация статических полей дочерних классов
const std::string Janitor::position = "Janitor";
const double Janitor::min_rate = 100.0;
const double Janitor::pin_rate = 20.0;
const double Janitor::bon_rate = 30.0;

const std::string Plumber::position = "Plumber";
const double Plumber::min_rate = 150.0;
const double Plumber::pin_rate = 30.0;
const double Plumber::bon_rate = 40.0;

const std::string Electrician::position = "Electrician";
const double Electrician::min_rate = 200.0;
const double Electrician::pin_rate = 40.0;
const double Electrician::bon_rate = 50.0;

const std::string Cashier::position = "Cashier";
const double Cashier::min_rate = 80.0;
const double Cashier::pin_rate = 25.0;
const double Cashier::bon_rate = 30.0;

// Реализация Employee
Employee::Employee(int id, int exp, int rate, int access_level) 
    : rate(rate), experience(exp), id(id), 
      amount_penalty(0), amount_bonus(0), access_level(access_level) {}

// Ставка = минимальная ставка * опыт работы
Janitor::Janitor(int id, int exp) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), 0) {} 
std::string Janitor::get_position() const { return position; }

Plumber::Plumber(int id, int exp, int access) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), access) {}
std::string Plumber::get_position() const { return position; }

Electrician::Electrician(int id, int exp, int access) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), access) {}
std::string Electrician::get_position() const { return position; }

Cashier::Cashier(int id, int exp) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), 0) {}
std::string Cashier::get_position() const { return position; }

Shift::Shift(Employee* emp, std::string start, std::string end) 
    : employee(emp), start_time(start), end_time(end), 
      incidents(0), has_penalty(false), has_bonus(false), shift_rate(0.0) {}

// Генерирует количество происшествий от 1 до 15 случайным образом
void Shift::generate_incidents() {
    if (!employee) throw NullPointerError("Shift: Cannot generate incidents for null employee.");
    incidents = rand() % 15 + 1;
}

// Генерирует число от 1 до 10.
void Shift::generate_events() {
    if (!employee) throw NullPointerError("Shift: Cannot generate events for null employee.");
    int event = rand() % 10 + 1;
    
    if (event == 1) {
        has_penalty = true;
        if (employee) employee->add_penalty();
    } else if (event == 10) {
        has_bonus = true;
        if (employee) employee->add_bonus();
    }
}

// Вычисляет ставку работника за эту смену
double Shift::calculate_shift_rate() const {
    if (!employee) throw NullPointerError("Shift: Cannot calculate rate for null employee.");
    
    // происшествия * ставка
    double result = incidents * employee->get_rate();
    
    // минус ставка работника за штраф
    if (has_penalty) {
        result -= employee->get_pin_rate();
    }
    
    // плюс ставка работника за бонус
    if (has_bonus) {
        result += employee->get_bon_rate();
    }
    
    shift_rate = result;
    return shift_rate;
}

// Реализация Schedule
void Schedule::add_shift(const Shift& shift) {
    if (!shift.get_employee()) {
        throw StaffManagementError("Schedule: Cannot add a shift with a null employee.");
    }
    shifts.push_back(shift);
}

// Функция распределения зарплат
void Schedule::calculate_monthly_salaries() {
    for (const auto& shift : shifts) {
        Employee* emp = shift.get_employee();
        if (!emp) continue;
        double actual_shift_rate = shift.calculate_shift_rate(); 

        bool found = false;
        for (size_t i = 0; i < employees.size(); ++i) {
            if (employees[i] == emp) { found = true; break; }
        }

        if (!found) {
            employees.push_back(emp);
            emp->add_salary(actual_shift_rate); // Используем посчитанное значение
        } else {
            emp->update_last_salary(actual_shift_rate);
        }
    }
}
