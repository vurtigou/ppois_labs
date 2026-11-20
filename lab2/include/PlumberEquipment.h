#pragma once

#include "EmployeesEquipment.h"

/** @brief Pressure and leak-repair tools with limited durability.
 * @ingroup inventory
 */
class PlumberEquipment : public EmployeesEquipment {
private:
    int pressure_gauge; // Показатель давления в системе (для поиска утечек)
    bool has_wrench;    // Есть ли разводной ключ (если сломался - false)

public:
    PlumberEquipment(int max_dur = 100)
        : EmployeesEquipment(max_dur), pressure_gauge(10), has_wrench(true) {}

    void check_pressure(); // Проверить давление
    bool fix_leak();       // Починить протечку (тратит прочность)
    void use() override;
    void repair() override;

    int get_pressure_gauge() const { return pressure_gauge; }
    bool get_has_wrench()    const { return has_wrench; }
};
