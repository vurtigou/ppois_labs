#include "ControlSystem.h"

#include <string>
#include <vector>

#include "exceptions.h"   // NullPointerError, DuplicateEntityError,
                          // EntityNotFoundError, EmployeePermissionDeniedError,
                          // InvalidDateError
#include "Employee.h"        // Employee

ControlSystem::ControlSystem(int id, WaterQuality& quality,
                             int access_level, bool is_good_working)
    : id(id), access_level(access_level),
      is_good_working(is_good_working), quality(quality) {}

void ControlSystem::assign_employee(Employee* employee) {
    if (!employee)
        throw NullPointerError("Cannot assign null employee to control system");
    for (const Employee* e : employees) {
        if (e && e->get_id() == employee->get_id())
            throw DuplicateEntityError(
                "Employee " + std::to_string(employee->get_id()) +
                " is already assigned to control system " + std::to_string(id));
    }
    employees.push_back(employee);
}

void ControlSystem::release_employee(int employee_id) {
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if (*it && (*it)->get_id() == employee_id) {
            employees.erase(it);
            return;
        }
    }
    throw EntityNotFoundError(
        "Employee " + std::to_string(employee_id) +
        " is not assigned to control system " + std::to_string(id));
}

bool ControlSystem::can_work(const Employee* employee) const {
    if (!employee) return false;
    bool found = false;
    for (const Employee* e : employees) {
        if (e && e->get_id() == employee->get_id()) { found = true; break; }
    }
    if (!found) return false;
    return employee->get_access_level() >= access_level;
}

void ControlSystem::perform_work(const Employee* employee, int date) {
    if (!employee)
        throw NullPointerError("Cannot perform work on control system " +
                               std::to_string(id) + " with null employee");
    if (!can_work(employee))
        throw EmployeePermissionDeniedError(
            "Employee " + std::to_string(employee->get_id()) +
            " is not allowed to service control system " + std::to_string(id));
    if (!is_valid_date(date))
        throw InvalidDateError("Invalid date, expected ddmmyyyy: " + std::to_string(date));
    for (int d : repair_dates) {
        if (d == date)
            throw DuplicateEntityError(
                "Repair on " + std::to_string(date) +
                " already recorded for system " + std::to_string(id));
    }

    if (!is_good_working) {
        is_good_working = true;
        access_level    = 0;
        on_repair();          // <-- восстановить расходники/настройки
    }
    repair_dates.push_back(date);
}

bool ControlSystem::is_valid_date(int date) {
    if (date < 1011900 || date > 31122999) return false;
    int day   =  date / 1000000;
    int month = (date / 10000)   % 100;
    int year  =  date           % 10000;
    if (year < 1900 || year > 2999 || month < 1 || month > 12) return false;
    constexpr int month_lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const bool leap_year = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    const int max_day = month_lengths[month - 1] + (month == 2 && leap_year ? 1 : 0);
    return day >= 1 && day <= max_day;
}
