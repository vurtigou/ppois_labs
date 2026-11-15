#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Ticket.h"   // unique_ptr<Ticket> — член + in-class defaulted move-операции

class Favour;

class Visitor {
private:
    int age;
    std::vector<Favour*> services;
    int visitor_id;
    std::string full_name;
    std::unique_ptr<Ticket> ticket;
    double money_count;
    bool can_enter;
    bool in_group;

public:
    Visitor(int id = 0, const std::string& name = "", int age = 0, double money = 0);

    Visitor(const Visitor&)            = delete;
    Visitor& operator=(const Visitor&) = delete;
    Visitor(Visitor&&)                 = default;
    Visitor& operator=(Visitor&&)      = default;

    std::unique_ptr<Ticket> buy_ticket(std::unique_ptr<Ticket> t);

    void fill_ticket_data();
    void add_service(Favour* service);
    const std::vector<Favour*>& get_services() const;
    void remove_service(Favour* service);

    void set_in_group(bool status);
    void set_visitor_id(int id);
    void set_money_count(double money);

    bool   is_in_group() const;
    int    get_visitor_id() const;
    int    get_age() const;
    double get_money_count() const;
    bool   get_can_enter() const;
    std::string get_full_name() const;
    Ticket* get_ticket() const;       // non-owning view
};
