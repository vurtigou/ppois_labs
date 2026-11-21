#include "PlumberEquipment.h"

#include <cstdlib>
#include <iostream>

#include "exceptions.h"   // EquipmentFailureError

void PlumberEquipment::check_pressure() {
    if (!has_wrench) {
        throw EquipmentFailureError("PlumberEquipment: Cannot check pressure. The wrench is broken.");
    }
    pressure_gauge = rand() % 15 + 1; // случайное давление от 1 до 15
    std::cout << "Замер давления: " << pressure_gauge << " атм." << std::endl;
}

bool PlumberEquipment::fix_leak() {
    if (is_broken() || !has_wrench) {
        throw EquipmentFailureError("PlumberEquipment: Cannot fix leak. Tools are broken.");
    }

    wear_out(20); // Починка сильно изнашивает ключ
    if (durability <= 0) {
        has_wrench = false;
        // Инструмент сломался прямо в процессе
        throw EquipmentFailureError("PlumberEquipment: Wrench broke during the repair process!");
    }

    std::cout << "Протечка успешно устранена!" << std::endl;
    return true;
}

void PlumberEquipment::repair() {
    EmployeesEquipment::repair(); // Вызываем базовый ремонт
    has_wrench = true;            // И чиним ключ
}

void PlumberEquipment::use() {
    fix_leak();
}
