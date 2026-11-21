#pragma once

#include <string>
#include <vector>

class Exhibit;
class Habitat;
class Tank;

/** @brief Area with an access level and one or more exhibits.
 * @ingroup habitats
 */
class Zone {
private:
    std::string zone_name;
    std::vector<Exhibit*> exhibits; // non-owning
    int access_level; // 0 — закрыто для всех, 1/2 — для сотрудников, 3 — для всех

public:
    Zone(std::string name = "", int access = 0);

    void restrict_access(int new_access_level);
    void list_tanks() const;

    void add_tank(Tank* tank);
    void remove_tank(Tank* tank, Habitat* backup_habitat);
    void evacuate_tank_animals(Tank* tank, Habitat* backup_habitat);

    void add_exhibit(Exhibit* exhibit);
    std::string const& get_zone_name() const { return zone_name; }
    int get_access_level() const { return access_level; }
    const std::vector<Exhibit*>& get_exhibits() const { return exhibits; }
};
