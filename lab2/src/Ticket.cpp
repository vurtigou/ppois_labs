#include "Ticket.h"

#include <string>

#include "exceptions.h"   // InvalidActionError

Ticket::Ticket(int price)
    : price(price), bought(false), visitor_id(0), full_name("") {
    if (price < 0) throw InvalidActionError("Ticket: Price cannot be negative.");
}

int Ticket::get_price() const { return price; }
bool Ticket::is_bought() const { return bought; }
int Ticket::get_visitor_id() const { return visitor_id; }
std::string Ticket::get_full_name() const { return full_name; }

void Ticket::set_bought(bool status) { bought = status; }

void Ticket::set_visitor_data(int id, const std::string& name) {
    visitor_id = id;
    full_name = name;
}
