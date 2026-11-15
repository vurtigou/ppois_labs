#pragma once

#include "EmployeesEquipment.h"

class Mop : public EmployeesEquipment {
private:
    int dirt_level;     // Загрязненность (0 - чистая, 100 - грязная)
    int water_level;    // Уровень воды в ведре (0 - сухо, 100 - полно)

public:
    Mop(int max_dur = 50)
        : EmployeesEquipment(max_dur), dirt_level(0), water_level(100) {}

    void clean();
    void wash();          // Помыть швабру
    void refill_water();  // Налить воды
    void use() override;

    int get_dirt_level()  const { return dirt_level; }
    int get_water_level() const { return water_level; }
};
