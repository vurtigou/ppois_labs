#include "WaterSensor.h"

#include "WaterQuality.h"

WaterSensor::WaterSensor(int id, WaterQuality& quality,
                         int access_level, bool is_good_working,
                         double vol_co2, double vol_alkali)
    : ControlSystem(id, quality, access_level, is_good_working),
      vol_co2(vol_co2), vol_alkali(vol_alkali),
      max_co2(vol_co2 > 0.0 ? vol_co2 : 100.0),
      max_alkali(vol_alkali > 0.0 ? vol_alkali : 100.0) {}

void WaterSensor::checking() {
    if (vol_co2 <= 0.0 || vol_alkali <= 0.0)
        mark_broken();
}

void WaterSensor::on_repair() {
    vol_co2    = max_co2;
    vol_alkali = max_alkali;
}

void WaterSensor::improving() {
    checking();
    if (!get_is_good_working()) return;

    double current_ph = quality.get_ph();
    if (current_ph >= 6.5 && current_ph <= 8.5) return;

    if (current_ph < 6.5) {
        double needed_delta    = 6.5 - current_ph;
        double needed_resource = needed_delta / 2.0;
        if (vol_alkali >= needed_resource) {
            vol_alkali -= needed_resource;
            quality.set_ph(6.5);
            return;
        }
        double possible_delta = vol_alkali * 2.0;
        quality.set_ph(current_ph + possible_delta);
        vol_alkali = 0.0;
        checking();
    } else {
        double needed_delta    = current_ph - 8.5;
        double needed_resource = needed_delta / 2.0;
        if (vol_co2 >= needed_resource) {
            vol_co2 -= needed_resource;
            quality.set_ph(8.5);
            return;
        }
        double possible_delta = vol_co2 * 2.0;
        quality.set_ph(current_ph - possible_delta);
        vol_co2 = 0.0;
        checking();
    }
}
