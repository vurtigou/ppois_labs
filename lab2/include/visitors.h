#pragma once

#include <string>
#include <vector>
#include <stdexcept> 
#include <iostream>
#include <memory> 

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
    std::string get_full_name() const;

    void set_bought(bool status);
    void set_visitor_data(int id, const std::string& name);
};

class Visitor {
private:
    int visitor_id;
    std::string full_name;
    std::unique_ptr<Ticket> ticket;  
    double money_count;               
    bool can_enter;
    bool in_group;

public:
    Visitor(int id = 0, const std::string& name = "", double money = 0);

    Visitor(const Visitor&)            = delete;
    Visitor& operator=(const Visitor&) = delete;
    Visitor(Visitor&&)                 = default;
    Visitor& operator=(Visitor&&)      = default;

    std::unique_ptr<Ticket> buy_ticket(std::unique_ptr<Ticket> t);

    void fill_ticket_data();

    void set_in_group(bool status);
    void set_visitor_id(int id);
    void set_money_count(double money);

    bool   is_in_group() const;
    int    get_visitor_id() const;
    double get_money_count() const;
    bool   get_can_enter() const;
    std::string get_full_name() const;
    Ticket* get_ticket() const;       // non-owning view
};


class GroupVisit {
private:
    int group_id;
    std::vector<Visitor*> visitors;   // non-owning
    bool can_enter;

public:
    GroupVisit(int id = 0);

    GroupVisit(const GroupVisit&)            = delete;
    GroupVisit& operator=(const GroupVisit&) = delete;
    GroupVisit(GroupVisit&&)                 = default;
    GroupVisit& operator=(GroupVisit&&)      = default;

    void add_visitor(Visitor* v);
    bool check_can_enter();

    int  get_group_id() const;
    bool get_can_enter() const; 
    const std::vector<Visitor*>& get_visitors() const;
};

class DailyStatistics {
private:
    std::string date;           // Дата (в формате "YYYY-MM-DD")
    
    int tickets_sold;           // Всего продано билетов
    int visitors_count;         // Всего пришло посетителей (может отличаться от билетов, если есть дети до 5 лет)
    int groups_count;           // Количество групп
    int group_visitors;         // Количество человек в группах
    
    double total_revenue;       // Общая выручка за день

public:
    // Конструктор
    DailyStatistics(std::string date = "");
    void add_ticket_sale(double price, int count = 1);
    void add_visitors(int count = 1);
    void add_group(int visitors_in_group);

    // Геттеры
    std::string get_date() const { return date; }
    int get_tickets_sold() const { return tickets_sold; }
    int get_visitors_count() const { return visitors_count; }
    int get_groups_count() const { return groups_count; }
    int get_group_visitors() const { return group_visitors; }
    double get_total_revenue() const { return total_revenue; }

    // Вывод красивого отчета в консоль
    void print_report() const;
};

//абстрактный класс
class Favour {
private:
    double price;
    std::unique_ptr<Visitor> visitor;
public:
    //метод покупки (добавления visitor). передается ссылка на конкретного покупателя, проверяется у него наличие билета. затем проверяется money_count, если покупатель может позволить себе купить услугу, она покупается
};
class AudioGuide : public Favour {};
class Lounge : public Favour {};
class VipVisit : public Favour {};
