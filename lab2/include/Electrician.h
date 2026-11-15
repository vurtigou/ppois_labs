#pragma once

#include <string>

#include "Employee.h"

class ElectricianEquipment;

// Класс Электрик
class Electrician : public Employee {
private:
    ElectricianEquipment* equipment;

public:
    static const std::string position;
    static const double min_rate;
    // ставка за штраф
    static const double pin_rate;
    // ставка за бонус
    static const double bon_rate;

    Electrician(int id, int exp, int access);

    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }

    void set_equipment(ElectricianEquipment* eq);
    void inspect_circuits();
};
