#pragma once

#include "Animal.h"
#include "AudioGuide.h"
#include "BreedingProgram.h"
#include "Cashier.h"
#include "DailyStatistics.h"
#include "ElectricianEquipment.h"
#include "Exhibit.h"
#include "FeedStock.h"
#include "GroupVisit.h"
#include "Habitat.h"
#include "HabitatMap.h"
#include "Headphones.h"
#include "Identifier.h"
#include "Lounge.h"
#include "Mop.h"
#include "AlcoholDrinks.h"
#include "Drinks.h"
#include "Food.h"
#include "Oceanarium.h"
#include "Ozonator.h"
#include "PlumberEquipment.h"
#include "QuarantineTank.h"
#include "SalinityController.h"
#include "Schedule.h"
#include "Shift.h"
#include "TemperatureController.h"
#include "VipVisit.h"
#include "Visitor.h"
#include "WaterSensor.h"
#include "Zone.h"
#include "exceptions.h"

#include <cmath>
#include <iosfwd>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class Console {
public:
    Console();
    ~Console();
    void run(std::istream& input);

private:
    using Args = std::vector<std::string>;
    struct Command {
        const char* name;
        std::size_t min_args;
        std::size_t max_args;
        const char* usage;
        void (Console::*apply)(const Args&);
    };

    static const std::vector<Command>& commands();
    void help(const Args&);

    template<class Number>
    static Number number(const std::string& text) {
        Number value{};
        std::istringstream input(text);
        if (!(input >> value) || input.peek() != std::char_traits<char>::eof() || !std::isfinite(value)) {
            throw InvalidActionError("Invalid number: " + text);
        }
        return value;
    }

    template<class Key, class Value>
    static Value& require(const std::map<Key, std::unique_ptr<Value>>& objects, const Key& key, const char* category) {
        auto entry = objects.find(key);
        if (entry == objects.end()) {
            std::ostringstream message;
            message << "Unknown " << category << ": " << key;
            throw EntityNotFoundError(message.str());
        }
        return *entry->second;
    }

    Identifier animal_ids{"fish", 1};
    Species reef_fish{"reef fish", "plankton", 150};
    Species lionfish{"lionfish", "reef fish,krill", 300};
    FeedStock plankton{"plankton", 25};
    FeedStock krill{"krill", 10};
    std::map<std::string, std::unique_ptr<Animal>> animals;
    BreedingProgram reef_breeding{reef_fish};
    BreedingProgram lionfish_breeding{lionfish};

    Habitat reef{"Reef", 1000, WaterQuality(7.4, 30, 23, 7)};
    Habitat reserve{"Reserve", 1000, WaterQuality(7.4, 30, 23, 7)};
    Tank display{"Reef-1", &reef, "pump"};
    Tank backup{"Reserve-1", &reserve, "pump"};
    QuarantineTank quarantine{"Quarantine-1"};
    TemperatureController thermostat{1, reef.get_water_quality(), 1, true, 12};
    WaterSensor acidity{2, reef.get_water_quality(), 1, true, 10, 10};
    SalinityController salinity{3, reef.get_water_quality(), 1, true, 10, 10};
    Ozonator oxygen{4, reef.get_water_quality(), 1, true, 10, 10};
    Exhibit exhibit{"Tropical reef"};
    Exhibit service_exhibit{"Service tanks"};
    Zone zone{"Main hall", 3};
    HabitatMap map;
    Oceanarium oceanarium{"Oceanarium", Location("Minsk", "", "53.9,27.5667")};

    Mop mop;
    PlumberEquipment wrench;
    ElectricianEquipment tester;
    std::map<int, std::unique_ptr<Employee>> employees;
    std::vector<std::unique_ptr<Shift>> shifts;
    Schedule schedule;

    Food bread{3, "bread", 150};
    Drinks juice{4, 330};
    AlcoholDrinks beer{6, 330, 5};
    std::map<std::string, Headphones> headsets{{"1", {}}, {"2", {}}};
    std::map<int, std::unique_ptr<Visitor>> visitors;
    std::map<int, std::unique_ptr<GroupVisit>> groups;
    std::set<int> admitted_groups;
    AudioGuide audio{3};
    Lounge lounge{5};
    VipVisit vip{12};
    DailyStatistics statistics;

    Habitat& habitat(const std::string& name);
    Exhibit& exhibition(const std::string& name);
    ControlSystem& controller(const std::string& name);
    Visitor& visitor(const std::string& id);
    AudioGuide& guide_for(Visitor& guest, const std::string& kind);
    Buffet& product(const std::string& name);
    void stock_menu(Lounge& room);

    void place(std::unique_ptr<Animal> animal, Habitat& home);
    void add(const Args&);
    void cub(const Args&);
    void list_animals(const Args&);
    void compatibility(const Args&);
    void stock(const Args&);
    void transfer(const Args&);
    void weigh(const Args&);
    void feed(const Args&);
    void vet(const Args&);
    void release(const Args&);
    void breed(const Args&);

    void open(const Args&);
    void close(const Args&);
    void set_zone(const Args&);
    void set_exhibit(const Args&);
    void find_tank(const Args&);
    void move_tank(const Args&);
    void water(const Args&);
    void set_water(const Args&);
    void water_status(const Args&);
    void filter(const Args&);
    void drain(const Args&);
    void system(const Args&);
    void report(const Args&);
    void log(const Args&);
    void contact(const Args&);
    void distance(const Args&);

    void hire(const Args&);
    void list_staff(const Args&);
    void shift(const Args&);
    void payroll(const Args&);
    void work(const Args&);

    void register_visitor(const Args&);
    void visitor_status(const Args&);
    void group_add(const Args&);
    void group_entry(const Args&);
    void ticket(const Args&);
    void buy_service(const Args&);
    void rent(const Args&);
    void return_headphones(const Args&);
    void menu(const Args&);
    void order(const Args&);
};
