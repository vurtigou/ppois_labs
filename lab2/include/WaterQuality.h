#pragma once

/** @brief Validated pH, salinity, temperature, and oxygen measurements.
 * @ingroup water
 */
class WaterQuality {
private:
    double ph;
    double salinity;
    double temperature;
    double oxygen_level;

public:
    WaterQuality(double ph = 0.0, double salinity = 0.0,
                 double temperature = 0.0, double oxygen_level = 0.0);

    double get_ph()           const { return ph; }
    double get_salinity()     const { return salinity; }
    double get_temperature()  const { return temperature; }
    double get_oxygen_level() const { return oxygen_level; }

    // Сеттеры с валидацией — определены в water.cpp
    /**
     * @brief Sets a finite pH value from 0 to 14 inclusive.
     */
    void set_ph(double new_ph);
    /**
     * @brief Sets a finite salinity value from 0 to 45 inclusive.
     */
    void set_salinity(double new_salinity);
    /**
     * @brief Sets water temperature from -2 to 40 degrees Celsius inclusive.
     */
    void set_temperature(double new_temperature);
    /**
     * @brief Sets a finite oxygen value from 0 to 20 inclusive.
     */
    void set_oxygen_level(double new_oxygen);

    /**
     * @brief Checks the model’s narrower operating ranges for all four measurements.
     */
    bool is_within_norm() const;
};
