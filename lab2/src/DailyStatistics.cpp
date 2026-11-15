#include "DailyStatistics.h"

#include <iostream>
#include <string>

DailyStatistics* DailyStatistics::current = nullptr;

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
