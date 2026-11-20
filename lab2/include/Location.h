#pragma once

#include <string>

/** @brief Named location with optional coordinates for distance calculations.
 * @ingroup core
 */
class Location {
private:
    std::string city;
    std::string address;
    std::string coordinates; // формат "lat,lon"

public:
    Location(std::string city = "", std::string address = "", std::string coordinates = "");

    // Haversine. Бросает std::invalid_argument, если координаты не заданы/битые.
    double calculate_distance_to(const Location& other) const;

    std::string get_city()        const { return city; }
    std::string get_address()     const { return address; }
    std::string get_coordinates() const { return coordinates; }
    bool is_valid() const { return !city.empty(); }
};
