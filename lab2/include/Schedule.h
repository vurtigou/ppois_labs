#pragma once

#include <vector>

class Employee;
class Shift;

// Класс График на месяц
class Schedule {
private:
    std::vector<Shift*>    shifts;    // non-owning
    std::vector<Employee*> employees;

public:
    Schedule() = default;
    void add_shift(Shift& shift);

    void calculate_monthly_salaries();
};
