#pragma once

#include "Favour.h"

class Headphones;
class Visitor;

class AudioGuide : public Favour {
private:
    Headphones* headphones;

public:
    AudioGuide(double price = 0.0) : Favour(price), headphones(nullptr) {}

    bool buy(Visitor& visitor) override { return purchase(visitor); }

    // Метод добавления наушников. Если переданные наушники есть и никому
    // не принадлежат — они привязываются к посетителю. Иначе посетителю
    // возвращается price услуги, а сама услуга удаляется из его вектора услуг.
    void assign_headphones(Headphones* hp, Visitor& visitor);
    void return_headphones(const Visitor& visitor);
};
