#include "Schedule.h"

#include <algorithm>
#include <map>
#include <vector>

#include "Employee.h"
#include "Shift.h"
#include "exceptions.h"   // StaffManagementError, DuplicateEntityError

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
