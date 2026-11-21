#pragma once

class Visitor;

// абстрактный класс
/** @brief Base for paid visitor services and their common purchase rules.
 * @ingroup visitors
 */
class Favour {
private:
    double price;

protected:
    // Общая логика покупки услуги (проверка билета, денег, списание, учёт)
    bool purchase(Visitor& visitor);

public:
    explicit Favour(double price = 0.0);
    virtual ~Favour() = default;

    double get_price() const { return price; }

    // Метод покупки: проверяет билет, деньги, списывает, добавляет услугу в вектор
    virtual bool buy(Visitor& visitor) = 0;
};
