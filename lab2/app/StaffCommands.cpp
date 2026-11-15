#include "Console.h"

#include "Electrician.h"
#include "Janitor.h"
#include "Plumber.h"

#include <iostream>
#include <limits>

namespace {

int minutes(const std::string& time) {
    if (time.size() != 5 || time[2] != ':' || time.find_first_not_of("0123456789:", 0) != std::string::npos ||
        time.find(':') != 2 || time.rfind(':') != 2) {
        throw InvalidActionError("Use HH:MM for shift time.");
    }
    const int hour = std::stoi(time.substr(0, 2));
    const int minute = std::stoi(time.substr(3, 2));
    if (hour > 23 || minute > 59) throw InvalidActionError("Shift time out of range.");
    return hour * 60 + minute;
}

} // namespace

void Console::hire(const Args& args) {
    const auto id = number<int>(args[1]);
    const auto experience = number<int>(args[2]);
    if (id < 1 || experience < 0 || experience > std::numeric_limits<int>::max() / 200) {
        throw InvalidActionError("Invalid employee ID or experience.");
    }
    if (employees.count(id) != 0) throw DuplicateEntityError("Employee ID already exists.");
    std::unique_ptr<Employee> worker;
    if (args[0] == "cashier") {
        worker = std::make_unique<Cashier>(id, experience);
    } else if (args[0] == "janitor") {
        auto janitor = std::make_unique<Janitor>(id, experience);
        janitor->set_mop(&mop);
        worker = std::move(janitor);
    } else if (args[0] == "plumber") {
        auto plumber = std::make_unique<Plumber>(id, experience, 1);
        plumber->set_equipment(&wrench);
        worker = std::move(plumber);
    } else if (args[0] == "electrician") {
        auto electrician = std::make_unique<Electrician>(id, experience, 2);
        electrician->set_equipment(&tester);
        worker = std::move(electrician);
    } else {
        throw EntityNotFoundError("Unknown employee role: " + args[0]);
    }
    auto entry = employees.emplace(id, std::move(worker)).first;
    oceanarium.add_employee(entry->second.get());
}

void Console::list_staff(const Args&) {
    for (const auto& [id, worker] : employees) {
        std::cout << id << " | " << worker->get_position() << " | experience: " << worker->get_experience() << '\n';
    }
}

void Console::shift(const Args& args) {
    auto& worker = require(employees, number<int>(args[0]), "employee");
    if (minutes(args[1]) >= minutes(args[2])) throw InvalidActionError("Shift must end after it starts on the same day.");
    auto item = std::make_unique<Shift>(&worker, args[1], args[2]);
    item->generate_incidents();
    item->generate_events();
    shifts.push_back(std::move(item));
    schedule.add_shift(*shifts.back());
}

void Console::payroll(const Args& args) {
    auto& selected = require(employees, args.empty() ? 1 : number<int>(args[0]), "employee");
    schedule.calculate_monthly_salaries();
    const auto& payments = selected.get_salaries();
    std::cout << "Payments made: " << payments.size();
    if (!payments.empty()) std::cout << ", latest: " << payments.back();
    std::cout << '\n';
}

void Console::work(const Args& args) {
    const auto& action = args[0];
    if (action == "clean") static_cast<Janitor&>(*employees.at(2)).clean_area();
    else if (action == "wash") mop.wash();
    else if (action == "refill") mop.refill_water();
    else if (action == "pressure") wrench.check_pressure();
    else if (action == "leak") static_cast<Plumber&>(*employees.at(3)).repair_leak();
    else if (action == "circuits") static_cast<Electrician&>(*employees.at(4)).inspect_circuits();
    else if (action == "recharge") tester.recharge_battery();
    else if (action == "insulate") tester.replace_insulation();
    else if (action == "repair-tools") {
        mop.repair();
        wrench.repair();
        tester.repair();
    } else {
        throw InvalidActionError("Unknown work operation: " + action);
    }
}
