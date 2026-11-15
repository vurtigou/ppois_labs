#include "EmployeesEquipment.h"

#include <iostream>

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
