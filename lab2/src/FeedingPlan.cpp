#include "FeedingPlan.h"

#include <iostream>
#include <string>

#include "Animal.h"
#include "Cub.h"          // dynamic_cast<Cub*>
#include "exceptions.h"   // OceanariumError, NullPointerError
#include "FeedStock.h"    // FeedStock::get_name

FeedingPlan::FeedingPlan(Animal* a, double portion)
    : animal(a), portion_grams(portion) {}

void FeedingPlan::adjust_portion(double last_weight) {
    if (last_weight > 0.0 && animal != nullptr) {
        double current_weight = animal->get_weight();

        if (current_weight > 0.0) {
            double weight_ratio = current_weight / last_weight;
            portion_grams *= weight_ratio;

            std::cout << "Feeding portion adjusted to: " << portion_grams << " grams." << std::endl;
        }
    } else {
        throw OceanariumError("FeedingPlan: Invalid last weight or animal is null.");
    }
}

void FeedingPlan::add_feed_item(FeedStock* item) {
    if (item != nullptr) {
        feed_items.push_back(item);
    }
}

bool FeedingPlan::check_diet_compliance() const {
    if (!animal) {
        throw NullPointerError("FeedingPlan: Animal is null, cannot check diet.");
    }
    std::string animal_diet = animal->get_species().get_diet_type();
    if (feed_items.empty()) return true;

    for (const auto* item : feed_items) {
        if (item == nullptr) continue;

        std::string food_name = item->get_name();
        if (animal_diet.find(food_name) == std::string::npos) return false;
    }

    return true;
}

void FeedingPlan::calculate_portion() {
    if (!animal) {
        throw NullPointerError("FeedingPlan: Animal is null, cannot calculate portion.");
    }

    constexpr double grams_per_kg = 1000.0;
    double weight = animal->get_weight();
    double weight_grams = weight * grams_per_kg;

    if (dynamic_cast<Cub*>(animal) != nullptr) {
        portion_grams = weight_grams * 0.03; // 3% от массы
    }
    else if (weight > 10.0) {
        portion_grams = weight_grams * 0.10; // 10% от массы
    }
    else {
        portion_grams = weight_grams * 0.05; // 5% от массы
        std::cout << "Portion calculated: " << portion_grams << " grams (5% of mass)." << std::endl;
    }
}
