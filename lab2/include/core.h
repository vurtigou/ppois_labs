#pragma once

#include <string>
#include <vector>
#include <cstddef>   // std::size_t

#include "habitats.h"   // Tank
#include "staff.h"      // Employee
#include "exceptions.h" // OceanariumError

// 1. ContactInfo
class ContactInfo {
private:
    std::string phone;
    std::string email;

public:
    ContactInfo(std::string phone = "", std::string email = "");

    bool is_valid_contact() const;

    std::string get_phone() const { return phone; }
    std::string get_email() const { return email; }
};

// 2. Identifier
class Identifier {
private:
    std::string prefix;
    int number;

public:
    // start_number — номер, который будет выдан первым
    Identifier(std::string prefix = "ID", int start_number = 0);

    std::string generate_next();

    std::string get_prefix() const { return prefix; }
    int get_next_number() const { return number; }
};

// 3. Location
class Location {
private:
    std::string city;
    std::string address;
    std::string coordinates; // формат "lat,lon"

public:
    Location(std::string city = "", std::string address = "", std::string coordinates = "");

    // Haversine. Бросает std::invalid_argument, если координаты не заданы/битые.
    double calculate_distance_to(const Location& other) const;

    std::string get_city()        const { return city; }
    std::string get_address()     const { return address; }
    std::string get_coordinates() const { return coordinates; }
    bool is_valid() const { return !city.empty(); }
};

// 4. AuditLog
struct LogEntry {
    std::string timestamp; // "YYYY-MM-DDTHH:MM:SS"
    std::string action;
};

class AuditLog {
private:
    std::vector<LogEntry> entries;
    std::size_t max_size;

public:
    // Бросает std::invalid_argument при max_size == 0
    explicit AuditLog(std::size_t max_size = 1000);

    void record_action(const std::string& action);

    // Даты в формате "YYYY-MM-DD" (включительно). Пустые границы — без ограничения.
    std::vector<std::string> filter_by_period(const std::string& start_date,
                                              const std::string& end_date) const;

    void print_logs() const;

    std::size_t size() const { return entries.size(); }
};

// 5. Oceanarium
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
