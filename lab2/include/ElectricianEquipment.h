#pragma once

#include "EmployeesEquipment.h"

/** @brief Circuit-testing tools with battery and insulation levels.
 * @ingroup inventory
 */
class ElectricianEquipment : public EmployeesEquipment {
private:
    int insulation_level; // Уровень изоляции (если упадет до 0 - ударит током)
    int battery_level;    // Заряд аккумулятора для приборов

public:
    ElectricianEquipment(int max_dur = 80)
        : EmployeesEquipment(max_dur), insulation_level(100), battery_level(100) {}

    void test_circuit();       // Проверить цепь (тратит заряд)
    void replace_insulation(); // Заменить изоляцию
    void recharge_battery();   // Зарядить батарею
    void use() override;

    int get_insulation_level() const { return insulation_level; }
    int get_battery_level()    const { return battery_level; }
};
