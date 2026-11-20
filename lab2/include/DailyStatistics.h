#pragma once

#include <string>

/** @brief Tracks daily ticket sales, admissions, groups, and revenue.
 * @ingroup visitors
 */
class DailyStatistics {
private:
    std::string date;           // Дата (в формате "YYYY-MM-DD")

    int tickets_sold;           // Всего продано билетов
    int visitors_count;         // Всего пришло посетителей
    int groups_count;           // Количество групп
    int group_visitors;         // Количество человек в группах

    double total_revenue;       // Общая выручка за день
    static DailyStatistics* current;

public:
    DailyStatistics(std::string date = "");
    /**
     * @brief Sets the active, non-owning statistics sink for transactions.
     * @note The caller must clear or replace the sink before destroying it.
     */
    static void set_current(DailyStatistics* stats) { current = stats; }
    static DailyStatistics* get_current() { return current; }
    void add_ticket_sale(double price, int count = 1);
    void add_visitors(int count = 1);
    void add_group(int visitors_in_group);
    void add_revenue(double amount);

    // Геттеры
    std::string get_date() const { return date; }
    int get_tickets_sold() const { return tickets_sold; }
    int get_visitors_count() const { return visitors_count; }
    int get_groups_count() const { return groups_count; }
    int get_group_visitors() const { return group_visitors; }
    double get_total_revenue() const { return total_revenue; }

    void print_report() const;
};
