#pragma once

class Animal;
class Habitat;

// заявка на перевод животного
class AnimalTransfer {
private:
    Animal* animal;              // non-owning
    Habitat* destination_habitat; // non-owning

public:
    // Создает заявку. Запоминает, кого переводят и куда
    AnimalTransfer(Animal* a = nullptr, Habitat* dest = nullptr);
    void approve_transfer();
};
