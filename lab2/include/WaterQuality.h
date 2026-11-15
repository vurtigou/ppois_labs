#pragma once

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
    void set_ph(double new_ph);
    void set_salinity(double new_salinity);
    void set_temperature(double new_temperature);
    void set_oxygen_level(double new_oxygen);

    bool is_within_norm() const;
};
