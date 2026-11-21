#include "WaterQuality.h"

#include <cmath>
#include <string>

#include "exceptions.h"   // WaterQualityViolationError

WaterQuality::WaterQuality(double ph, double salinity,
                           double temperature, double oxygen_level)
{
    set_ph(ph);
    set_salinity(salinity);
    set_temperature(temperature);
    set_oxygen_level(oxygen_level);
}

// Валидация в сеттерах — чтобы нельзя было выставить запрещённые значения
void WaterQuality::set_ph(double v) {
    if (!std::isfinite(v) || v < 0.0 || v > 14.0)
        throw WaterQualityViolationError("pH out of range [0, 14]: " + std::to_string(v));
    ph = v;
}
void WaterQuality::set_salinity(double v) {
    if (!std::isfinite(v) || v < 0.0 || v > 45.0)
        throw WaterQualityViolationError("Salinity out of range [0, 45]: " + std::to_string(v));
    salinity = v;
}
void WaterQuality::set_temperature(double v) {
    if (!std::isfinite(v) || v < -2.0 || v > 40.0)
        throw WaterQualityViolationError("Temperature out of range [-2, 40]: " + std::to_string(v));
    temperature = v;
}
void WaterQuality::set_oxygen_level(double v) {
    if (!std::isfinite(v) || v < 0.0 || v > 20.0)
        throw WaterQualityViolationError("Oxygen level out of range [0, 20]: " + std::to_string(v));
    oxygen_level = v;
}

bool WaterQuality::is_within_norm() const {
    return ph >= 6.5 && ph <= 8.5
        && salinity >= 0.0 && salinity <= 35.0
        && temperature >= 15.0 && temperature <= 25.0
        && oxygen_level >= 5.0 && oxygen_level <= 10.0;
}
