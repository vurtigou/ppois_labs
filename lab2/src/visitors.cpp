#include "visitors.h"
#include <algorithm>
#include <utility>

Ticket::Ticket(int price) 
    : price(price), bought(false), visitor_id(0), full_name("") {}

int Ticket::get_price() const { return price; }
bool Ticket::is_bought() const { return bought; }
int Ticket::get_visitor_id() const { return visitor_id; }
std::string Ticket::get_full_name() const { return full_name; }

void Ticket::set_bought(bool status) { bought = status; }

void Ticket::set_visitor_data(int id, const std::string& name) {
    visitor_id = id;
    full_name = name;
}

Visitor::Visitor(int id, const std::string& name, double money)
    : visitor_id(id), full_name(name), ticket(nullptr),
      money_count(money), can_enter(false), in_group(false) {}

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
    return nullptr;
}

void Visitor::fill_ticket_data() {
    if (ticket) {
        ticket->set_visitor_data(visitor_id, full_name);
    }
}

void Visitor::set_in_group(bool status)   { in_group = status; }
void Visitor::set_visitor_id(int id)      { visitor_id = id; }
void Visitor::set_money_count(double m)   { money_count = m; } 

bool   Visitor::is_in_group()     const { return in_group; }
int    Visitor::get_visitor_id()  const { return visitor_id; }
double Visitor::get_money_count() const { return money_count; }  
bool   Visitor::get_can_enter()   const { return can_enter; }
std::string Visitor::get_full_name() const { return full_name; }
Ticket* Visitor::get_ticket()     const { return ticket.get(); } 

GroupVisit::GroupVisit(int id) : group_id(id), can_enter(false) {}

void GroupVisit::add_visitor(Visitor* v) {
    if (!v) return;

    visitors.push_back(v);
    v->set_in_group(true);
    v->set_visitor_id(group_id);

    std::cout << "Visitor " << v->get_full_name()
              << " added to group " << group_id << std::endl;
}

bool GroupVisit::check_can_enter() {
    // Удаляем из группы всех, кто не может войти (nullptr или нет права входа)
    visitors.erase(std::remove_if(visitors.begin(), visitors.end(),
    [](Visitor* v) {
        if (v == nullptr || !v->get_can_enter()) {
            if (v) v->set_in_group(false);
            return true;
        }
        return false;
    }), visitors.end());

    // Группа может войти, если после фильтрации в ней остался хотя бы один,
    // а все оставшиеся (по построению) могут войти
    can_enter = !visitors.empty();
    return can_enter;
}

int GroupVisit::get_group_id() const { return group_id; }
bool GroupVisit::get_can_enter()  const { return can_enter; }
const std::vector<Visitor*>& GroupVisit::get_visitors() const { return visitors; }

DailyStatistics::DailyStatistics(std::string date)
    : date(date), tickets_sold(0), visitors_count(0), 
      groups_count(0), group_visitors(0), total_revenue(0.0) {}

void DailyStatistics::add_ticket_sale(double price, int count) {
    if (count <= 0) return;
    tickets_sold += count;
    total_revenue += price * count;
}

void DailyStatistics::add_visitors(int count) {
    if (count <= 0) return;
    visitors_count += count;
}

void DailyStatistics::add_group(int visitors_in_group) {
    if (visitors_in_group <= 0) return;
    groups_count++;
    group_visitors += visitors_in_group;
}

void DailyStatistics::print_report() const {
    std::cout << "========================================" << std::endl;
    std::cout << "DAILY STATISTICS REPORT" << std::endl;
    std::cout << "Date: " << (date.empty() ? "Unknown" : date) << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tickets sold:    " << tickets_sold << std::endl;
    std::cout << "Visitors arrived:" << visitors_count << std::endl;
    std::cout << "Groups visited:  " << groups_count 
              << " (Total people in groups: " << group_visitors << ")" << std::endl;
    std::cout << "Total revenue:   $" << total_revenue << std::endl;
    std::cout << "========================================" << std::endl;
}
