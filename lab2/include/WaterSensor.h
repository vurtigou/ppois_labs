#pragma once

#include "ControlSystem.h"

/** @brief Corrects pH using available carbon dioxide or alkali.
 * @ingroup water
 */
class WaterSensor : public ControlSystem {
private:
    double vol_co2;
    double vol_alkali;
    double max_co2;
    double max_alkali;

public:
    WaterSensor(int id, WaterQuality& quality,
                int access_level = 0, bool is_good_working = true,
                double vol_co2 = 0.0, double vol_alkali = 0.0);

    void checking()  override;
    void improving() override;
    void on_repair() override;
};
