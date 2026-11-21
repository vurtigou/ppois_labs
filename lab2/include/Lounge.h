#pragma once

#include <vector>

#include "Favour.h"

class Buffet;
class Visitor;

/** @brief Service that sells menu items and enforces purchase and age rules.
 * @ingroup visitors
 */
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
    /**
     * @brief Charges a visitor for a menu item and records the revenue.
     * @throws ServiceNotPurchasedError The visitor has not purchased this lounge service.
     * @throws InvalidActionError The buyer is under 18 and the item is alcoholic.
     * @throws InsufficientFundsError The visitor cannot afford the item.
     */
    bool buy_item(Visitor& visitor, Buffet* item);
};
