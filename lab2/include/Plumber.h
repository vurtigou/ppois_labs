#pragma once

#include <string>

#include "Employee.h"

class PlumberEquipment;

// Класс Сантехник
/** @brief Employee who repairs leaks with plumbing equipment.
 * @ingroup staff
 */
class Plumber : public Employee {
private:
    PlumberEquipment* equipment;

public:
    static const std::string position;
    static const double min_rate;
    // ставка за штраф
    static const double pin_rate;
    // ставка за бонус
    static const double bon_rate;

    Plumber(int id, int exp, int access);

    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }

    void set_equipment(PlumberEquipment* eq);
    bool repair_leak();
};
