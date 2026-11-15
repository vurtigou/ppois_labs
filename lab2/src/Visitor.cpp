#include "Visitor.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "DailyStatistics.h"
#include "Favour.h"
#include "Ticket.h"
#include "exceptions.h"   // InvalidActionError

Visitor::Visitor(int id, const std::string& name, int age, double money)
    : age(age), visitor_id(id), full_name(name), ticket(nullptr),
      money_count(0), can_enter(false), in_group(false) {
    if (age < 0) throw InvalidActionError("Visitor: Age cannot be negative.");
    set_money_count(money);
}

std::unique_ptr<Ticket> Visitor::buy_ticket(std::unique_ptr<Ticket> t) {
    if (!t)             return t;   // билета нет
    if (t->is_bought()) return t;   // уже продан
    if (ticket)         return t;   // у посетителя уже есть билет

    double actual_price = static_cast<double>(t->get_price());
    if (in_group) actual_price *= 0.7;

    if (money_count < actual_price) {
        can_enter = false;
        return t;                    // не хватает денег — билет возвращаем
    }

    money_count -= actual_price;
    can_enter = true;
    t->set_bought(true);
    ticket = std::move(t);           // забираем владение
    fill_ticket_data();
    if (auto* stats = DailyStatistics::get_current()) {
        stats->add_ticket_sale(actual_price);
        stats->add_visitors(1);
    }
    return nullptr;
}

void Visitor::fill_ticket_data() {
    if (ticket) {
        ticket->set_visitor_data(visitor_id, full_name);
    }
}

void Visitor::add_service(Favour* service) {
    if (service != nullptr) {
        services.push_back(service);
    }
}

void Visitor::remove_service(Favour* service) {
    if (!service) return;
    auto it = std::find(services.begin(), services.end(), service);
    if (it != services.end()) {
        services.erase(it);
    }
}

const std::vector<Favour*>& Visitor::get_services() const { return services; }
void Visitor::set_in_group(bool status) { in_group = status; }
void Visitor::set_visitor_id(int id)    { visitor_id = id; }

void Visitor::set_money_count(double money) {
    if (!std::isfinite(money) || money < 0) {
        throw InvalidActionError("Visitor: Balance must be finite and nonnegative.");
    }
    money_count = money;
}

bool   Visitor::is_in_group()      const { return in_group; }
int    Visitor::get_visitor_id()   const { return visitor_id; }
double Visitor::get_money_count()  const { return money_count; }
int    Visitor::get_age()          const { return age; }
bool   Visitor::get_can_enter()    const { return can_enter; }
std::string Visitor::get_full_name() const { return full_name; }
Ticket* Visitor::get_ticket()      const { return ticket.get(); }
