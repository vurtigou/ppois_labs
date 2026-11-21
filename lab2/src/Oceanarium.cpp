#include "Oceanarium.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

#include "exceptions.h" // OceanariumError
#include "Tank.h"   // Tank
#include "Employee.h"      // Employee

Oceanarium::Oceanarium(std::string name, const Location& location)
    : name(std::move(name)), is_open(false), location(location) {

    if (this->name.empty()) {
        throw OceanariumError(
            "Ошибка создания: Название океанариума не может быть пустым.");
    }
    if (!this->location.is_valid()) {
        throw OceanariumError(
            "Ошибка создания: Локация океанариума некорректна (пустой город).");
    }

    audit_log.record_action("Oceanarium '" + this->name + "' created.");
}

Oceanarium::~Oceanarium() = default;

void Oceanarium::open_for_day() {
    if (is_open) {
        std::cout << "Oceanarium " << name << " is already OPEN." << std::endl;
        audit_log.record_action("Attempt to open Oceanarium '" + name
                                + "': already open.");
        return;
    }
    is_open = true;
    std::cout << "Oceanarium " << name << " is now OPEN." << std::endl;
    audit_log.record_action("Oceanarium opened for the day.");
}

void Oceanarium::close_for_day() {
    if (!is_open) {
        std::cout << "Oceanarium " << name << " is already CLOSED." << std::endl;
        audit_log.record_action("Attempt to close Oceanarium '" + name
                                + "': already closed.");
        return;
    }
    is_open = false;
    std::cout << "Oceanarium " << name << " is now CLOSED." << std::endl;
    audit_log.record_action("Oceanarium closed for the day.");
}

// Регистрирует танк в океанариуме
void Oceanarium::add_tank(Tank* tank) {
    if (!tank) {
        throw OceanariumError(
            "Ошибка добавления: Попытка добавить пустой указатель на Tank.");
    }
    if (std::find(tanks.begin(), tanks.end(), tank) != tanks.end()) {
        throw OceanariumError(
            "Ошибка добавления: Этот танк уже зарегистрирован в океанариуме.");
    }
    tanks.push_back(tank);
    audit_log.record_action("Tank '" + tank->get_tank_id() + "' added.");
}

// Регистрирует сотрудника в океанариуме
void Oceanarium::add_employee(Employee* employee) {
    if (!employee) {
        throw OceanariumError(
            "Ошибка найма: Попытка добавить пустой указатель на Employee.");
    }
    if (std::find(employees.begin(), employees.end(), employee) != employees.end()) {
        throw OceanariumError(
            "Ошибка найма: Этот сотрудник уже работает в океанариуме.");
    }
    employees.push_back(employee);

    // в лог записывается ID и должность сотрудника
    audit_log.record_action("Employee with ID " + std::to_string(employee->get_id()) +
                            " (" + employee->get_position() + ") was hired.");
}
