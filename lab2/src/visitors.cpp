#include "visitors.h"
#include "inventory.h"
#include "exceptions.h"
#include <algorithm>
#include <cmath>
#include <utility>

double Buffet::cash_register = 0.0;
DailyStatistics* DailyStatistics::current = nullptr; 

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
    // Ищем услугу в векторе и удаляем её
    auto it = std::find(services.begin(), services.end(), service);
    if (it != services.end()) {
        services.erase(it);
    }
}

const std::vector<Favour*>& Visitor::get_services() const {return services;}
void Visitor::set_in_group(bool status)   { in_group = status; }
void Visitor::set_visitor_id(int id)      { visitor_id = id; }
void Visitor::set_money_count(double money) {
    if (!std::isfinite(money) || money < 0) {
        throw InvalidActionError("Visitor: Balance must be finite and nonnegative.");
    }
    money_count = money;
}

bool   Visitor::is_in_group()     const { return in_group; }
int    Visitor::get_visitor_id()  const { return visitor_id; }
double Visitor::get_money_count() const { return money_count; }  
int Visitor::get_age() const { return age; }
bool   Visitor::get_can_enter()   const { return can_enter; }
std::string Visitor::get_full_name() const { return full_name; }
Ticket* Visitor::get_ticket()     const { return ticket.get(); } 

GroupVisit::GroupVisit(int id) : group_id(id), can_enter(false) {}

void GroupVisit::add_visitor(Visitor* v) {
    if (!v) {
        throw NullPointerError("GroupVisit: Cannot add a null visitor to the group.");
    }

    if (v->is_in_group()) {
        throw DuplicateEntityError("GroupVisit: The visitor already belongs to a group.");
    }
    visitors.push_back(v);
    v->set_in_group(true);

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

void Lounge::add_to_menu(Buffet* item) { 
    if (item) menu.push_back(item); 
}

bool Lounge::buy_item(Visitor& visitor, Buffet* item) {
    if (!item) {
        throw NullPointerError("Lounge: Item pointer is null.");
    }

        // 1. Товар должен быть в меню
    if (std::find(menu.begin(), menu.end(), item) == menu.end()) {
        throw EntityNotFoundError("Lounge: Item is not on the menu.");
    }

    // 2. Услуга Lounge должна быть куплена
    bool has_lounge = false;
    for (const auto* s : visitor.get_services()) {
        if (s == this) { has_lounge = true; break; }
    }
    if (!has_lounge) {
        throw ServiceNotPurchasedError("Lounge: Visitor " + visitor.get_full_name() + " didn't buy the Lounge service.");
    }

    // 3. Проверка возраста для алкоголя
    if (dynamic_cast<AlcoholDrinks*>(item) != nullptr) {
        if (visitor.get_age() < 18) {
            throw InvalidActionError("Lounge: Underage visitor cannot buy alcohol. (Lounge service kept).");
        }
    }

    // 4. Деньги
    if (visitor.get_money_count() < item->get_price()) {
        throw InsufficientFundsError("Lounge: Not enough money for item. (Need: " 
                                     + std::to_string(item->get_price()) + ")");
    }

    // 5. Списываем деньги и пополняем кассу
    visitor.set_money_count(visitor.get_money_count() - item->get_price());
    Buffet::add_to_cash_register(item->get_price());
    if (auto* stats = DailyStatistics::get_current()) {
        stats->add_revenue(item->get_price());
    }

    std::cout << "Success: " << visitor.get_full_name() 
              << " bought an item in Lounge for " << item->get_price() << " rub." << std::endl;
    return true;
}

VipVisit::VipVisit(double price) : Favour(price) {}

VipVisit::Bundle* VipVisit::find_bundle(const Visitor& visitor) {
    for (auto& p : bundles) {
        if (p.first == &visitor) return &p.second;
    }
    return nullptr;
}

const VipVisit::Bundle* VipVisit::find_bundle(const Visitor& visitor) const {
    for (const auto& p : bundles) {
        if (p.first == &visitor) return &p.second;
    }
    return nullptr;
}

bool VipVisit::buy(Visitor& visitor) {
    // 0. Защита от повторной покупки тем же посетителем.
    if (find_bundle(visitor) != nullptr) {
        throw InvalidActionError(
            "VipVisit: Visitor " + visitor.get_full_name()
            + " already bought this VIP package.");
    }

    // 1. Списываем деньги за сам VIP-визит (добавляет `this` в services).
    if (!purchase(visitor)) {
        return false;
    }

    // 2. Создаём ПЕРСОНАЛЬНЫЙ бандл для этого посетителя.
    Bundle b;
    b.audio_guide = std::make_unique<AudioGuide>(0.0);
    for (int i = 0; i < 6; ++i) {
        b.lounges.push_back(std::make_unique<Lounge>(0.0));
    }

    // 3. Раздаём сырые указатели внутрь бандла. Владелец — VipVisit.
    visitor.add_service(b.audio_guide.get());
    for (auto& lounge : b.lounges) {
        visitor.add_service(lounge.get());
    }

    // 4. Сохраняем бандл. Порядок: сначала складываем в bundles, чтобы будущие find_bundle сразу его видели.
    bundles.emplace_back(&visitor, std::move(b));
    return true;
}

AudioGuide* VipVisit::get_audio_guide(const Visitor& visitor) const {
    const Bundle* b = find_bundle(visitor);
    return b ? b->audio_guide.get() : nullptr;
}

Lounge* VipVisit::get_lounge(const Visitor& visitor, int index) const {
    const Bundle* b = find_bundle(visitor);
    if (!b) return nullptr;
    if (index < 0 || index >= static_cast<int>(b->lounges.size())) return nullptr;
    return b->lounges[index].get();
}
