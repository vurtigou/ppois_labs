#include "AudioGuide.h"

#include <iostream>
#include <string>

#include "DailyStatistics.h"
#include "Visitor.h"
#include "exceptions.h"   // InvalidActionError, ServiceNotPurchasedError
#include "Headphones.h"    // Headphones

void AudioGuide::assign_headphones(Headphones* hp, Visitor& visitor) {
    // 1. Услуга должна быть куплена
    bool bought = false;
    for (const auto* s : visitor.get_services()) {
        if (s == this) { bought = true; break; }
    }
    if (!bought) {
        throw ServiceNotPurchasedError("AudioGuide: Visitor " + visitor.get_full_name() + " didn't buy this service.");
    }

    if (hp && hp == headphones && hp->get_visitor() == &visitor) return;
    if (headphones && headphones->get_visitor() == &visitor) {
        throw InvalidActionError("AudioGuide: Return the current headphones first.");
    }

    if (!hp || hp->is_assigned() || headphones) {
        visitor.set_money_count(visitor.get_money_count() + get_price());
        visitor.remove_service(this);
        if (auto* stats = DailyStatistics::get_current()) stats->add_revenue(-get_price());
        throw InvalidActionError("AudioGuide: Headphones unavailable. Money refunded.");
    }

    headphones = hp;
    hp->set_visitor(&visitor);
    std::cout << "AudioGuide: Headphones successfully assigned to " << visitor.get_full_name() << std::endl;
}

void AudioGuide::return_headphones(const Visitor& visitor) {
    if (!headphones || headphones->get_visitor() != &visitor) {
        throw InvalidActionError("AudioGuide: The visitor has no headphones to return.");
    }
    headphones->set_visitor(nullptr);
    headphones = nullptr;
}
