#pragma once

#include "ControlSystem.h"

class Ozonator : public ControlSystem {
private:
    double availible_vol_ozon;
    double vol_coal;
    double max_ozon;
    double max_coal;

public:
    Ozonator(int id, WaterQuality& quality,
             int access_level = 0, bool is_good_working = true,
             double availible_vol_ozon = 0.0, double vol_coal = 0.0);

    void checking()  override;
    void improving() override;
    void on_repair() override;
};
