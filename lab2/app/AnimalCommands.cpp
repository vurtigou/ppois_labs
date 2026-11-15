#include "Console.h"

#include "AnimalTransfer.h"
#include "Cub.h"
#include "VetVisit.h"

#include <iostream>
#include <utility>

void Console::place(std::unique_ptr<Animal> animal, Habitat& home) {
    const auto id = animal->get_id();
    if (id.empty()) throw InvalidActionError("Animal ID cannot be empty.");
    auto [entry, inserted] = animals.try_emplace(id, std::move(animal));
    if (!inserted) throw DuplicateEntityError("Animal ID already exists");
    try {
        home.add_animal(entry->second.get());
    } catch (...) {
        animals.erase(entry);
        throw;
    }
}

void Console::add(const Args& args) {
    auto id = args[0];
    const auto weight = number<double>(args[1]);
    if (weight <= 0) throw InvalidActionError("Weight must be positive.");
    auto& home = habitat(args.size() >= 3 ? args[2] : "reef");
    const auto profile = args.size() == 4 ? args[3] : "reef-fish";
    if (profile != "reef-fish" && profile != "lionfish") throw EntityNotFoundError("Unknown species: " + profile);
    if (id == "auto") {
        do {
            id = animal_ids.generate_next();
        } while (animals.count(id) != 0);
    }
    place(std::make_unique<Animal>(id, profile == "lionfish" ? lionfish : reef_fish, weight), home);
}

void Console::cub(const Args& args) {
    auto& parent = require(animals, args[1], "animal ID");
    const auto weight = number<double>(args[2]);
    if (weight <= 0) throw InvalidActionError("Weight must be positive.");
    auto* home = parent.get_habitat();
    if (!home || !parent.can_breed()) throw InvalidActionError("Parent must be an adult in a regular habitat.");
    place(std::make_unique<Cub>(args[0], parent, weight), *home);
}

void Console::list_animals(const Args&) {
    std::cout << "Registered animals: " << animals.size() << '\n';
    for (const auto& [id, animal] : animals) {
        const auto* tank = animal->get_current_tank();
        std::cout << id << " | " << animal->get_weight() << " kg | "
                  << (tank ? tank->get_tank_id() : "no tank") << '\n';
    }
}

void Console::compatibility(const Args& args) {
    const auto& first = require(animals, args[0], "animal ID");
    const auto& second = require(animals, args[1], "animal ID");
    const bool compatible = first.get_species().is_compatible_with_species(second.get_species());
    std::cout << "Species compatibility: " << (compatible ? "compatible" : "incompatible") << '\n';
}

void Console::stock(const Args&) {
    std::cout << plankton.get_name() << ": " << plankton.get_weight_kg() << " kg"
              << " | " << krill.get_name() << ": " << krill.get_weight_kg() << " kg\n";
}

void Console::transfer(const Args& args) {
    auto& animal = require(animals, args[0], "animal ID");
    auto& destination = habitat(args.size() == 2 ? args[1] : "reserve");
    AnimalTransfer(&animal, &destination).approve_transfer();
}

void Console::weigh(const Args& args) {
    auto& animal = require(animals, args[0], "animal ID");
    animal.record_weight_change(number<double>(args[1]));
    std::cout << "Recorded weight for " << args[0] << ": " << animal.get_weight() << " kg\n";
}

void Console::feed(const Args& args) {
    auto& animal = require(animals, args[0], "animal ID");
    const auto name = args.size() == 2 ? args[1] : "plankton";
    if (name != "plankton" && name != "krill") throw EntityNotFoundError("Unknown feed: " + name);
    auto& stock = name == "plankton" ? plankton : krill;
    FeedingPlan plan(&animal);
    plan.add_feed_item(&stock);
    if (!plan.check_diet_compliance()) throw InvalidActionError("Feed is incompatible with the animal's diet.");
    plan.calculate_portion();
    animal.assign_feeding_plan(plan);
    std::cout << "Feeding portion: " << plan.get_portion_grams() << " g\n";
}

void Console::vet(const Args& args) {
    auto& animal = require(animals, args[0], "animal ID");
    if (args.size() == 3 && args[2] != "quarantine") throw InvalidActionError("Use quarantine as the third argument.");
    if (args[1].empty()) throw InvalidActionError("Diagnosis cannot be empty.");
    auto visit = std::make_unique<VetVisit>(&animal);
    visit->diagnostic(args[1], args.size() == 3 ? &quarantine : nullptr);
    visit->close_visit();
    animal.get_health_record().add_visit(std::move(visit));
    animal.get_health_record().status_diagnosis();
}

void Console::release(const Args& args) {
    auto& animal = require(animals, args[0], "animal ID");
    auto& destination = habitat(args[1]);
    quarantine.release_after_check(animal.get_id(), destination);
    std::cout << "Returned from quarantine: " << animal.get_id() << '\n';
}

void Console::breed(const Args& args) {
    auto& first = require(animals, args[0], "animal ID");
    auto& second = require(animals, args[1], "animal ID");
    if (!ControlSystem::is_valid_date(number<int>(args[2]))) throw InvalidDateError("Invalid breeding date.");
    auto& breeding = first.get_species().get_latin_name() == lionfish.get_latin_name()
        ? lionfish_breeding : reef_breeding;
    breeding.select_pair(&first, &second);
    breeding.schedule_breeding(args[2]);
}
