#pragma once

#include <string>
#include <vector>

class Animal;
class Tank;

// выставочный зал
/** @brief Visitor display with open or closed status and a collection of tanks.
 * @ingroup habitats
 */
class Exhibit {
private:
    std::string title;
    std::string description;
    std::vector<Tank*> tanks; // non-owning
    bool opened;

public:
    Exhibit(std::string title = "", std::string description = "", bool is_open = false);

    void open_exhibit();
    void close_exhibit();

    // Функция, которая собирает всех животных из всех танков зала
    std::vector<Animal*> get_all_animals() const;

    void add_tank(Tank* tank);
    bool remove_tank(Tank* tank);
    bool is_open() const { return opened; }
    std::string const& get_title() const { return title; }
    const std::vector<Tank*>& get_tanks() const { return tanks; }
};
