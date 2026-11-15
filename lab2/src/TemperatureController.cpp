#include "TemperatureController.h"

#include "WaterQuality.h"

TemperatureController::TemperatureController(int id, WaterQuality& quality,
                                             int access_level, bool is_good_working,
                                             int amperage)
    : ControlSystem(id, quality, access_level, is_good_working),
      amperage(amperage),
      initial_amperage((amperage >= 5 && amperage <= 24) ? amperage : 12) {}

void TemperatureController::checking() {
    if (amperage < 5 || amperage > 24)
        mark_broken();
}

void TemperatureController::on_repair() {
    amperage = initial_amperage;   // вернуть в допустимый диапазон
}

void TemperatureController::improving() {
    checking();
    if (!get_is_good_working()) return;    // <-- критично: не трогаем воду, если сломан

    double current_temp = quality.get_temperature();
    if (current_temp >= 15.0 && current_temp <= 25.0) return;

    quality.set_temperature(20.0);
    checking();
}
