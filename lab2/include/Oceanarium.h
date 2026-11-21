#pragma once

/** @defgroup core Core and audit */

/** @defgroup animals Animals and veterinary care */

/** @defgroup habitats Habitats, exhibits, and tanks */

/** @defgroup water Water quality and controls */

/** @defgroup staff Staff and payroll */

/** @defgroup inventory Equipment and food stock */

/** @defgroup visitors Visitors, tickets, and services */

/** @defgroup errors Domain exceptions */

/** @defgroup console Console application */

#include <string>
#include <vector>

#include "AuditLog.h"
#include "Location.h"

class Tank;
class Employee;

/** @brief Registers tanks and employees and tracks daily opening and closing.
 * @ingroup core
 */
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

    /**
     * @brief Registers an externally owned tank and records the addition in the audit log.
     */
    void add_tank(Tank* tank);
    /**
     * @brief Registers an externally owned employee and records the addition in the audit log.
     */
    void add_employee(Employee* employee);

    std::string const& get_name() const { return name; }
    Location  const&  get_location() const { return location; }

    const std::vector<Tank*>&     get_tanks()     const { return tanks; }
    const std::vector<Employee*>& get_employees() const { return employees; }
    const AuditLog&               get_audit_log() const { return audit_log; }
};
