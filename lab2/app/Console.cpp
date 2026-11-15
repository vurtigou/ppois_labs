#include "Console.h"

#include "AuditLog.h"
#include "ContactInfo.h"
#include "Electrician.h"
#include "Janitor.h"
#include "Location.h"
#include "Plumber.h"

#include <iomanip>
#include <iostream>

Console::Console() {
    reef.add_tank(&display);
    reserve.add_tank(&backup);
    for (ControlSystem* device : std::initializer_list<ControlSystem*>{&thermostat, &acidity, &salinity, &oxygen}) {
        display.add_sensor(device);
    }
    zone.add_exhibit(&exhibit);
    zone.add_exhibit(&service_exhibit);
    zone.add_tank(&display);
    service_exhibit.add_tank(&backup);
    service_exhibit.add_tank(&quarantine);
    map.add_zone(&zone);
    for (Tank* tank : std::initializer_list<Tank*>{&display, &backup, &quarantine}) oceanarium.add_tank(tank);
    hire({"cashier", "1", "6"});
    hire({"janitor", "2", "3"});
    hire({"plumber", "3", "5"});
    hire({"electrician", "4", "4"});
    thermostat.assign_employee(employees.at(4).get());
    shift({"1", "09:00", "18:00"});
    visitors.emplace(1, std::make_unique<Visitor>(1, "Demo visitor", 20, 100));
    stock_menu(lounge);
    DailyStatistics::set_current(&statistics);
}

Console::~Console() {
    DailyStatistics::set_current(nullptr);
}

const std::vector<Console::Command>& Console::commands() {
    static const std::vector<Command> available{
        {"help", 0, 0, "help", &Console::help},
        {"open", 0, 0, "open", &Console::open},
        {"close", 0, 0, "close", &Console::close},
        {"zone", 1, 1, "zone LEVEL", &Console::set_zone},
        {"exhibit", 2, 2, "exhibit reef|service open|close", &Console::set_exhibit},
        {"tank", 1, 1, "tank ID", &Console::find_tank},
        {"tank-move", 2, 2, "tank-move ID reef|service", &Console::move_tank},
        {"add", 2, 4, "add ID|auto KG [reef|reserve [reef-fish|lionfish]]", &Console::add},
        {"cub", 3, 3, "cub ID PARENT_ID KG", &Console::cub},
        {"animals", 0, 0, "animals", &Console::list_animals},
        {"compatibility", 2, 2, "compatibility FIRST_ID SECOND_ID", &Console::compatibility},
        {"stock", 0, 0, "stock", &Console::stock},
        {"transfer", 1, 2, "transfer ID [reef|reserve]", &Console::transfer},
        {"weigh", 2, 2, "weigh ID KG", &Console::weigh},
        {"feed", 1, 2, "feed ID [plankton|krill]", &Console::feed},
        {"vet", 2, 3, "vet ID DIAGNOSIS [quarantine]", &Console::vet},
        {"release", 2, 2, "release ID reef|reserve", &Console::release},
        {"breed", 3, 3, "breed FIRST_ID SECOND_ID DDMMYYYY", &Console::breed},
        {"water", 1, 1, "water TEMP", &Console::water},
        {"water-set", 5, 5, "water-set reef|reserve PH SALINITY TEMP OXYGEN", &Console::set_water},
        {"water-status", 0, 1, "water-status [reef|reserve]", &Console::water_status},
        {"filter", 0, 0, "filter", &Console::filter},
        {"drain", 0, 0, "drain", &Console::drain},
        {"system", 2, 4, "system temperature|ph|salinity|oxygen status|assign|release|repair [EMPLOYEE] [DDMMYYYY]", &Console::system},
        {"hire", 3, 3, "hire cashier|janitor|plumber|electrician ID EXPERIENCE", &Console::hire},
        {"staff", 0, 0, "staff", &Console::list_staff},
        {"shift", 3, 3, "shift EMPLOYEE HH:MM HH:MM", &Console::shift},
        {"payroll", 0, 1, "payroll [EMPLOYEE]", &Console::payroll},
        {"work", 1, 1, "work clean|wash|refill|pressure|leak|circuits|recharge|insulate|repair-tools", &Console::work},
        {"visitor", 4, 4, "visitor ID AGE BALANCE NAME", &Console::register_visitor},
        {"balance", 1, 1, "balance VISITOR", &Console::visitor_status},
        {"group-add", 2, 2, "group-add GROUP VISITOR", &Console::group_add},
        {"group-entry", 1, 1, "group-entry GROUP", &Console::group_entry},
        {"ticket", 1, 3, "ticket PRICE [VISITOR [CASHIER]]", &Console::ticket},
        {"service", 2, 2, "service VISITOR audio|lounge|vip", &Console::buy_service},
        {"rent", 2, 3, "rent VISITOR HEADSET [standard|vip]", &Console::rent},
        {"return-headphones", 1, 2, "return-headphones VISITOR [standard|vip]", &Console::return_headphones},
        {"menu", 0, 0, "menu", &Console::menu},
        {"order", 2, 3, "order VISITOR bread|juice|beer [VIP_LOUNGE_1_TO_6]", &Console::order},
        {"report", 0, 0, "report", &Console::report},
        {"log", 0, 2, "log [FROM [TO]] (YYYY-MM-DD; - means open bound)", &Console::log},
        {"contact", 2, 2, "contact PHONE EMAIL", &Console::contact},
        {"distance", 2, 2, "distance CITY LAT,LON", &Console::distance},
    };
    return available;
}

