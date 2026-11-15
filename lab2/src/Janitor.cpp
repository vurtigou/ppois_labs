#include "Janitor.h"

#include <iostream>

#include "exceptions.h"   // EquipmentFailureError, ResourceDepletedError, OceanariumError
#include "Mop.h"    // Mop

// Инициализация статических полей
const std::string Janitor::position = "Janitor";
const double Janitor::min_rate = 100.0;
const double Janitor::pin_rate = 20.0;
const double Janitor::bon_rate = 30.0;

// Ставка = минимальная ставка * опыт работы
Janitor::Janitor(int id, int exp)
    : Employee(id, exp, static_cast<int>(min_rate * exp), 0), mop(nullptr) {}

std::string Janitor::get_position() const { return position; }

void Janitor::set_mop(Mop* m) { mop = m; }

void Janitor::clean_area() {
    if (!mop) {
        std::cout << "Уборщик (ID:" << id << "): У меня нет швабры!" << std::endl;
        return;
    }
    try {
        std::cout << "Уборщик (ID:" << id << ") начинает мыть пол..." << std::endl;
        mop->use();

        // Если швабра сильно загрязнилась в процессе, уборщик её моет
        if (mop->get_dirt_level() >= 75) {
            std::cout << "Уборщик: Швабра слишком грязная, иду мыть." << std::endl;
            mop->wash();
        }
    }
    catch (const EquipmentFailureError& e) {
        std::cerr << "Уборщик (ID:" << id << "): Швабра сломалась! " << e.what() << std::endl;
    }
    catch (const ResourceDepletedError& e) {
        std::cerr << "Уборщик (ID:" << id << "): Закончилась вода или швабра слишком грязная! " << e.what() << std::endl;
    }
    catch (const OceanariumError& e) {
        std::cerr << "Уборщик (ID:" << id << ") не смог помыть пол: " << e.what() << std::endl;
    }
}
