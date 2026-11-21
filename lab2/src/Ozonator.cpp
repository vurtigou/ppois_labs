#include "Ozonator.h"

#include <algorithm>
#include <cmath>

#include "WaterQuality.h"

Ozonator::Ozonator(int id, WaterQuality& quality,
                   int access_level, bool is_good_working,
                   double availible_vol_ozon, double vol_coal)
    : ControlSystem(id, quality, access_level, is_good_working),
      availible_vol_ozon(availible_vol_ozon),
      vol_coal(vol_coal),
      max_ozon (availible_vol_ozon > 0.0 ? availible_vol_ozon : 100.0),
      max_coal (vol_coal           > 0.0 ? vol_coal           : 100.0) {}

void Ozonator::checking() {
    if (availible_vol_ozon <= 0.0 || vol_coal <= 0.0)
        mark_broken();
}

void Ozonator::on_repair() {
    availible_vol_ozon = max_ozon;
    vol_coal           = max_coal;
}

void Ozonator::improving() {
    checking();
    if (!get_is_good_working()) return;   // <-- не работаем, если сломан

    double current_ox = quality.get_oxygen_level();
    if (current_ox >= 5.0 && current_ox <= 10.0) return;

    double target = (current_ox < 5.0) ? 5.0 : 10.0;
    double delta  = std::fabs(target - current_ox);
    double needed_ozon = delta * 0.5;
    double needed_coal = delta * 1.0;

    if (availible_vol_ozon >= needed_ozon && vol_coal >= needed_coal) {
        availible_vol_ozon -= needed_ozon;
        vol_coal           -= needed_coal;
        quality.set_oxygen_level(target);
        return;
    }

    double max_delta_by_ozon = availible_vol_ozon / 0.5;
    double max_delta_by_coal = vol_coal           / 1.0;
    double max_delta = std::min(max_delta_by_ozon, max_delta_by_coal);

    double direction = (target > current_ox) ? 1.0 : -1.0;
    quality.set_oxygen_level(current_ox + direction * max_delta);

    availible_vol_ozon -= max_delta * 0.5;
    vol_coal           -= max_delta * 1.0;

    checking();
}
