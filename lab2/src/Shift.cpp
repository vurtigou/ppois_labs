#include "Shift.h"

#include <cstdlib>
#include <string>

#include "Employee.h"
#include "exceptions.h"   // NullPointerError

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
