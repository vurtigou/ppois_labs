#pragma once

// Базовый класс для всей еды и напитков
/** @brief Base for priced food and drinks with a shared cash register.
 * @ingroup inventory
 */
class Buffet {
protected:
    double price;
    static double cash_register; // Статическая касса

public:
    explicit Buffet(double price = 0.0);
    virtual ~Buffet() = default;

    double get_price() const { return price; }

    // Методы для работы с кассой
    static double get_cash_register() { return cash_register; }
    static void add_to_cash_register(double amount) { cash_register += amount; }
};
