#include "Lounge.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "DailyStatistics.h"
#include "Visitor.h"
#include "exceptions.h"   // NullPointerError, EntityNotFoundError, ServiceNotPurchasedError,
                          // InvalidActionError, InsufficientFundsError
#include "Buffet.h"    
#include "AlcoholDrinks.h"    

void Lounge::add_to_menu(Buffet* item) {
    if (item) menu.push_back(item);
}

bool Lounge::buy_item(Visitor& visitor, Buffet* item) {
    if (!item) {
        throw NullPointerError("Lounge: Item pointer is null.");
    }

    // 1. Товар должен быть в меню
    if (std::find(menu.begin(), menu.end(), item) == menu.end()) {
        throw EntityNotFoundError("Lounge: Item is not on the menu.");
    }

    // 2. Услуга Lounge должна быть куплена
    bool has_lounge = false;
    for (const auto* s : visitor.get_services()) {
        if (s == this) { has_lounge = true; break; }
    }
    if (!has_lounge) {
        throw ServiceNotPurchasedError("Lounge: Visitor " + visitor.get_full_name() + " didn't buy the Lounge service.");
    }

    // 3. Проверка возраста для алкоголя
    if (dynamic_cast<AlcoholDrinks*>(item) != nullptr) {
        if (visitor.get_age() < 18) {
            throw InvalidActionError("Lounge: Underage visitor cannot buy alcohol. (Lounge service kept).");
        }
    }

    // 4. Деньги
    if (visitor.get_money_count() < item->get_price()) {
        throw InsufficientFundsError("Lounge: Not enough money for item. (Need: "
                                     + std::to_string(item->get_price()) + ")");
    }

    // 5. Списываем деньги и пополняем кассу
    visitor.set_money_count(visitor.get_money_count() - item->get_price());
    Buffet::add_to_cash_register(item->get_price());
    if (auto* stats = DailyStatistics::get_current()) {
        stats->add_revenue(item->get_price());
    }

    std::cout << "Success: " << visitor.get_full_name()
              << " bought an item in Lounge for " << item->get_price() << " rub." << std::endl;
    return true;
}
