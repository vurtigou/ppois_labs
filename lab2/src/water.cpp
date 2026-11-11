#include "water.h"
#include "exceptions.h"

#include <cmath> 
#include <algorithm>
#include <stdexcept>

WaterQuality::WaterQuality(double ph, double salinity,
                           double temperature, double oxygen_level)
    : ph(ph), salinity(salinity),
      temperature(temperature), oxygen_level(oxygen_level)
{
    if (ph < 0.0 || ph > 14.0)
        throw WaterQualityViolationError(
            "pH out of range [0, 14]: " + std::to_string(ph));
    if (salinity < 0.0 || salinity > 45.0)
        throw WaterQualityViolationError(
            "Salinity out of range [0, 45]: " + std::to_string(salinity));
    if (temperature < -2.0 || temperature > 40.0)
        throw WaterQualityViolationError(
            "Temperature out of range [-2, 40]: " + std::to_string(temperature));
    if (oxygen_level < 0.0 || oxygen_level > 20.0)
        throw WaterQualityViolationError(
            "Oxygen level out of range [0, 20]: " + std::to_string(oxygen_level));
}

bool WaterQuality::is_within_norm() const {
    return ph >= 6.5 && ph <= 8.5
        && salinity >= 0.0 && salinity <= 35.0
        && temperature >= 15.0 && temperature <= 25.0
        && oxygen_level >= 5.0 && oxygen_level <= 10.0;
}

ControlSystem::ControlSystem(int id, WaterQuality& quality,
                             int access_level, bool is_good_working)
    : id(id),
      access_level(access_level),
      is_good_working(is_good_working),
      quality(quality) {}
      
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
        throw NullPointerError(
            "Cannot perform work on control system " + std::to_string(id) +
            " with null employee");

    if (!can_work(employee))
        throw EmployeePermissionDeniedError(
            "Employee " + std::to_string(employee->get_id()) +
            " is not allowed to service control system " + std::to_string(id));

    if (!is_valid_date(date))
        throw InvalidDateError(
            "Invalid date, expected ddmmyyyy: " + std::to_string(date));

    for (int d : repair_dates) {
        if (d == date)
            throw DuplicateEntityError(
                "Repair on " + std::to_string(date) +
                " already recorded for system " + std::to_string(id));
    }

    if (!is_good_working) {
        is_good_working = true;
        access_level   = 0;
    }
    repair_dates.push_back(date);
}

bool ControlSystem::is_valid_date(int date) {
    if (date < 1011900 || date > 31122999) return false;
    int day   =  date / 1000000;
    int month = (date / 10000)   % 100;
    int year  =  date           % 10000;
    if (year < 1900 || year > 2026) return false;
    if (month < 1 || month > 12)    return false;
    if (day   < 1 || day   > 31)    return false;
    return true;
}

Ozonator::Ozonator(int id, WaterQuality& quality,
                   int access_level, bool is_good_working,
                   double availible_vol_ozon, double vol_coal)
    : ControlSystem(id, quality, access_level, is_good_working),
      availible_vol_ozon(availible_vol_ozon),
      vol_coal(vol_coal) {}
      
void Ozonator::checking() {
    if (availible_vol_ozon <= 0.0 || vol_coal <= 0.0) {
        mark_broken();
    }
}

void Ozonator::improving() {
    double current_ox = quality.get_oxygen_level();

    if (current_ox >= 5.0 && current_ox <= 10.0) return;

    // Куда тянем: ниже 5 к 5.0, выше 10 → к 10.0
    double target = (current_ox < 5.0) ? 5.0 : 10.0;
    double delta  = std::fabs(target - current_ox);

    double needed_ozon = delta * 0.5;   // на единицу oxygen — 0.5 озона
    double needed_coal = delta * 1.0;   // на единицу oxygen — 1.0 угля

    // Хватает ресурсов — доводим до границы нормы
    if (availible_vol_ozon >= needed_ozon && vol_coal >= needed_coal) {
        availible_vol_ozon -= needed_ozon;
        vol_coal           -= needed_coal;
        quality.set_oxygen_level(target);
        return;
    }

    // Не хватает — используем максимум возможного при текущем соотношении
    double max_delta_by_ozon = availible_vol_ozon / 0.5;
    double max_delta_by_coal = vol_coal           / 1.0;
    double max_delta = std::min(max_delta_by_ozon, max_delta_by_coal);

    double direction = (target > current_ox) ? 1.0 : -1.0;
    quality.set_oxygen_level(current_ox + direction * max_delta);

    availible_vol_ozon -= max_delta * 0.5;
    vol_coal           -= max_delta * 1.0;

    checking();   // один из ресурсов ушёл в 0 → mark_broken()
}

