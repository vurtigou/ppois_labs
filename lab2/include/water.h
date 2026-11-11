#pragma once

#include <vector>
#include <string>

#include "Employee.h"

class WaterQuality;
class WaterSensor;
class SalinityController;
class TemperatureController;
class Ozonator;

class WaterQuality {
private:
    double ph;
    double salinity;
    double temperature;
    double oxygen_level;
public:
    WaterQuality(double ph = 0.0, double salinity = 0.0,
                 double temperature = 0.0, double oxygen_level = 0.0);

    double get_ph()            const { return ph; }
    double get_salinity()      const { return salinity; }
    double get_temperature()   const { return temperature; }
    double get_oxygen_level()  const { return oxygen_level; }
    void set_ph(double new_ph) { ph = new_ph; }
    void set_salinity(double new_salinity) { salinity = new_salinity; }
    void set_temperature(double new_temperature) { temperature = new_temperature; }
    void set_oxygen_level(double new_oxygen) { oxygen_level = new_oxygen; }
    //проверка на соответствие норме
    bool is_within_norm() const;
};

class ControlSystem {
private:
    int id;
    std::vector<Employee*> employees;   // non-owning
    int access_level;  //0-для корректно работающего, 1-для поломанного
    bool is_good_working;
    std::vector<int> repair_dates;      // даты в формате ддммгггг
    WaterQuality &quality;

public:
    ControlSystem(int id, WaterQuality& quality,
                  int access_level = 0, bool is_good_working = true);
    virtual ~ControlSystem() = default;

    // чистые виртуальные — реализуют дочерние классы
    virtual void checking() = 0;
    virtual void improving() = 0;

    // геттеры
    int  get_id()             const { return id; }
    int  get_access_level()   const { return access_level; }
    bool get_is_good_working()const { return is_good_working; }
    const std::vector<int>& get_repair_dates() const { return repair_dates; }
    const std::vector<Employee*>& get_employees() const { return employees; }

    // добавить работника в вектор
    void assign_employee(Employee* employee);
    //удалить работника из вектора
    void release_employee(int employee_id);

    bool can_work(const Employee* employee) const;

    void perform_work(const Employee* employee, int date);

    // утилита
    static bool is_valid_date(int date);
protected:
    void mark_broken() { is_good_working = false; access_level = 1; }
};

//pH
class WaterSensor : public ControlSystem { 
private:
    double vol_co2;
    double vol_alkali;
public:
        WaterSensor(int id, WaterQuality& quality,
                int access_level = 0, bool is_good_working = true,
                double vol_co2 = 0.0, double vol_alkali = 0.0);
    void checking() override;
    void improving() override;
};

//Sality
class SalinityController : public ControlSystem {
private:
    double vol_water;
    double vol_brine;
public:
    SalinityController(int id, WaterQuality& quality,
                       int access_level = 0, bool is_good_working = true,
                       double vol_water = 0.0, double vol_brine = 0.0);
    void checking() override;
    void improving() override;
};

//temperature
class TemperatureController : public ControlSystem {
private:
    int amperage;
public:
        TemperatureController(int id, WaterQuality& quality,
                          int access_level = 0, bool is_good_working = true,
                          int amperage = 0);

    void checking() override;
    void improving() override;
};

//oxygen_level
class Ozonator : public ControlSystem {
private:
    double availible_vol_ozon;   // было int
    double vol_coal;             // было int
public:
    Ozonator(int id, WaterQuality& quality,
             int access_level = 0, bool is_good_working = true,
             double availible_vol_ozon = 0.0, double vol_coal = 0.0);
    void checking() override;
    void improving() override;
};
