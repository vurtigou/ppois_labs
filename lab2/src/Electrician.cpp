#include "Electrician.h"

#include <iostream>

#include "exceptions.h"   // EquipmentFailureError, ResourceDepletedError
#include "ElectricianEquipment.h"    // ElectricianEquipment

const std::string Electrician::position = "Electrician";
const double Electrician::min_rate = 200.0;
const double Electrician::pin_rate = 40.0;
const double Electrician::bon_rate = 50.0;

Electrician::Electrician(int id, int exp, int access)
    : Employee(id, exp, static_cast<int>(min_rate * exp), access), equipment(nullptr) {}

std::string Electrician::get_position() const { return position; }

void Electrician::set_equipment(ElectricianEquipment* eq) { equipment = eq; }

void Electrician::inspect_circuits() {
    if (!equipment) {
        std::cout << "Электрик (ID:" << id << "): У меня нет оборудования!" << std::endl;
        return;
    }
    try {
        std::cout << "Электрик (ID:" << id << ") проверяет цепи..." << std::endl;
        equipment->use(); // Вызывает test_circuit()
    } catch (const ResourceDepletedError& e) {
        std::cerr << "Электрик (ID:" << id << "): " << e.what() << " (Пора зарядить батарею!)" << std::endl;
    } catch (const EquipmentFailureError& e) {
        std::cerr << "Электрик (ID:" << id << "): КРИТИЧЕСКАЯ ОШИБКА: " << e.what() << std::endl;
    }
}