void Console::help(const Args&) {
    std::cout << "Oceanarium commands (quote arguments containing spaces)\n";
    for (const auto& command : commands()) std::cout << "  " << command.usage << '\n';
    std::cout << "  quit\n";
}

void Console::run(std::istream& input) {
    help({});
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream stream(line);
        std::string action;
        if (!(stream >> action)) continue;
        try {
            Args args;
            std::string argument;
            while (stream >> std::ws && !stream.eof()) {
                if (!(stream >> std::quoted(argument))) throw InvalidActionError("Unclosed quote.");
                args.push_back(argument);
            }
            if (action == "quit" && args.empty()) return;
            bool found = false;
            for (const auto& command : commands()) {
                if (action != command.name) continue;
                if (args.size() < command.min_args || args.size() > command.max_args) {
                    throw InvalidActionError(std::string("Usage: ") + command.usage);
                }
                (this->*command.apply)(args);
                found = true;
                break;
            }
            if (!found) throw InvalidActionError("Unknown command. Type help.");
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << '\n';
        }
    }
}

Habitat& Console::habitat(const std::string& name) {
    if (name == "reef") return reef;
    if (name == "reserve") return reserve;
    throw EntityNotFoundError("Unknown habitat: " + name);
}

Exhibit& Console::exhibition(const std::string& name) {
    if (name == "reef") return exhibit;
    if (name == "service") return service_exhibit;
    throw EntityNotFoundError("Unknown exhibit: " + name);
}

void Console::open(const Args&) {
    oceanarium.open_for_day();
    if (zone.get_access_level() > 0) exhibit.open_exhibit();
}

void Console::close(const Args&) {
    exhibit.close_exhibit();
    service_exhibit.close_exhibit();
    oceanarium.close_for_day();
}

void Console::set_zone(const Args& args) {
    zone.restrict_access(number<int>(args[0]));
}

void Console::set_exhibit(const Args& args) {
    Exhibit& selected = exhibition(args[0]);
    if (args[1] == "close") return selected.close_exhibit();
    if (args[1] != "open") throw InvalidActionError("Exhibit action must be open or close.");
    if (zone.get_access_level() == 0) throw InvalidActionError("The zone is closed.");
    selected.open_exhibit();
}

void Console::find_tank(const Args& args) {
    map.locate_tank(args[0]);
}

void Console::move_tank(const Args& args) {
    auto& target = exhibition(args[1]);
    auto* tank = map.locate_tank(args[0]);
    if (!tank) throw EntityNotFoundError("Unknown tank: " + args[0]);
    target.add_tank(tank);
    std::cout << "Tank " << args[0] << " now in " << target.get_title() << '\n';
}

void Console::report(const Args&) {
    zone.list_tanks();
    std::cout << "Animals in reserve: " << backup.get_animal_count()
              << "\nAnimals in quarantine: " << quarantine.get_animal_count()
              << "\nReef biomass: " << reef.calculate_biomass() << " kg"
              << "\nReserve biomass: " << reserve.calculate_biomass() << " kg\n";
    statistics.print_report();
    std::cout << "Buffet cash register: " << Buffet::get_cash_register() << '\n';
    oceanarium.get_audit_log().print_logs();
}

void Console::log(const Args& args) {
    const std::string start = !args.empty() && args[0] != "-" ? args[0] : "";
    const std::string end = args.size() == 2 && args[1] != "-" ? args[1] : "";
    const auto entries = oceanarium.get_audit_log().filter_by_period(start, end);
    std::cout << "Audit entries: " << entries.size() << '\n';
    for (const auto& entry : entries) std::cout << entry << '\n';
}

void Console::contact(const Args& args) {
    const bool valid = ContactInfo(args[0], args[1]).is_valid_contact();
    std::cout << "Contact: " << (valid ? "valid" : "invalid") << '\n';
}

void Console::distance(const Args& args) {
    const auto km = oceanarium.get_location().calculate_distance_to(Location(args[0], "", args[1]));
    std::cout << "Distance to " << args[0] << ": " << km << " km\n";
}
