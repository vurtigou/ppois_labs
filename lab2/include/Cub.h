#pragma once

#include <string>

#include "Animal.h"

class Cub : public Animal {
private:
    int age;

public:
    // Конструктор принимает ID родителя, чтобы сгенерировать свой ID
    Cub(const std::string& parent_id, Species sp = Species(), double weight = 0.0);

    // Переопределяем методы родителя
    void record_weight_change(double new_weight) override;
    void move_to_habitat(Habitat* new_habitat) override;
    bool can_be_moved_to_habitat() const override { return get_habitat() == nullptr; }

    // Детеныши не могут спариваться
    bool can_breed() const override { return false; }

    // При каждом визите к врачу возраст увеличивается
    void on_vet_visit() override { age++; }

    int get_age() const { return age; }
};
