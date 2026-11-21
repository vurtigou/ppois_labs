#include "Plumber.h"

#include <iostream>

#include "exceptions.h"   // EquipmentFailureError, ResourceDepletedError
#include "PlumberEquipment.h"    // PlumberEquipment

const std::string Plumber::position = "Plumber";
const double Plumber::min_rate = 150.0;
const double Plumber::pin_rate = 30.0;
const double Plumber::bon_rate = 40.0;

Plumber::Plumber(int id, int exp, int access)
    : Employee(id, exp, static_cast<int>(min_rate * exp), access), equipment(nullptr) {}

std::string Plumber::get_position() const { return position; }

void Plumber::set_equipment(PlumberEquipment* eq) { equipment = eq; }

bool Plumber::repair_leak() {
    if (!equipment) {
        std::cout << "Сантехник (ID:" << id << "): У меня нет инструментов!" << std::endl;
        return false;
    }
    try {
        std::cout << "Сантехник (ID:" << id << ") берется за ремонт..." << std::endl;
        equipment->use(); // Вызывает fix_leak()
        return true;
    }
    catch (const EquipmentFailureError& e) {
        std::cerr << "Сантехник (ID:" << id << "): Ошибка инструментов: " << e.what() << std::endl;
        return false;
    }
    catch (const ResourceDepletedError& e) {
        std::cerr << "Сантехник (ID:" << id << "): Не хватает ресурсов: " << e.what() << std::endl;
        return false;
    }
}
