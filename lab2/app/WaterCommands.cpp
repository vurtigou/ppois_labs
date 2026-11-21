#include "Console.h"

#include <iostream>

ControlSystem& Console::controller(const std::string& name) {
    if (name == "temperature") return thermostat;
    if (name == "ph") return acidity;
    if (name == "salinity") return salinity;
    if (name == "oxygen") return oxygen;
    throw EntityNotFoundError("Unknown system: " + name);
}

void Console::water(const Args& args) {
    reef.get_water_quality().set_temperature(number<double>(args[0]));
    filter({});
}

void Console::set_water(const Args& args) {
    auto& quality = habitat(args[0]).get_water_quality();
    quality = WaterQuality(number<double>(args[1]), number<double>(args[2]),
                           number<double>(args[3]), number<double>(args[4]));
    water_status({args[0]});
}

void Console::water_status(const Args& args) {
    const auto& quality = habitat(args.empty() ? "reef" : args[0]).get_water_quality();
    std::cout << "Water temperature: " << quality.get_temperature() << " C"
              << " | pH: " << quality.get_ph()
              << " | salinity: " << quality.get_salinity()
              << " | oxygen: " << quality.get_oxygen_level()
              << " | " << (quality.is_within_norm() ? "within limits" : "out of range") << '\n';
}

void Console::filter(const Args&) {
    display.start_filtration();
    water_status({});
}

void Console::drain(const Args&) {
    display.emergency_drain(&reserve);
    std::cout << "Reef animals remaining: " << display.get_animal_count() << '\n';
}

void Console::system(const Args& args) {
    auto& device = controller(args[0]);
    const auto& action = args[1];
    if (action == "status" && args.size() == 2) {
        device.checking();
        std::cout << args[0] << ": " << (device.get_is_good_working() ? "working" : "out of service")
                  << " | minimum access: " << device.get_access_level()
                  << " | assigned: " << device.get_employees().size()
                  << " | repairs: " << device.get_repair_dates().size() << '\n';
    } else if (action == "assign" && args.size() == 3) {
        device.assign_employee(&require(employees, number<int>(args[2]), "employee"));
    } else if (action == "release" && args.size() == 3) {
        device.release_employee(number<int>(args[2]));
    } else if (action == "repair" && args.size() == 4) {
        auto& worker = require(employees, number<int>(args[2]), "employee");
        device.perform_work(&worker, number<int>(args[3]));
        std::cout << "Maintenance recorded: " << args[0] << '\n';
    } else {
        throw InvalidActionError("Use status, assign EMPLOYEE, release EMPLOYEE or repair EMPLOYEE DDMMYYYY.");
    }
}
