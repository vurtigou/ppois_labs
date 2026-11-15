#pragma once

#include <string>
#include <vector>

class Tank;
class Zone;

class HabitatMap {
private:
    std::vector<Zone*> zones; // non-owning
    double scale;

public:
    HabitatMap(double scale = 1.0);

    Tank* locate_tank(const std::string& tank_id) const;
    void add_zone(Zone* zone);
};
