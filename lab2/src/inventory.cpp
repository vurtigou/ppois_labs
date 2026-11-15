#include "inventory.h"
#include <algorithm>
#include <cmath>
#include <iostream> 
#include <cstdlib>

double Buffet::cash_register = 0.0;

void EmployeesEquipment::wear_out(int amount) {
    durability -= amount;
    if (durability < 0) durability = 0;
    
    if (durability == 0) {
        std::cout << "Внимание! Инструмент сломался!" << std::endl;
    }
}

void EmployeesEquipment::repair() {
    if (durability == max_durability) {
        std::cout << "Инструмент уже в идеальном состоянии." << std::endl;
        return;
    }
    durability = max_durability;
    std::cout << "Инструмент успешно отремонтирован." << std::endl;
}

void Mop::clean() {
    if (is_broken()) {
        throw EquipmentFailureError("Mop: Cannot clean. The mop is broken.");
    }
    if (water_level <= 0) {
        throw ResourceDepletedError("Mop: No water in the bucket.");
    }
    if (dirt_level >= 90) {
        throw ResourceDepletedError("Mop: The mop is too dirty to clean.");
    }

    // Логика мытья
    dirt_level = std::min(dirt_level + 25, 100);
    water_level = std::max(water_level - 20, 0);
    wear_out(5); 
    
    std::cout << "Полы помыты. Загрязненность швабры: " << dirt_level 
              << "%, Воды осталось: " << water_level << "%" << std::endl;
}

void Mop::wash() {
    dirt_level = 0;
    water_level -= 30; // Тратим воду на мытье
    if (water_level < 0) water_level = 0;
    std::cout << "Швабра помыта. Воды в ведре: " << water_level << "%" << std::endl;
}

void Mop::refill_water() {
    water_level = 100;
    std::cout << "Ведро наполнено водой." << std::endl;
}

// Использование базового метода
void Mop::use() {
    clean(); // Использование швабры = мытье пола
}

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

void ElectricianEquipment::test_circuit() {
    if (battery_level < 10) {
        throw ResourceDepletedError("ElectricianEquipment: Battery level is too low.");
    }
    if (insulation_level < 30) {
        wear_out(50); 
        throw EquipmentFailureError("ElectricianEquipment: SHOCK HAZARD! Insulation is damaged.");
    }

    battery_level -= 10;
    wear_out(5);
    insulation_level -= 5;
    std::cout << "Цепь проверена. Все исправно." << std::endl;
}

void ElectricianEquipment::replace_insulation() {
    insulation_level = 100;
    std::cout << "Изоляция заменена." << std::endl;
}

void ElectricianEquipment::recharge_battery() {
    battery_level = 100;
    std::cout << "Батарея заряжена." << std::endl;
}

void ElectricianEquipment::use() {
    test_circuit();
}

Buffet::Buffet(double price) : price(price) {
    if (!std::isfinite(price) || price < 0.0) {
        throw InvalidActionError("Buffet: Price must be finite and nonnegative.");
    }
}
