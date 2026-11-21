#pragma once

#include <vector>

class Animal;
class FeedStock;

/** @brief Calculates portions in grams and checks food against the animal’s diet.
 * @ingroup animals
 */
class FeedingPlan {
private:
    Animal* animal;                    // non-owning
    std::vector<FeedStock*> feed_items; // non-owning
    double portion_grams;

public:
    FeedingPlan(Animal* a = nullptr, double portion = 0.0);
    /**
     * @brief Scales the existing portion by the ratio of current to previous weight.
     * @param last_weight Previous animal weight in kilograms.
     */
    void adjust_portion(double last_weight);
    void add_feed_item(FeedStock* item);
    bool check_diet_compliance() const;
    /**
     * @brief Recalculates the portion in grams from the animal’s weight and age class.
     * @throws NullPointerError The plan has no animal.
     */
    void calculate_portion();

    double get_portion_grams() const { return portion_grams; }
    void set_animal(Animal* a) { animal = a; }
};
