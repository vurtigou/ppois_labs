#include "ContactInfo.h"  
#include "Identifier.h"   
#include "Location.h"
#include "AuditLog.h"
#include "Oceanarium.h"
#include "habitats.h"
#include "stuff.h"
#include "visitors.h"
#include "exceptions.h"    // EntityNotFoundError / InvalidActionError / DuplicateEntityError

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace {

class Console {
    std::vector<std::unique_ptr<Animal>> animals;
    Habitat reef{"Reef", 1000, WaterQuality(7.4, 30, 23, 7)};
    Habitat reserve{"Reserve", 1000, WaterQuality(7.4, 30, 23, 7)};
    Tank display{"Reef-1", &reef, "pump"};
    Tank backup{"Reserve-1", &reserve, "pump"};
    TemperatureController thermostat{1, reef.get_water_quality(), 0, true, 12};
    Exhibit exhibit{"Tropical reef"};
    Zone zone{"Main hall", 3};
    Oceanarium oceanarium{"Oceanarium", Location("Minsk")};
    Cashier cashier{1, 6};
    Visitor visitor{1, "Demo visitor", 20, 100};
    DailyStatistics statistics;
    Shift shift{&cashier, "09:00", "18:00"};
    Schedule schedule;

public:
    Console() {
        reef.add_tank(&display);
        reserve.add_tank(&backup);
        display.add_sensor(&thermostat);
        zone.add_exhibit(&exhibit);
        zone.add_tank(&display);
        oceanarium.add_tank(&display);
        oceanarium.add_tank(&backup);
        oceanarium.add_employee(&cashier);
        shift.generate_incidents();
        schedule.add_shift(shift);
        DailyStatistics::set_current(&statistics);
    }

    ~Console() { DailyStatistics::set_current(nullptr); }

    static void help() {
        std::cout << "Commands:\n"
                  << "  open | close       Open or close the oceanarium\n"
                  << "  add ID KG          Add a fish to the reef\n"
                  << "  transfer ID        Move a fish to the reserve habitat\n"
                  << "  feed ID            Calculate the feeding portion\n"
                  << "  water TEMP         Set and regulate reef water temperature\n"
                  << "  ticket PRICE       Sell a ticket to the demo visitor\n"
                  << "  payroll            Pay outstanding shifts\n"
                  << "  report             Show tanks, ticket statistics and audit log\n"
                  << "  help | quit\n";
    }

    void run(std::istream& input) {
        help();
        for (std::string line; std::getline(input, line);) {
            std::istringstream args(line);
            std::string command;
            if (!(args >> command)) continue;
            if (command == "quit") return;
            try {
                run_command(command, args);
            } catch (const std::exception& error) {
                std::cout << "Error: " << error.what() << '\n';
            }
        }
    }

private:
    Animal& find_animal(const std::string& id) {
        auto found = std::find_if(animals.begin(), animals.end(),
            [&id](const auto& animal) { return animal->get_id() == id; });
        if (found == animals.end()) throw EntityNotFoundError("Unknown animal ID " + id);
        return **found;
    }

    void add_animal(std::istream& args) {
        std::string id;
        double weight = 0.0;
        if (!(args >> id >> weight) || weight <= 0.0) throw InvalidActionError("Usage: add ID KG (positive weight)");
        if (reef.contains_animal(id) || reserve.contains_animal(id)) throw DuplicateEntityError("Animal ID already exists");
        animals.push_back(std::make_unique<Animal>(id, Species("reef fish", "plankton", 150), weight));
        try {
            reef.add_animal(animals.back().get());
        } catch (...) {
            animals.pop_back();
            throw;
        }
    }

    void run_command(const std::string& command, std::istream& args) {
        if (command == "add") return add_animal(args);
        if (command == "transfer" || command == "feed") {
            std::string id;
            if (!(args >> id)) throw InvalidActionError("Animal ID is required.");
            auto& animal = find_animal(id);
            if (command == "transfer") {
                AnimalTransfer(&animal, &reserve).approve_transfer();
            } else {
                auto& plan = animal.get_feeding_plan();
                plan.calculate_portion();
                std::cout << "Feeding portion: " << plan.get_portion_grams() << " g\n";
            }
        } else if (command == "water") {
            double temperature = 0.0;
            if (!(args >> temperature)) throw InvalidActionError("Usage: water TEMP");
            reef.get_water_quality().set_temperature(temperature);
            display.start_filtration();
            std::cout << "Water temperature: " << reef.get_water_quality().get_temperature() << " C\n";
        } else if (command == "ticket") {
            int price = 0;
            if (!(args >> price) || price < 0) throw InvalidActionError("Usage: ticket PRICE (nonnegative integer)");
            cashier.sell_ticket(visitor, std::make_unique<Ticket>(price));
        } else if (command == "payroll") {
            schedule.calculate_monthly_salaries();
            const auto& payments = cashier.get_salaries();
            std::cout << "Payments made: " << payments.size() << ", latest: " << payments.back() << '\n';
        } else if (command == "open") {
            oceanarium.open_for_day();
            exhibit.open_exhibit();
        } else if (command == "close") {
            exhibit.close_exhibit();
            oceanarium.close_for_day();
        } else if (command == "report") {
            zone.list_tanks();
            std::cout << "Animals in reserve: " << backup.get_animal_count() << '\n';
            statistics.print_report();
            oceanarium.get_audit_log().print_logs();
        } else if (command == "help") {
            help();
        } else {
            throw InvalidActionError("Unknown command. Type help.");
        }
    }
};

} // namespace

int main() {
    Console console;
    console.run(std::cin);
}
