#pragma once

#include "Favour.h"

class Headphones;
class Visitor;

/** @brief Paid audio guide that manages headset assignment and return.
 * @ingroup visitors
 */
class AudioGuide : public Favour {
private:
    Headphones* headphones;

public:
    AudioGuide(double price = 0.0) : Favour(price), headphones(nullptr) {}

    bool buy(Visitor& visitor) override { return purchase(visitor); }

    // Метод добавления наушников. Если переданные наушники есть и никому
    // не принадлежат — они привязываются к посетителю. Иначе посетителю
    // возвращается price услуги, а сама услуга удаляется из его вектора услуг.
    /**
     * @brief Assigns a headset to a visitor who purchased this guide.
     * @details If the requested headset cannot be assigned, the purchase is refunded when the visitor has no headset from this guide.
     * @throws ServiceNotPurchasedError The visitor does not hold this service.
     * @throws InvalidActionError The visitor must return an existing headset or the requested headset is unavailable.
     */
    void assign_headphones(Headphones* hp, Visitor& visitor);
    /**
     * @brief Frees the headset held by this visitor.
     * @throws InvalidActionError No headset from this guide is assigned to the visitor.
     */
    void return_headphones(const Visitor& visitor);
};
