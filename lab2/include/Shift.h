#pragma once

#include <string>

class Employee;

// Класс Смена
/** @brief Employee work period with incidents, adjustments, and calculated pay.
 * @ingroup staff
 */
class Shift {
    friend class Schedule;

private:
    Employee* employee;     // ссылается на реального работника (non-owning)
    std::string start_time; // время начала смены
    std::string end_time;   // время конца смены
    int incidents;          // количество происшествий
    bool has_penalty;
    bool has_bonus;
    // сколько в итоге заработал за смену
    bool paid = false;
    mutable double shift_rate; // mutable для изменения в const-методе

public:
    Shift(Employee* emp = nullptr, std::string start = "", std::string end = "");

    // Генерирует количество происшествий от 1 до 15 случайным образом
    void generate_incidents();
    void generate_events();

    // Вычисляет ставку работника за эту смену
    double calculate_shift_rate() const;

    int get_incidents() const { return incidents; }
    Employee* get_employee() const { return employee; }
    double get_shift_rate() const { return shift_rate; }
};
