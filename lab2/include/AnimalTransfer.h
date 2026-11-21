#pragma once

class Animal;
class Habitat;

// заявка на перевод животного
/** @brief Validates and applies the movement of an animal between habitats.
 * @ingroup animals
 */
class AnimalTransfer {
private:
    Animal* animal;              // non-owning
    Habitat* destination_habitat; // non-owning

public:
    // Создает заявку. Запоминает, кого переводят и куда
    AnimalTransfer(Animal* a = nullptr, Habitat* dest = nullptr);
    /**
     * @brief Validates the request and asks the destination habitat to admit the animal.
     * @throws InvalidActionError The animal cannot move or is already at the destination.
     * @throws UnsuitableHabitatError The destination fails the species requirements.
     */
    void approve_transfer();
};
