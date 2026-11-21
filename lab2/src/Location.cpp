#include "Location.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

// Возвращает пару координат вида "lat,lon"
std::pair<double, double> parse_coordinates(const std::string& coord) {
    auto comma = coord.find(',');
    if (comma == std::string::npos) {
        throw std::invalid_argument("Invalid coordinates format: '" + coord + "'");
    }
    try {
        double lat = std::stod(coord.substr(0, comma));
        double lon = std::stod(coord.substr(comma + 1));
        return {lat, lon};
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid coordinates values: '" + coord + "'");
    }
}

} // namespace

Location::Location(std::string city, std::string address, std::string coordinates)
    : city(std::move(city)),
      address(std::move(address)),
      coordinates(std::move(coordinates)) {}

// Считает расстояние между двумя локациями в километрах по формуле гаверсинуса
double Location::calculate_distance_to(const Location& other) const {
    if (coordinates.empty() || other.coordinates.empty()) {
        throw std::invalid_argument(
            "calculate_distance_to: coordinates are not set");
    }
    auto [lat1, lon1] = parse_coordinates(coordinates);
    auto [lat2, lon2] = parse_coordinates(other.coordinates);

    constexpr double PI = 3.14159265358979323846;
    constexpr double R  = 6371.0; // радиус Земли, км
    auto rad = [](double d) { return d * PI / 180.0; };

    double dlat = rad(lat2 - lat1);
    double dlon = rad(lon2 - lon1);
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(rad(lat1)) * std::cos(rad(lat2)) *
               std::sin(dlon / 2) * std::sin(dlon / 2);
    a = std::clamp(a, 0.0, 1.0);
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return R * c;
}
