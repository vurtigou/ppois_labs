#include "SalinityController.h"

#include <algorithm>

#include "WaterQuality.h"

SalinityController::SalinityController(int id, WaterQuality& quality,
                                       int access_level, bool is_good_working,
                                       double vol_water, double vol_brine)
    : ControlSystem(id, quality, access_level, is_good_working),
      vol_water(vol_water), vol_brine(vol_brine),
      max_water(vol_water > 0.0 ? vol_water : 100.0),
      max_brine(vol_brine > 0.0 ? vol_brine : 100.0) {}

void SalinityController::checking() {
    if (vol_water <= 0.0 || vol_brine <= 0.0)
        mark_broken();
}

void SalinityController::on_repair() {
    vol_water = max_water;
    vol_brine = max_brine;
}

void SalinityController::improving() {
    checking();
    if (!get_is_good_working()) return;

    double current_sal = quality.get_salinity();
    constexpr double max_salinity = 35.0;
    constexpr double salinity_drop_per_unit = 2.0;
    if (current_sal <= max_salinity) return;

    const double needed_water = (current_sal - max_salinity) / salinity_drop_per_unit;
    const double used_water = std::min(vol_water, needed_water);
    quality.set_salinity(current_sal - used_water * salinity_drop_per_unit);
    vol_water -= used_water;
    checking();
}
