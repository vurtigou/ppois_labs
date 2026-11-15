#pragma once

#include <string>

#include "Employee.h"

class Mop;

// Класс Уборщик
class Janitor : public Employee {
private:
    Mop* mop;

public:
    static const std::string position;
    static const double min_rate;
    // ставка за штраф
    static const double pin_rate;
    // ставка за бонус
    static const double bon_rate;

    Janitor(int id, int exp);

    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }

    void set_mop(Mop* m);
    void clean_area();
};
