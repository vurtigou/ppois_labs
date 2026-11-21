#include "Favour.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "DailyStatistics.h"
#include "Visitor.h"
#include "exceptions.h"   // InvalidActionError, InsufficientFundsError, DuplicateEntityError

Favour::Favour(double price) : price(price) {
    if (!std::isfinite(price) || price < 0.0) {
        throw InvalidActionError("Favour: Price must be finite and nonnegative.");
    }
}

bool Favour::purchase(Visitor& visitor) {
    // 1. Проверяем наличие билета и права на вход
    if (visitor.get_ticket() == nullptr || !visitor.get_can_enter()) {
        throw InvalidActionError("Favour: Visitor " + visitor.get_full_name() + " doesn't have a valid ticket.");
    }

    // 2. Проверяем деньги
    if (visitor.get_money_count() < price) {
        throw InsufficientFundsError("Favour: Visitor " + visitor.get_full_name()
                                     + " doesn't have enough money. (Need: " + std::to_string(price)
                                     + ", Has: " + std::to_string(visitor.get_money_count()) + ")");
    }

    const auto& services = visitor.get_services();
    if (std::find(services.begin(), services.end(), this) != services.end()) {
        throw DuplicateEntityError("Favour: The visitor already bought this service.");
    }
    visitor.add_service(this);
    visitor.set_money_count(visitor.get_money_count() - price);

    if (auto* stats = DailyStatistics::get_current()) {
        stats->add_revenue(price);
    }
    return true;
}
