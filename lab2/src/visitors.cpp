#include "visitors.h"
#include "inventory.h"
#include <algorithm>
#include <utility>

double Buffet::cash_register = 0.0;
DailyStatistics* DailyStatistics::current = nullptr; 

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

Visitor::Visitor(int id, const std::string& name, int age, double money)
    : visitor_id(id), full_name(name), age(age), ticket(nullptr),
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
    // Ищем услугу в векторе и удаляем её
    auto it = std::find(services.begin(), services.end(), service);
    if (it != services.end()) {
        services.erase(it);
    }
}

const std::vector<Favour*>& Visitor::get_services() const {return services;}
void Visitor::set_in_group(bool status)   { in_group = status; }
void Visitor::set_visitor_id(int id)      { visitor_id = id; }
void Visitor::set_money_count(double m)   { money_count = m; } 

bool   Visitor::is_in_group()     const { return in_group; }
int    Visitor::get_visitor_id()  const { return visitor_id; }
double Visitor::get_money_count() const { return money_count; }  
int Visitor::get_age() const { return age; }
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

void DailyStatistics::add_revenue(double amount) {
    total_revenue += amount;   // допускает отрицательные значения для возвратов
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

bool Favour::purchase(Visitor& visitor) {
    // 1. Проверяем наличие билета и права на вход
    if (visitor.get_ticket() == nullptr || !visitor.get_can_enter()) {
        std::cout << "Ошибка: Посетитель " << visitor.get_full_name() 
                  << " не имеет действительного билета для покупки услуг." << std::endl;
        return false;
    }

    // 2. Проверяем деньги
    if (visitor.get_money_count() < price) {
        std::cout << "Ошибка: У посетителя " << visitor.get_full_name() 
                  << " недостаточно средств для покупки (Нужно: " << price 
                  << ", Есть: " << visitor.get_money_count() << ")." << std::endl;
        return false;
    }

    // 3. Списываем деньги и добавляем услугу
    visitor.set_money_count(visitor.get_money_count() - price);
    visitor.add_service(this);

    // <-- добавлено: учёт выручки от услуг
    if (auto* stats = DailyStatistics::get_current()) {
        stats->add_revenue(price);
    }
    return true;
}

void AudioGuide::assign_headphones(Headphones* hp, Visitor& visitor) {
    // 1. Услуга должна быть куплена (симметрично для успеха и ошибки)
    bool bought = false;
    for (const auto* s : visitor.get_services()) {
        if (s == this) { bought = true; break; }
    }
    if (!bought) {
        std::cout << "Ошибка: Посетитель " << visitor.get_full_name()
                  << " не покупал услугу аудиогида." << std::endl;
        return;
    }

    // 2. Наушники должны существовать и быть свободными
    if (hp == nullptr || hp->is_assigned()) {
        std::cout << "Ошибка: Наушники отсутствуют или уже заняты другим посетителем." << std::endl;
        std::cout << "Возврат средств за аудиогид." << std::endl;
        visitor.set_money_count(visitor.get_money_count() + this->get_price());
        visitor.remove_service(this);
        if (auto* stats = DailyStatistics::get_current()) {
            stats->add_revenue(-this->get_price());
        }
        this->headphones = nullptr;
        return;
    }

    // 3. Выдаём наушники
    this->headphones = hp;
    hp->set_visitor(&visitor);
}

void Lounge::add_to_menu(Buffet* item) { 
    if (item) menu.push_back(item); 
}

bool Lounge::buy_item(Visitor& visitor, Buffet* item) {
    if (!item) return false;

    //товар должен быть в меню этого Lounge
    if (std::find(menu.begin(), menu.end(), item) == menu.end()) {
        std::cout << "Ошибка: Товар отсутствует в меню Lounge." << std::endl;
        return false;
    }

    // 2. Услуга Lounge должна быть куплена
    bool has_lounge = false;
    for (const auto* s : visitor.get_services()) {
        if (s == this) { has_lounge = true; break; }
    }
    if (!has_lounge) {
        std::cout << "Ошибка: У посетителя " << visitor.get_full_name() 
                  << " нет купленной услуги Lounge." << std::endl;
        return false;
    }

    // 3. Проверка возраста для алкоголя
    if (dynamic_cast<AlcoholDrinks*>(item) != nullptr) {
        if (visitor.get_age() < 18) {
            std::cout << "Ошибка: Посетителю " << visitor.get_full_name() 
                      << " меньше 18 лет, продажа алкоголя запрещена. (Услуга Lounge сохранена)." << std::endl;
            return false; 
        }
    }

    // 4. Деньги
    if (visitor.get_money_count() < item->get_price()) {
        std::cout << "Ошибка: У посетителя " << visitor.get_full_name() 
                  << " недостаточно средств для покупки." << std::endl;
        return false;
    }

    // 5. Списываем деньги и пополняем кассу
    visitor.set_money_count(visitor.get_money_count() - item->get_price());
    Buffet::add_to_cash_register(item->get_price());

    std::cout << "Успех: Посетитель " << visitor.get_full_name() 
              << " купил в Lounge товар за " << item->get_price() << " руб." << std::endl;
    return true;
}
