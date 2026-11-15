// --- core ---
#include "AuditLog.h"
#include "ContactInfo.h"
#include "Identifier.h"
#include "Location.h"
#include "Oceanarium.h"

// --- biology ---
#include "Animal.h"
#include "AnimalTransfer.h"
#include "BreedingProgram.h"
#include "Cub.h"
#include "FeedingPlan.h"
#include "HealthRecord.h"
#include "Species.h"
#include "VetVisit.h"

// --- habitats ---
#include "Exhibit.h"
#include "Habitat.h"
#include "HabitatMap.h"
#include "QuarantineTank.h"
#include "Tank.h"
#include "Zone.h"

// --- water ---
#include "ControlSystem.h"
#include "Ozonator.h"
#include "SalinityController.h"
#include "TemperatureController.h"
#include "WaterQuality.h"
#include "WaterSensor.h"

// --- staff / equipment ---
#include "Cashier.h"
#include "Electrician.h"
#include "ElectricianEquipment.h"
#include "EmployeesEquipment.h"
#include "Janitor.h"
#include "Mop.h"
#include "Plumber.h"
#include "PlumberEquipment.h"
#include "Schedule.h"
#include "Shift.h"

// --- inventory ---
#include "AlcoholDrinks.h"
#include "Buffet.h"
#include "Drinks.h"
#include "FeedStock.h"
#include "Food.h"
#include "Headphones.h"

// --- visitors ---
#include "AudioGuide.h"
#include "DailyStatistics.h"
#include "Favour.h"
#include "GroupVisit.h"
#include "Lounge.h"
#include "Ticket.h"
#include "VipVisit.h"
#include "Visitor.h"

// --- misc ---
#include "exceptions.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

