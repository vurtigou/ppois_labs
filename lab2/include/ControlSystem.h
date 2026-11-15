#pragma once

#include <vector>

class Employee;
class WaterQuality;

class ControlSystem {
private:
    int id;
    std::vector<Employee*> employees;   // non-owning
    int access_level;
    bool is_good_working;
    std::vector<int> repair_dates;      // ддммгггг

protected:
    WaterQuality& quality;

public:
    ControlSystem(int id, WaterQuality& quality,
                  int access_level = 0, bool is_good_working = true);
    virtual ~ControlSystem() = default;

    virtual void checking()  = 0;
    virtual void improving() = 0;

    // Хук, вызываемый после успешного ремонта — восстановить расходники/настройки.
    // По умолчанию ничего не делает.
    virtual void on_repair() {}

    int  get_id()              const { return id; }
    int  get_access_level()    const { return access_level; }
    bool get_is_good_working() const { return is_good_working; }
    const std::vector<int>& get_repair_dates() const { return repair_dates; }
    const std::vector<Employee*>& get_employees() const { return employees; }

    void assign_employee(Employee* employee);
    void release_employee(int employee_id);

    bool can_work(const Employee* employee) const;
    void perform_work(const Employee* employee, int date);

    static bool is_valid_date(int date);

protected:
    void mark_broken() { is_good_working = false; access_level = 1; }
};
