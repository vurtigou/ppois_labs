#include "ElectricianEquipment.h"

#include <iostream>

#include "exceptions.h"   // EquipmentFailureError, ResourceDepletedError

void ElectricianEquipment::test_circuit() {
    if (battery_level < 10) {
        throw ResourceDepletedError("ElectricianEquipment: Battery level is too low.");
    }
    if (insulation_level < 30) {
        wear_out(50);
        throw EquipmentFailureError("ElectricianEquipment: SHOCK HAZARD! Insulation is damaged.");
    }

    battery_level    -= 10;
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
