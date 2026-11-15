#pragma once

#include "ControlSystem.h"

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
