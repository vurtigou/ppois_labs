#pragma once

#include <memory>
#include <string>
#include <utility>

#include "Employee.h"

class Ticket;
class Visitor;

/** @brief Employee who sells tickets and counts successful sales.
 * @ingroup staff
 */
class Cashier : public Employee {
private:
    int tickets_sold;

public:
    static const std::string position;
    static const double min_rate;
    // ставка за штраф
    static const double pin_rate;
    // ставка за бонус
    static const double bon_rate;

    Cashier(int id, int exp);

    std::string get_position() const override;
    double get_pin_rate() const override { return pin_rate; }
    double get_bon_rate() const override { return bon_rate; }

    // Метод продажи билета
    /**
     * @brief Attempts to sell a caller-owned ticket and increments sales only on success.
     * @return The success flag and the unsold ticket, if the purchase failed.
     */
    std::pair<bool, std::unique_ptr<Ticket>>
    sell_ticket(Visitor& visitor, std::unique_ptr<Ticket> ticket);

    // Геттер для счётчика
    int get_tickets_sold() const { return tickets_sold; }
};
