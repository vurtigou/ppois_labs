#pragma once

#include <vector>

#include "Favour.h"

class Buffet;
class Visitor;

class Lounge : public Favour {
private:
    std::vector<Buffet*> menu; // ссылки на доступные к покупке товары

public:
    Lounge(double price = 0.0) : Favour(price) {}

    bool buy(Visitor& visitor) override { return purchase(visitor); }

    void add_to_menu(Buffet* item);
    const std::vector<Buffet*>& get_menu() const { return menu; }

    // Покупатель, купивший Lounge, может купить любой элемент из меню.
    // Если возраст < 18 — покупка не проходит, услуга остаётся у посетителя.
    bool buy_item(Visitor& visitor, Buffet* item);
};
