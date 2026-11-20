#pragma once

#include <string>

/** @brief Admission ticket with its price and purchaser details.
 * @ingroup visitors
 */
class Ticket {
private:
    int price;
    bool bought;
    int visitor_id;
    std::string full_name;

public:
    Ticket(int price = 0);

    int get_price() const;
    bool is_bought() const;
    int get_visitor_id() const;
    std::string const& get_full_name() const;

    void set_bought(bool status);
    void set_visitor_data(int id, const std::string& name);
};
