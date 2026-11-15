#include "Exhibit.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Tank.h"
#include "exceptions.h"   // NullPointerError

Exhibit::Exhibit(std::string title, std::string description, bool is_open)
    : title(title), description(description), opened(is_open) {}

void Exhibit::open_exhibit() {
    if (opened) return;
    opened = true;
    std::cout << "Exhibit '" << title << "' is now OPEN for visitors." << std::endl;
}

void Exhibit::close_exhibit() {
    if (!opened) return;
    opened = false;
    std::cout << "Exhibit '" << title << "' is now CLOSED." << std::endl;
}

std::vector<Animal*> Exhibit::get_all_animals() const {
    std::vector<Animal*> all_animals;
    for (const auto* tank : tanks) {
        if (tank != nullptr) {
            const std::vector<Animal*>& tank_animals = tank->get_animals();
            // insert вставляет элементы из диапазона [begin, end) в конец вектора
            all_animals.insert(all_animals.end(), tank_animals.begin(), tank_animals.end());
        }
    }
    return all_animals;
}

void Exhibit::add_tank(Tank* tank) {
    if (!tank) throw NullPointerError("Exhibit: Cannot add null tank.");
    if (std::find(tanks.begin(), tanks.end(), tank) != tanks.end()) return;

    // отвязать от прежнего зала, иначе получим «двойное членство»
    Exhibit* prev = tank->get_exhibit();
    if (prev && prev != this) {
        prev->remove_tank(tank);
    }

    tanks.push_back(tank);
    tank->set_exhibit(this);
}

bool Exhibit::remove_tank(Tank* tank) {
    auto it = std::find(tanks.begin(), tanks.end(), tank);
    if (it == tanks.end()) return false;

    tanks.erase(it);
    // обнуляем привязку только если она всё ещё указывает на нас
    if (tank->get_exhibit() == this) {
        tank->set_exhibit(nullptr);
    }
    return true;
}
