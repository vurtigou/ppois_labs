#pragma once

#include <string>
#include <vector>

#include "AuditLog.h"
#include "Location.h"

class Tank;
class Employee;

class Oceanarium {
private:
    std::string name;
    bool is_open;
    Location location;
    std::vector<Tank*>     tanks;      // non-owning
    std::vector<Employee*> employees;  // non-owning
    AuditLog audit_log;

public:
    Oceanarium(std::string name, const Location& location);
    ~Oceanarium();

    Oceanarium(const Oceanarium&)            = delete;
    Oceanarium& operator=(const Oceanarium&) = delete;
    Oceanarium(Oceanarium&&)                 = default;
    Oceanarium& operator=(Oceanarium&&)      = default;

    void open_for_day();
    void close_for_day();

    void add_tank(Tank* tank);
    void add_employee(Employee* employee);

    std::string get_name() const { return name; }
    Location    get_location() const { return location; }

    const std::vector<Tank*>&     get_tanks()     const { return tanks; }
    const std::vector<Employee*>& get_employees() const { return employees; }
    const AuditLog&               get_audit_log() const { return audit_log; }
};
