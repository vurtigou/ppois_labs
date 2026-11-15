#include "stuff.h"
#include "exceptions.h"
#include "visitors.h"
#include "inventory.h"
#include <cstdlib> // Для rand()
#include <iostream>
#include <algorithm>
#include <map>

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
    : Employee(id, exp, static_cast<int>(min_rate * exp), 0), mop(nullptr) {} 
std::string Janitor::get_position() const { return position; }

void Janitor::set_mop(Mop* m) { mop = m; }

void Janitor::clean_area() {
    if (!mop) {
        std::cout << "Уборщик (ID:" << id << "): У меня нет швабры!" << std::endl;
        return;
    }
    try {
        std::cout << "Уборщик (ID:" << id << ") начинает мыть пол..." << std::endl;
        mop->use(); 
        
        // Если швабра сильно загрязнилась в процессе, уборщик её моет
        if (mop->get_dirt_level() >= 75) {
            std::cout << "Уборщик: Швабра слишком грязная, иду мыть." << std::endl;
            mop->wash();
        }
    } 
    catch (const EquipmentFailureError& e) {
        std::cerr << "Уборщик (ID:" << id << "): Швабра сломалась! " << e.what() << std::endl;
    } 
    catch (const ResourceDepletedError& e) {
        std::cerr << "Уборщик (ID:" << id << "): Закончилась вода или швабра слишком грязная! " << e.what() << std::endl;
    } 
    catch (const OceanariumError& e) {
        std::cerr << "Уборщик (ID:" << id << ") не смог помыть пол: " << e.what() << std::endl;
    }
}

Plumber::Plumber(int id, int exp, int access) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), access), equipment(nullptr) {}
std::string Plumber::get_position() const { return position; }

void Plumber::set_equipment(PlumberEquipment* eq) { equipment = eq; }

bool Plumber::repair_leak() {
    if (!equipment) {
        std::cout << "Сантехник (ID:" << id << "): У меня нет инструментов!" << std::endl;
        return false;
    }
    try {
        std::cout << "Сантехник (ID:" << id << ") берется за ремонт..." << std::endl;
        equipment->use(); // Вызывает fix_leak()
        return true;
    } 
    catch (const EquipmentFailureError& e) {
        std::cerr << "Сантехник (ID:" << id << "): Ошибка инструментов: " << e.what() << std::endl;
        return false;
    } 
    catch (const ResourceDepletedError& e) {
        std::cerr << "Сантехник (ID:" << id << "): Не хватает ресурсов: " << e.what() << std::endl;
        return false;
    }
}

Electrician::Electrician(int id, int exp, int access) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), access), equipment(nullptr) {}
std::string Electrician::get_position() const { return position; }

void Electrician::set_equipment(ElectricianEquipment* eq) { equipment = eq; }

void Electrician::inspect_circuits() {
    if (!equipment) {
        std::cout << "Электрик (ID:" << id << "): У меня нет оборудования!" << std::endl;
        return;
    }
    try {
        std::cout << "Электрик (ID:" << id << ") проверяет цепи..." << std::endl;
        equipment->use(); // Вызывает test_circuit()
    } catch (const ResourceDepletedError& e) {
        std::cerr << "Электрик (ID:" << id << "): " << e.what() << " (Пора зарядить батарею!)" << std::endl;
    } catch (const EquipmentFailureError& e) {
        std::cerr << "Электрик (ID:" << id << "): КРИТИЧЕСКАЯ ОШИБКА: " << e.what() << std::endl;
    }
}

Cashier::Cashier(int id, int exp) 
    : Employee(id, exp, static_cast<int>(min_rate * exp), 0), tickets_sold(0) {}
std::string Cashier::get_position() const { return position; }
      
std::pair<bool, std::unique_ptr<Ticket>>
Cashier::sell_ticket(Visitor& visitor, std::unique_ptr<Ticket> ticket) {
    if (!ticket) {
        throw NullPointerError("Cashier: Cannot sell a null ticket.");
    }

    // Visitor::buy_ticket возвращает: nullptr      — успешно куплено; nique_ptr   — не куплено, билет возвращён нам.
    std::unique_ptr<Ticket> unsold = visitor.buy_ticket(std::move(ticket));

    if (!unsold) {
        tickets_sold++;
        std::cout << "Кассир: Билет успешно продан посетителю "
                  << visitor.get_full_name() << "!" << std::endl;
        return {true, nullptr};
    }

    // Не куплено — билет возвращаем вызывающей стороне.
    std::cout << "Кассир: Не удалось продать билет посетителю "
              << visitor.get_full_name()
              << " (билет возвращён в кассу)." << std::endl;
    return {false, std::move(unsold)};
}

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
void Schedule::add_shift(Shift& shift) {
    if (!shift.get_employee()) {
        throw StaffManagementError("Schedule: Cannot add a shift with a null employee.");
    }

    // Защита от повторного добавления той же смены
    for (const Shift* s : shifts) {
        if (s == &shift) {
            throw DuplicateEntityError("Schedule: This shift is already added.");
        }
    }

    shifts.push_back(&shift);
}

// Функция распределения зарплат
void Schedule::calculate_monthly_salaries() {
    std::map<Employee*, std::vector<Shift*>> pending;
    for (Shift* shift : shifts) {
        if (!shift->paid) pending[shift->get_employee()].push_back(shift);
    }

    for (const auto& [employee, employee_shifts] : pending) {
        double total = 0.0;
        for (Shift* shift : employee_shifts) total += shift->calculate_shift_rate();

        employee->add_salary(total);
        for (Shift* shift : employee_shifts) shift->paid = true;

        if (std::find(employees.begin(), employees.end(), employee) == employees.end()) {
            employees.push_back(employee);
        }
    }
}
