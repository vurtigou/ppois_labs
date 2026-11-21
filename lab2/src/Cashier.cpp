#include "Cashier.h"

#include <iostream>
#include <memory>
#include <utility>

#include "exceptions.h"   // NullPointerError
#include "Visitor.h"     
#include "Ticket.h" 

const std::string Cashier::position = "Cashier";
const double Cashier::min_rate = 80.0;
const double Cashier::pin_rate = 25.0;
const double Cashier::bon_rate = 30.0;

Cashier::Cashier(int id, int exp)
    : Employee(id, exp, static_cast<int>(min_rate * exp), 0), tickets_sold(0) {}

std::string Cashier::get_position() const { return position; }

std::pair<bool, std::unique_ptr<Ticket>>
Cashier::sell_ticket(Visitor& visitor, std::unique_ptr<Ticket> ticket) {
    if (!ticket) {
        throw NullPointerError("Cashier: Cannot sell a null ticket.");
    }

    // Visitor::buy_ticket возвращает: nullptr — успешно куплено;
    // unique_ptr — не куплено, билет возвращён нам.
    std::unique_ptr<Ticket> unsold = visitor.buy_ticket(std::move(ticket));

    if (!unsold) {
        tickets_sold++;
        std::cout << "Кассир: Билет успешно продан посетителю "
                  << visitor.get_full_name() << "!" << std::endl;
        return {true, nullptr};
    }

    // Не куплено — билет возвращаем вызывающей стороне.
    std::cout << "Кассир: Не удалось продать билет посетителю "
              << visitor.get_full_name()
              << " (билет возвращён в кассу)." << std::endl;
    return {false, std::move(unsold)};
}
