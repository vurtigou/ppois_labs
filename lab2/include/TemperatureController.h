#pragma once

#include "ControlSystem.h"

/** @brief Brings water temperature back toward its operating range.
 * @ingroup water
 */
class TemperatureController : public ControlSystem {
private:
    int amperage;
    int initial_amperage;

public:
    TemperatureController(int id, WaterQuality& quality,
                          int access_level = 0, bool is_good_working = true,
                          int amperage = 0);

    void checking()  override;
    void improving() override;
    void on_repair() override;
};
