#include "water.h"
#include "exceptions.h"

#include <cmath>
#include <algorithm>

// ---------------- WaterQuality ----------------

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

// ---------------- ControlSystem ----------------

ControlSystem::ControlSystem(int id, WaterQuality& quality,
                             int access_level, bool is_good_working)
    : id(id), access_level(access_level),
      is_good_working(is_good_working), quality(quality) {}

void ControlSystem::assign_employee(Employee* employee) {
    if (!employee)
        throw NullPointerError("Cannot assign null employee to control system");
    for (const Employee* e : employees) {
        if (e && e->get_id() == employee->get_id())
            throw DuplicateEntityError(
                "Employee " + std::to_string(employee->get_id()) +
                " is already assigned to control system " + std::to_string(id));
    }
    employees.push_back(employee);
}

void ControlSystem::release_employee(int employee_id) {
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if (*it && (*it)->get_id() == employee_id) {
            employees.erase(it);
            return;
        }
    }
    throw EntityNotFoundError(
        "Employee " + std::to_string(employee_id) +
        " is not assigned to control system " + std::to_string(id));
}

bool ControlSystem::can_work(const Employee* employee) const {
    if (!employee) return false;
    bool found = false;
    for (const Employee* e : employees) {
        if (e && e->get_id() == employee->get_id()) { found = true; break; }
    }
    if (!found) return false;
    return employee->get_access_level() >= access_level;
}

void ControlSystem::perform_work(const Employee* employee, int date) {
    if (!employee)
        throw NullPointerError("Cannot perform work on control system " +
                               std::to_string(id) + " with null employee");
    if (!can_work(employee))
        throw EmployeePermissionDeniedError(
            "Employee " + std::to_string(employee->get_id()) +
            " is not allowed to service control system " + std::to_string(id));
    if (!is_valid_date(date))
        throw InvalidDateError("Invalid date, expected ddmmyyyy: " + std::to_string(date));
    for (int d : repair_dates) {
        if (d == date)
            throw DuplicateEntityError(
                "Repair on " + std::to_string(date) +
                " already recorded for system " + std::to_string(id));
    }

    if (!is_good_working) {
        is_good_working = true;
        access_level    = 0;
        on_repair();          // <-- восстановить расходники/настройки
    }
    repair_dates.push_back(date);
}

bool ControlSystem::is_valid_date(int date) {
    if (date < 1011900 || date > 31122999) return false;
    int day   =  date / 1000000;
    int month = (date / 10000)   % 100;
    int year  =  date           % 10000;
    if (year < 1900 || year > 2999 || month < 1 || month > 12) return false;
    constexpr int month_lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const bool leap_year = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    const int max_day = month_lengths[month - 1] + (month == 2 && leap_year ? 1 : 0);
    return day >= 1 && day <= max_day;
}

// ---------------- Ozonator ----------------

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

// ---------------- TemperatureController ----------------

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

// ---------------- SalinityController ----------------

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

// ---------------- WaterSensor ----------------

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
