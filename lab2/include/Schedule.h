#pragma once

#include <vector>

class Employee;
class Shift;

// Класс График на месяц
/** @brief Non-owning collection of shifts used to settle outstanding pay.
 * @ingroup staff
 */
class Schedule {
private:
    std::vector<Shift*>    shifts;    // non-owning
    std::vector<Employee*> employees;

public:
    Schedule() = default;
    /**
     * @brief Registers a shift that must outlive its registration in the schedule.
     */
    void add_shift(Shift& shift);

    /**
     * @brief Totals and posts each employee’s unpaid shifts, then marks those shifts paid.
     */
    void calculate_monthly_salaries();
};