TemperatureController::TemperatureController(int id, WaterQuality& quality,
                                             int access_level, bool is_good_working,
                                             int amperage)
    : ControlSystem(id, quality, access_level, is_good_working),
      amperage(amperage) {}

void TemperatureController::checking() {
    if (amperage < 5 || amperage > 24) {
        mark_broken();
    }
}

void TemperatureController::improving() {
    checking();   // проверяем состояние датчика до работы

    double current_temp = quality.get_temperature();

    // Уже в норме — ничего не меняем
    if (current_temp >= 15.0 && current_temp <= 25.0) {
        checking();
        return;
    }

    // Вне нормы — тянем к середине диапазона
    constexpr double target_temp = 20.0;
    quality.set_temperature(target_temp);

    checking();   // проверяем, не сломался ли в процессе
}

SalinityController::SalinityController(int id, WaterQuality& quality,
                                       int access_level, bool is_good_working,
                                       double vol_water, double vol_brine)
    : ControlSystem(id, quality, access_level, is_good_working),
      vol_water(vol_water),
      vol_brine(vol_brine) {}

void SalinityController::checking() {
    if (vol_water <= 0.0 || vol_brine <= 0.0) {
        mark_broken();
    }
}

void SalinityController::improving() {
    double current_sal = quality.get_salinity();

    // Уже в норме
    if (current_sal >= 0.0 && current_sal <= 35.0) return;

    if (current_sal < 0.0) {
        // Нужно ПОВЫСИТЬ до 0.0 используем vol_brine
        double needed_delta    = 0.0 - current_sal;
        double needed_resource = needed_delta / 2.0;

        if (vol_brine >= needed_resource) {
            vol_brine -= needed_resource;
            quality.set_salinity(0.0);
            return;
        }
        // Ресурса не хватает — используем всё и ломаемся
        double possible_delta = vol_brine * 2.0;
        quality.set_salinity(current_sal + possible_delta);
        vol_brine = 0.0;
        checking();
    } else {
        // current_sal > 35.0 — нужно ПОНИЗИТЬ до 35.0 используем vol_water
        double needed_delta    = current_sal - 35.0;
        double needed_resource = needed_delta / 2.0;

        if (vol_water >= needed_resource) {
            vol_water -= needed_resource;
            quality.set_salinity(35.0);
            return;
        }
        double possible_delta = vol_water * 2.0;
        quality.set_salinity(current_sal - possible_delta);
        vol_water = 0.0;
        checking();
    }
}

WaterSensor::WaterSensor(int id, WaterQuality& quality,
                         int access_level, bool is_good_working,
                         double vol_co2, double vol_alkali)
    : ControlSystem(id, quality, access_level, is_good_working),
      vol_co2(vol_co2),
      vol_alkali(vol_alkali) {}

void WaterSensor::checking() {
    if (vol_co2 <= 0.0 || vol_alkali <= 0.0) {
        mark_broken();
    }
}

void WaterSensor::improving() {
    double current_ph = quality.get_ph();

    // Уже в норме — ничего не делаем
    if (current_ph >= 6.5 && current_ph <= 8.5) return;

    if (current_ph < 6.5) {
        // Нужно ПОВЫСИТЬ pH до 6.5 → используем vol_alkali
        double needed_delta    = 6.5 - current_ph;
        double needed_resource = needed_delta / 2.0;

        if (vol_alkali >= needed_resource) {
            vol_alkali -= needed_resource;
            quality.set_ph(6.5);
            return;
        }
        // Ресурса не хватает — используем всё и ломаемся
        double possible_delta = vol_alkali * 2.0;
        quality.set_ph(current_ph + possible_delta);
        vol_alkali = 0.0;
        checking();
    } else {
        // current_ph > 8.5 — нужно ПОНИЗИТЬ до 8.5 → используем vol_co2
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
