#pragma once

#include <vector>

class Animal;
class FeedStock;

class FeedingPlan {
private:
    Animal* animal;                    // non-owning
    std::vector<FeedStock*> feed_items; // non-owning
    double portion_grams;

public:
    FeedingPlan(Animal* a = nullptr, double portion = 0.0);
    void adjust_portion(double last_weight);
    void add_feed_item(FeedStock* item);
    bool check_diet_compliance() const;
    void calculate_portion();

    double get_portion_grams() const { return portion_grams; }
    void set_animal(Animal* a) { animal = a; }
};
