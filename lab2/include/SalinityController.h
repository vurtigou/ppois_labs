#pragma once

#include "ControlSystem.h"

/** @brief Dilutes water when salinity exceeds the operating range.
 * @ingroup water
 */
class SalinityController : public ControlSystem {
private:
    double vol_water;
    double vol_brine;
    double max_water;
    double max_brine;

public:
    SalinityController(int id, WaterQuality& quality,
                       int access_level = 0, bool is_good_working = true,
                       double vol_water = 0.0, double vol_brine = 0.0);

    void checking()  override;
    void improving() override;
    void on_repair() override;
};