template<class Exception, class Action>
void expect_error(Action action, const std::string& message) {
    try {
        action();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error(message);
}

void check_near(double actual, double expected, const std::string& message) {
    check(std::abs(actual - expected) < 1e-9, message);
}

void test_core() {
    ContactInfo contact("+375 (29) 123-45-67", "info@example.org");
    check(contact.is_valid_contact() && !contact.get_email().empty(), "Valid contact");
    check(!ContactInfo("123abc", "info@example.org").is_valid_contact(), "Invalid phone");
    check(!ContactInfo("123456", "info@example").is_valid_contact(), "Invalid email");

    Identifier sequence("FISH", 3);
    check(sequence.generate_next() == "FISH-3" && sequence.generate_next() == "FISH-4", "Sequential IDs");

    const Location minsk("Minsk", "Center", "53.9,27.5667");
    const Location same("Minsk", "", "53.9,27.5667");
    check_near(minsk.calculate_distance_to(same), 0.0, "Identical coordinates");
    check(minsk.calculate_distance_to(Location("Warsaw", "", "52.23,21.01")) > 400, "Distance between cities");
    expect_error<std::invalid_argument>([&] { minsk.calculate_distance_to(Location("No coordinates")); }, "Missing coordinates");
    expect_error<std::invalid_argument>([&] { minsk.calculate_distance_to(Location("Bad", "", "53.9")); }, "Missing longitude");
    expect_error<std::invalid_argument>([&] { minsk.calculate_distance_to(Location("Bad", "", "bad,2")); }, "Invalid latitude");

    expect_error<std::invalid_argument>([] { AuditLog log(0); }, "Audit log capacity must be positive");
    AuditLog log(2);
    log.record_action("old");
    log.record_action("kept");
    log.record_action("last");
    const auto all = log.filter_by_period("", "");
    check(all.size() == 2 && all[0].find("kept") != std::string::npos, "Audit log retains latest entries");
    check(log.filter_by_period("9999-01-01", "").empty(), "Audit lower date bound");
    check(log.filter_by_period("", "0001-01-01").empty(), "Audit upper date bound");

    expect_error<OceanariumError>([&] { Oceanarium ocean("", minsk); }, "Empty oceanarium name");
    expect_error<OceanariumError>([] { Oceanarium ocean("Ocean", Location{}); }, "Empty oceanarium city");
    Tank tank("tank");
    Cashier cashier(1, 2);
    Oceanarium ocean("Ocean", minsk);
    expect_error<OceanariumError>([&] { ocean.add_tank(nullptr); }, "Null tank");
    expect_error<OceanariumError>([&] { ocean.add_employee(nullptr); }, "Null employee");
    ocean.add_tank(&tank);
    ocean.add_employee(&cashier);
    expect_error<OceanariumError>([&] { ocean.add_tank(&tank); }, "Duplicate tank");
    expect_error<OceanariumError>([&] { ocean.add_employee(&cashier); }, "Duplicate employee");
    check(ocean.get_tanks().size() == 1 && ocean.get_employees().size() == 1, "Registered tank and employee");
}

void test_biology() {
    const Species fish("fish", "flakes", 100);
    const Species shark("shark", "fish");
    check(fish.is_compatible_with_species(fish), "Same species are compatible here");
    check(!fish.is_compatible_with_species(shark), "Predator and prey are incompatible");

    Animal animal("", fish, 10);
    animal.set_id("a");
    expect_error<InvalidActionError>([&] { animal.set_id("b"); }, "Assigned ID is immutable");
    expect_error<InvalidActionError>([&] { animal.record_weight_change(-1); }, "Negative weight");
    expect_error<NullPointerError>([] { FeedingPlan().calculate_portion(); }, "Unassigned feeding plan");
    expect_error<OceanariumError>([] { FeedingPlan().adjust_portion(0); }, "Invalid previous weight");
    expect_error<NullPointerError>([] { FeedingPlan().check_diet_compliance(); }, "No diet without an animal");

    FeedStock flakes("flakes", 1);
    FeedStock plankton("plankton", 2);
    FeedingPlan plan;
    plan.add_feed_item(nullptr);
    plan.add_feed_item(&flakes);
    animal.assign_feeding_plan(plan);
    auto& feeding = animal.get_feeding_plan();
    check(feeding.check_diet_compliance(), "Suitable food");
    feeding.calculate_portion();
    check_near(feeding.get_portion_grams(), 500, "10 kg at 5 percent is 500 g");
    feeding.add_feed_item(&plankton);
    check(!feeding.check_diet_compliance(), "Unsuitable food");
    animal.record_weight_change(12);
    check_near(feeding.get_portion_grams(), 600, "Weight increases the existing portion proportionally");
    feeding.calculate_portion();
    check_near(feeding.get_portion_grams(), 1200, "12 kg at 10 percent is 1200 g");

    Cub cub("parent", fish, 2);
    cub.get_feeding_plan().calculate_portion();
    check_near(cub.get_feeding_plan().get_portion_grams(), 60, "Cub has a separate feeding rate");
    cub.record_weight_change(2.06);
    check(cub.get_age() == 1, "Cub records a routine check for a small weight change");
    cub.record_weight_change(3);
    check(cub.get_age() == 2, "Cub records an abnormal weight change once");
    check(!cub.can_breed(), "Cub is excluded from breeding");
    Cub named("mini", animal, 2);
    check(named.get_id() == "mini" && named.get_species().get_latin_name() == animal.get_species().get_latin_name(),
          "Named cub stores the requested ID and inherits its parent's species");

    HealthRecord standalone;
    standalone.status_diagnosis();
    expect_error<NullPointerError>([&] { standalone.add_visit(nullptr); }, "Null vet visit");
    VetVisit healthy;
    healthy.diagnostic("Healthy");
    healthy.close_visit();
    expect_error<InvalidActionError>([&] { healthy.close_visit(); }, "Vet visit cannot close twice");
    VetVisit incomplete;
    expect_error<OceanariumError>([&] { incomplete.close_visit(); }, "Unfinished vet visit");
    standalone.add_visit(std::make_unique<VetVisit>(nullptr, "Healthy"));
    standalone.status_diagnosis();

    BreedingProgram breeding(fish);
    Animal second("b", fish);
    Animal predator("s", shark);
    expect_error<NullPointerError>([&] { breeding.select_pair(nullptr, &second); }, "Null parent");
    expect_error<InvalidActionError>([&] { breeding.select_pair(&cub, &second); }, "Immature parent");
    expect_error<InvalidActionError>([&] { breeding.select_pair(&animal, &animal); }, "A pair requires two distinct animals");
    expect_error<SpeciesMismatchError>([&] { breeding.select_pair(&predator, &second); }, "Parents of different species");
    breeding.select_pair(&animal, &second);
    breeding.schedule_breeding("2026-12-01");
}

void test_habitats() {
    const WaterQuality clean(7, 30, 20, 7);
    Habitat reef("reef", 500, clean);
    Habitat reserve("reserve", 700, clean);
    Habitat adverse("adverse", 500, WaterQuality{});
    Habitat small("small", 25, clean);
    Habitat empty("empty", 500, clean);
    Tank a("A", &reef, "pump");
    Tank b("B", &reserve, "pump");
    Tank c("C", &adverse, "pump");
    Tank isolated("I");
    reef.add_tank(&a);
    reef.add_tank(&a);
    reserve.add_tank(&b);
    adverse.add_tank(&c);
    expect_error<NullPointerError>([&] { reef.add_tank(nullptr); }, "Null habitat tank");
    expect_error<OceanariumError>([&] { reef.add_tank(&b); }, "Tank belongs to another habitat");

    const Species species("fish", "flakes", 100);
    Animal fish("fish", species, 10);
    expect_error<NullPointerError>([&] { reef.add_animal(nullptr); }, "Null animal");
    expect_error<UnsuitableHabitatError>([&] { adverse.add_animal(&fish); }, "Unsuitable water prevents admission");
    expect_error<UnsuitableHabitatError>([&] { small.add_animal(&fish); }, "Insufficient volume prevents admission");
    expect_error<OceanariumError>([&] { empty.add_animal(&fish); }, "Habitat has no tanks");

    check(reef.add_animal(&fish) == &fish && fish.get_current_tank() == &a, "Initial admission updates both sides");
    check_near(reef.calculate_biomass(), 10, "Biomass is summed");
    expect_error<DuplicateEntityError>([&] { reef.add_animal(&fish); }, "Duplicate fish");
    expect_error<NullPointerError>([] { AnimalTransfer().approve_transfer(); }, "Incomplete transfer");
    expect_error<InvalidActionError>([&] { AnimalTransfer(&fish, &reef).approve_transfer(); }, "Transfer to current habitat");
    expect_error<UnsuitableHabitatError>([&] { AnimalTransfer(&fish, &adverse).approve_transfer(); }, "Transfer validates water");
    expect_error<OceanariumError>([&] { AnimalTransfer(&fish, &empty).approve_transfer(); }, "Transfer rolls back on insufficient capacity");
    check(reef.contains_animal("fish") && fish.get_current_tank() == &a, "Fish remains in its old tank after rejected transfer");
    AnimalTransfer(&fish, &reserve).approve_transfer();
    check(!reef.contains_animal("fish") && reserve.contains_animal("fish"), "Successful transfer updates both habitats");

    Cub cub("fish", species, 1);
    reef.add_animal(&cub);
    check(cub.get_habitat() == &reef, "Cub can be placed initially");
    expect_error<InvalidActionError>([&] { AnimalTransfer(&cub, &reserve).approve_transfer(); }, "Cub cannot transfer after admission");
    expect_error<InvalidActionError>([&] { cub.move_to_habitat(&reserve); }, "Cub rejects a direct transfer as well");

    Exhibit exhibit("reef");
    Exhibit second("annex");
    Zone zone("public", 3);
    expect_error<OceanariumError>([&] { zone.add_tank(&a); }, "Zone needs an exhibit");
    expect_error<NullPointerError>([&] { zone.add_tank(nullptr); }, "Null zone tank");
    expect_error<NullPointerError>([&] { zone.add_exhibit(nullptr); }, "Null exhibit");
    zone.add_exhibit(&exhibit);
    zone.add_exhibit(&second);
    expect_error<DuplicateEntityError>([&] { zone.add_exhibit(&exhibit); }, "Duplicate exhibit");
    expect_error<NullPointerError>([&] { exhibit.add_tank(nullptr); }, "Null exhibit tank");
    zone.add_tank(&a);
    zone.add_tank(&a);
    zone.add_tank(&isolated);
    check(exhibit.get_all_animals().size() == 1, "Exhibit collects its animals");
    second.add_tank(&a);
    check(exhibit.get_all_animals().empty() && a.get_exhibit() == &second, "Moving tanks between exhibits");
    check(!exhibit.remove_tank(&b), "Cannot remove a missing tank");
    exhibit.open_exhibit();
    expect_error<OceanariumError>([&] { zone.restrict_access(4); }, "Invalid access level");
    zone.restrict_access(0);
    check(!exhibit.is_open(), "A closed zone closes its exhibits");

    HabitatMap map;
    expect_error<NullPointerError>([&] { map.add_zone(nullptr); }, "Null map zone");
    map.add_zone(&zone);
    expect_error<DuplicateEntityError>([&] { map.add_zone(&zone); }, "Duplicate map zone");
    check(map.locate_tank("A") == &a && map.locate_tank("I") == &isolated, "Locate display and isolated tank");
    check(map.locate_tank("missing") == nullptr, "Unknown tank on map");
    zone.list_tanks();
    zone.remove_tank(&a, &reserve);
    check(a.get_exhibit() == &second, "Occupied tank cannot be detached after failed evacuation");
    reef.remove_animal(cub.get_id());
    Animal another("other", species, 2);
    reef.add_animal(&another);
    zone.remove_tank(&a, &reserve);
    check(a.get_exhibit() == nullptr && reserve.contains_animal("other"), "Successful evacuation before removing a tank");

    QuarantineTank quarantine("Q");
    expect_error<NullPointerError>([&] { quarantine.admit_animal(nullptr); }, "Null quarantine animal");
    VetVisit visit(&fish);
    visit.diagnostic("Needs observation", &quarantine);
    visit.close_visit();
    check(!reserve.contains_animal("fish") && quarantine.contains_animal("fish"), "Vet sends fish into quarantine");
    quarantine.admit_animal(&fish);
    check(quarantine.release_after_check("fish") == &fish && fish.get_current_tank() == nullptr, "Release clears tank reference");
    expect_error<EntityNotFoundError>([&] { quarantine.release_after_check("fish"); }, "Cannot discharge twice");
    expect_error<EntityNotFoundError>([&] { quarantine.release_after_check("fish", reserve); },
                                       "Cannot move an animal that is not in quarantine");
    quarantine.admit_animal(&fish);
    expect_error<UnsuitableHabitatError>([&] { quarantine.release_after_check("fish", adverse); },
                                         "Unsuitable destination is rejected on release");
    check(quarantine.contains_animal("fish") && fish.get_current_tank() == &quarantine,
          "Failed release leaves the animal in quarantine");
    check(quarantine.release_after_check("fish", reserve) == &fish &&
          !quarantine.contains_animal("fish") && reserve.contains_animal("fish") && fish.get_current_tank() == &b,
          "Successful release transfers the animal into the destination tank");

    expect_error<OceanariumError>([&] { isolated.start_filtration(); }, "Missing filtration equipment");
    Tank no_habitat("no habitat", nullptr, "pump");
    expect_error<OceanariumError>([&] { no_habitat.start_filtration(); }, "Filtration requires a habitat");
    expect_error<NullPointerError>([&] { b.emergency_drain(nullptr); }, "Emergency requires a backup habitat");
    check(!b.is_water_critical(), "Safe water is not critical");
    b.emergency_drain(&reef);
    reserve.get_water_quality().set_ph(4);
    expect_error<OceanariumError>([&] { b.emergency_drain(&empty); }, "Emergency drain aborts when no backup tank is available");
    check(reserve.contains_animal("fish") && fish.get_current_tank() == &b, "Failed evacuation leaves the fish in its original tank");
    b.emergency_drain(&reef);
    check(reef.contains_animal("fish") && b.get_animal_count() == 0, "Emergency evacuates all animals");
}

void test_water() {
    WaterQuality water(7, 25, 20, 7);
    check(water.is_within_norm(), "Water initially within limits");
    expect_error<WaterQualityViolationError>([&] { water.set_ph(15); }, "Invalid pH");
    expect_error<WaterQualityViolationError>([&] { water.set_salinity(-1); }, "Invalid salinity");
    expect_error<WaterQualityViolationError>([&] { water.set_temperature(-3); }, "Invalid temperature");
    expect_error<WaterQualityViolationError>([&] { water.set_oxygen_level(21); }, "Invalid oxygen");
    expect_error<WaterQualityViolationError>([] { WaterQuality bad(std::numeric_limits<double>::quiet_NaN()); }, "Non-finite water values rejected");

    check(!ControlSystem::is_valid_date(31022026), "February does not have 31 days");
    check(!ControlSystem::is_valid_date(29022026), "Common year has no February 29");
    check(ControlSystem::is_valid_date(29022028), "Leap year accepts February 29");
    check(!ControlSystem::is_valid_date(29022100), "Non-400 century is not a leap year");
    check(ControlSystem::is_valid_date(29022000), "400-year century is a leap year");
    check(!ControlSystem::is_valid_date(32122026) && !ControlSystem::is_valid_date(1010101), "Date bounds");

    Electrician technician(1, 2, 2);
    Cashier cashier(2, 2);
    TemperatureController thermostat(1, water, 1, true, 0);
    check(!thermostat.can_work(nullptr) && !thermostat.can_work(&technician), "Requires an assigned employee");
    expect_error<NullPointerError>([&] { thermostat.assign_employee(nullptr); }, "Cannot assign null");
    thermostat.assign_employee(&technician);
    thermostat.assign_employee(&cashier);
    expect_error<DuplicateEntityError>([&] { thermostat.assign_employee(&technician); }, "Cannot assign twice");
    expect_error<NullPointerError>([&] { thermostat.perform_work(nullptr, 1012026); }, "Null repair worker");
    expect_error<EmployeePermissionDeniedError>([&] { thermostat.perform_work(&cashier, 1012026); }, "Insufficient permission");
    expect_error<InvalidDateError>([&] { thermostat.perform_work(&technician, 31022026); }, "Invalid repair date");
    water.set_temperature(30);
    thermostat.improving();
    check_near(water.get_temperature(), 30, "Broken thermostat does not change the water");
    thermostat.perform_work(&technician, 1012026);
    thermostat.improving();
    check_near(water.get_temperature(), 20, "Thermostat works after repair");
    check(thermostat.get_repair_dates().size() == 1, "Repair date recorded");
    expect_error<DuplicateEntityError>([&] { thermostat.perform_work(&technician, 1012026); }, "Same-day duplicate repair");
    thermostat.release_employee(2);
    expect_error<EntityNotFoundError>([&] { thermostat.release_employee(2); }, "Unknown worker");

    WaterSensor ph(2, water, 0, true, 10, 10);
    ph.improving();
    water.set_ph(4);
    ph.improving();
    check_near(water.get_ph(), 6.5, "Raise low pH");
    water.set_ph(12);
    ph.improving();
    check_near(water.get_ph(), 8.5, "Lower high pH");
    WaterSensor scarce_ph(3, water, 0, true, 1, 1);
    water.set_ph(2);
    scarce_ph.improving();
    check_near(water.get_ph(), 4, "Low pH partially corrected with limited resources");
    scarce_ph.improving();
    scarce_ph.assign_employee(&technician);
    scarce_ph.perform_work(&technician, 1012026);
    water.set_ph(14);
    scarce_ph.improving();
    check_near(water.get_ph(), 12, "High pH partially corrected with limited resources");

    SalinityController salt(4, water, 0, true, 10, 10);
    salt.improving();
    water.set_salinity(43);
    salt.improving();
    check_near(water.get_salinity(), 35, "Dilute salty water");
    SalinityController scarce_salt(5, water, 0, true, 1, 1);
    water.set_salinity(43);
    scarce_salt.improving();
    check_near(water.get_salinity(), 41, "Partially dilute salty water");
    scarce_salt.improving();
    scarce_salt.assign_employee(&technician);
    scarce_salt.perform_work(&technician, 1012026);

    Ozonator oxygen(6, water, 0, true, 10, 10);
    oxygen.improving();
    water.set_oxygen_level(4);
    oxygen.improving();
    check_near(water.get_oxygen_level(), 5, "Raise low oxygen");
    water.set_oxygen_level(13);
    oxygen.improving();
    check_near(water.get_oxygen_level(), 10, "Lower high oxygen");
    Ozonator scarce_oxygen(7, water, 0, true, 1, 1);
    water.set_oxygen_level(18);
    scarce_oxygen.improving();
    check_near(water.get_oxygen_level(), 17, "Limited oxygen correction");
    scarce_oxygen.improving();
    scarce_oxygen.assign_employee(&technician);
    scarce_oxygen.perform_work(&technician, 1012026);
}

void test_staff_and_equipment() {
    Janitor janitor(1, 2);
    Plumber plumber(2, 2, 1);
    Electrician electrician(3, 2, 1);
    janitor.clean_area();
    check(!plumber.repair_leak(), "Plumber needs tools");
    electrician.inspect_circuits();

    Mop mop(100);
    janitor.set_mop(&mop);
    for (int i = 0; i < 3; ++i) janitor.clean_area();
    check(mop.get_dirt_level() == 0, "Janitor washes dirty mop");
    mop.clean();
    expect_error<ResourceDepletedError>([&] { mop.clean(); }, "Mop bucket is empty");
    mop.refill_water();
    mop.wash();
    mop.refill_water();
    for (int i = 0; i < 4; ++i) mop.clean();
    expect_error<ResourceDepletedError>([&] { mop.clean(); }, "Mop is too dirty");
    mop.wear_out(1000);
    expect_error<EquipmentFailureError>([&] { mop.clean(); }, "Mop is broken");
    janitor.clean_area();
    mop.repair();
    mop.repair();

    PlumberEquipment wrench(40);
    plumber.set_equipment(&wrench);
    wrench.check_pressure();
    check(wrench.get_pressure_gauge() >= 1, "Pressure was measured");
    check(plumber.repair_leak(), "First repair succeeds");
    check(!plumber.repair_leak(), "Wrench breaks during second repair");
    expect_error<EquipmentFailureError>([&] { wrench.check_pressure(); }, "Broken wrench cannot measure");
    expect_error<EquipmentFailureError>([&] { wrench.fix_leak(); }, "Broken wrench cannot fix leaks");
    EmployeesEquipment& base_tool = wrench;
    base_tool.repair();
    check(wrench.get_has_wrench(), "Repair dispatches to the derived tool");

    ElectricianEquipment tester;
    electrician.set_equipment(&tester);
    for (int i = 0; i < 10; ++i) electrician.inspect_circuits();
    electrician.inspect_circuits();
    check(tester.get_battery_level() == 0, "Drain charged tester");
    tester.recharge_battery();
    for (int i = 0; i < 5; ++i) electrician.inspect_circuits();
    electrician.inspect_circuits();
    tester.repair();
    tester.replace_insulation();
    check(tester.get_insulation_level() == 100, "Tester insulation restored");

    Schedule schedule;
    Shift invalid;
    expect_error<StaffManagementError>([&] { schedule.add_shift(invalid); }, "Shift must have an employee");
    expect_error<NullPointerError>([&] { invalid.generate_incidents(); }, "No incidents for invalid shift");
    expect_error<NullPointerError>([&] { invalid.generate_events(); }, "No events for invalid shift");
    expect_error<NullPointerError>([&] { invalid.calculate_shift_rate(); }, "No rate for invalid shift");
    Shift first(&janitor);
    Shift second(&janitor);
    Shift third(&electrician);
    first.generate_incidents();
    second.generate_incidents();
    third.generate_incidents();
    for (int i = 0; i < 200; ++i) first.generate_events();
    const double expected = first.calculate_shift_rate() + second.calculate_shift_rate();
    schedule.add_shift(first);
    schedule.add_shift(second);
    schedule.add_shift(third);
    expect_error<DuplicateEntityError>([&] { schedule.add_shift(second); }, "Cannot add the same shift twice");
    schedule.calculate_monthly_salaries();
    check_near(janitor.get_salaries().at(0), expected, "Two shifts produce one cumulative salary payment");
    schedule.calculate_monthly_salaries();
    check(janitor.get_salaries().size() == 1 && electrician.get_salaries().size() == 1, "Repeat payroll does not pay again");
}

void test_visitors() {
    DailyStatistics statistics("2026-11-15");
    DailyStatistics::set_current(&statistics);
    Cashier cashier(1, 2);
    AudioGuide guide(3);
    Lounge lounge(5);
    VipVisit vip(10);
    Headphones headphones;
    Headphones extra;
    Food food(3, "bread", 150);
    Drinks juice(4, 330);
    AlcoholDrinks alcohol(50, 500, 5);
    Visitor child(7, "Child", 16, 100);
    Visitor adult(8, "Adult", 20, 40);
    Visitor poor(9, "Poor", 20, 0);
    Visitor unregistered(10, "Unregistered", 22, 70);
    expect_error<InvalidActionError>([] { Ticket invalid(-1); }, "Negative ticket price");
    expect_error<InvalidActionError>([] { Food invalid(-3); }, "Negative buffet price");
    expect_error<InvalidActionError>([] { AudioGuide invalid(-3); }, "Negative service price");
    expect_error<InvalidActionError>([] { Visitor invalid(1, "x", -1, 10); }, "Negative age");
    expect_error<InvalidActionError>([&] { adult.set_money_count(-1); }, "Negative balance");
    adult.fill_ticket_data();
    adult.add_service(nullptr);
    adult.remove_service(nullptr);
    adult.remove_service(&lounge);
    check(!adult.buy_ticket(nullptr), "Null ticket returns null");
    auto sold = std::make_unique<Ticket>(2);
    sold->set_bought(true);
    check(adult.buy_ticket(std::move(sold)) != nullptr, "Already sold ticket is rejected");

    GroupVisit group(65);
    check(!group.check_can_enter(), "Empty group cannot enter");
    expect_error<NullPointerError>([&] { group.add_visitor(nullptr); }, "Cannot add null");
    group.add_visitor(&child);
    group.add_visitor(&poor);
    expect_error<DuplicateEntityError>([&] { group.add_visitor(&child); }, "Cannot join group twice");
    expect_error<NullPointerError>([&] { cashier.sell_ticket(child, nullptr); }, "No ticket at checkout");
    check(cashier.sell_ticket(child, std::make_unique<Ticket>(20)).first, "Sell group discount ticket");
    check_near(child.get_money_count(), 86, "Discount is 30 percent");
    check(child.get_ticket()->get_visitor_id() == 7 && child.get_ticket()->get_full_name() == "Child", "Ticket keeps visitor identity");
    check(!cashier.sell_ticket(poor, std::make_unique<Ticket>(20)).first, "Insufficient funds");
    check(group.check_can_enter() && group.get_visitors().size() == 1 && !poor.is_in_group(), "Group drops visitor without ticket");
    check(cashier.sell_ticket(adult, std::make_unique<Ticket>(10)).first, "Sell individual ticket");
    check(!cashier.sell_ticket(adult, std::make_unique<Ticket>(10)).first, "One ticket per visitor");
    statistics.add_group(0);
    statistics.add_group(1);
    statistics.add_visitors(0);
    statistics.add_ticket_sale(100, 0);
    check(statistics.get_groups_count() == 1 && statistics.get_tickets_sold() == 2, "Ticket and group statistics");

    expect_error<InvalidActionError>([&] { lounge.buy(unregistered); }, "Service needs a valid ticket");
    expect_error<ServiceNotPurchasedError>([&] { guide.assign_headphones(&headphones, child); }, "Guide must be bought before rental");
    check(guide.buy(child), "Child buys guide");
    expect_error<DuplicateEntityError>([&] { guide.buy(child); }, "Duplicate guide purchase");
    const double before_refund = child.get_money_count();
    expect_error<InvalidActionError>([&] { guide.assign_headphones(nullptr, child); }, "Unavailable headphones trigger a refund");
    check_near(child.get_money_count(), before_refund + 3, "Guide was refunded");
    guide.buy(child);
    guide.assign_headphones(&headphones, child);
    guide.assign_headphones(&headphones, child);
    expect_error<InvalidActionError>([&] { guide.assign_headphones(&extra, child); }, "A second pair cannot overwrite the first");
    guide.buy(adult);
    expect_error<InvalidActionError>([&] { guide.assign_headphones(&headphones, adult); }, "Cannot assign an occupied headset");
    check(headphones.get_visitor() == &child, "Failed rental does not invalidate active rental");
    expect_error<InvalidActionError>([&] { guide.return_headphones(adult); }, "Only holder can return headset");
    guide.return_headphones(child);
    check(!headphones.is_assigned(), "Returned headset becomes available");

    lounge.add_to_menu(nullptr);
    lounge.add_to_menu(&food);
    lounge.add_to_menu(&alcohol);
    expect_error<NullPointerError>([&] { lounge.buy_item(child, nullptr); }, "Null food item");
    expect_error<EntityNotFoundError>([&] { lounge.buy_item(child, &juice); }, "Unknown food item");
    expect_error<ServiceNotPurchasedError>([&] { lounge.buy_item(child, &food); }, "Food requires the lounge service");
    check(lounge.buy(child) && lounge.buy(adult), "Lounge purchases");
    expect_error<InvalidActionError>([&] { lounge.buy_item(child, &alcohol); }, "Underage visitor cannot buy alcohol");
    expect_error<InsufficientFundsError>([&] { lounge.buy_item(adult, &alcohol); }, "Unaffordable lounge item");
    const double till = Buffet::get_cash_register();
    check(lounge.buy_item(child, &food), "Underage visitor can still buy food");
    check_near(Buffet::get_cash_register(), till + 3, "Food payment is added to till");

    check(vip.get_audio_guide(adult) == nullptr && vip.get_lounge(adult, 0) == nullptr, "No service before VIP purchase");
    check(vip.buy(child), "VIP package");
    expect_error<InvalidActionError>([&] { vip.buy(child); }, "Cannot buy VIP package twice");
    check(vip.get_audio_guide(child) && vip.get_lounge(child, 5) && !vip.get_lounge(child, 6), "VIP grants access to six lounges");
    expect_error<InsufficientFundsError>([&] { Favour& expensive = vip; adult.set_money_count(0); expensive.buy(adult); }, "Insufficient VIP funds");
    DailyStatistics::set_current(nullptr);
    Visitor no_stats(11, "No statistics", 30, 20);
    no_stats.buy_ticket(std::make_unique<Ticket>(0));
    guide.buy(no_stats);
}

} // namespace

int main() {
    try {
        test_core();
        test_biology();
        test_habitats();
        test_water();
        test_staff_and_equipment();
        test_visitors();
        std::cout << "All domain tests passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Test failed: " << error.what() << '\n';
        return 1;
    }
}
