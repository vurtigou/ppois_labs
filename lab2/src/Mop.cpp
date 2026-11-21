#include "Mop.h"

#include <algorithm>
#include <iostream>

#include "exceptions.h"   // EquipmentFailureError, ResourceDepletedError

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
    dirt_level  = std::min(dirt_level + 25, 100);
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
