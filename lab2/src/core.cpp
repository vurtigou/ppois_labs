#include "core.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

// Внутренние хелперы
namespace {

std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &t);
#else
    localtime_r(&t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

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

// ContactInfo
ContactInfo::ContactInfo(std::string phone, std::string email)
    : phone(std::move(phone)), email(std::move(email)) {}

bool ContactInfo::is_valid_contact() const {
    // Email: есть '@', непустая локальная часть, есть '.' в домене
    auto at = email.find('@');
    bool valid_email = at != std::string::npos && at > 0 &&
                       email.find('.', at) != std::string::npos &&
                       email.find('.', at) > at + 1;

    // Phone: длина > 5 и только цифры/+/-/пробел/(/)
    bool valid_phone = phone.length() > 5;
    if (valid_phone) {
        for (char c : phone) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!std::isdigit(uc) && c != '+' && c != '-' &&
                c != ' ' && c != '(' && c != ')') {
                valid_phone = false;
                break;
            }
        }
    }
    return valid_email && valid_phone;
}

// Identifier
Identifier::Identifier(std::string prefix, int start_number)
    : prefix(std::move(prefix)), number(start_number) {}

std::string Identifier::generate_next() {
    std::string result = prefix + "-" + std::to_string(number);
    ++number;
    return result;
}

// Location
Location::Location(std::string city, std::string address, std::string coordinates)
    : city(std::move(city)),
      address(std::move(address)),
      coordinates(std::move(coordinates)) {}

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
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return R * c;
}

// AuditLog
AuditLog::AuditLog(std::size_t max_size) : max_size(max_size) {
    if (max_size == 0) {
        throw std::invalid_argument("AuditLog: max_size must be > 0");
    }
}

void AuditLog::record_action(const std::string& action) {
    if (entries.size() >= max_size) {
        entries.erase(entries.begin());
    }
    entries.push_back(LogEntry{current_timestamp(), action});
}

std::vector<std::string> AuditLog::filter_by_period(const std::string& start_date,
                                                    const std::string& end_date) const {
    std::vector<std::string> result;
    for (const auto& e : entries) {
        // "YYYY-MM-DD" — 10 символов
        std::string date = e.timestamp.substr(0, 10);
        if (!start_date.empty() && date < start_date) continue;
        if (!end_date.empty()   && date > end_date)   continue;
        result.push_back(e.timestamp + " " + e.action);
    }
    return result;
}

void AuditLog::print_logs() const {
    std::cout << "--- Audit Log ---" << std::endl;
    for (const auto& e : entries) {
        std::cout << e.timestamp << "  " << e.action << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}

// Oceanarium
Oceanarium::Oceanarium(std::string name, const Location& location)
    : name(std::move(name)), location(location) {

    if (this->name.empty()) {
        throw OceanariumError(
            "Ошибка создания: Название океанариума не может быть пустым.");
    }
    if (!this->location.is_valid()) {
        throw OceanariumError(
            "Ошибка создания: Локация океанариума некорректна (пустой город).");
    }

    audit_log.record_action("Oceanarium '" + this->name + "' created.");
}

Oceanarium::~Oceanarium() = default;

void Oceanarium::open_for_day() {
    std::cout << "Oceanarium " << name << " is now OPEN." << std::endl;
    audit_log.record_action("Oceanarium opened for the day.");
}

void Oceanarium::close_for_day() {
    std::cout << "Oceanarium " << name << " is now CLOSED." << std::endl;
    audit_log.record_action("Oceanarium closed for the day.");
}

void Oceanarium::transfer_animal_between_tanks(const std::string& animal_id,
                                               Tank* from_tank,
                                               Tank* to_tank) {
    if (!from_tank || !to_tank) {
        throw OceanariumError(
            "Ошибка перевода: Указатель на исходный или целевой танк равен nullptr.");
    }
    if (from_tank == to_tank) {
        throw OceanariumError(
            "Ошибка перевода: Исходный и целевой танк совпадают.");
    }
    if (std::find(tanks.begin(), tanks.end(), from_tank) == tanks.end()) {
        throw OceanariumError(
            "Ошибка перевода: Исходный танк не зарегистрирован в этом океанариуме.");
    }
    if (std::find(tanks.begin(), tanks.end(), to_tank) == tanks.end()) {
        throw OceanariumError(
            "Ошибка перевода: Целевой танк не зарегистрирован в этом океанариуме.");
    }

    // TODO: как только у Tank появятся методы contains_animal / remove_animal /
    // add_animal — здесь нужно будет проверить существование животного,
    // вместимость и совместимость видов (TankCapacityExceededError и т.п.),
    // а затем фактически перенести.

    // Реальный перенос ещё не реализован. Нельзя рапортовать об успехе
    // и писать в audit_log: это ввело бы в заблуждение пользователя
    // и самого океанариум (лог «врёт»).
    throw OceanariumError(
        "Ошибка перевода: перенос животных между танками ещё не реализован.");
}

void Oceanarium::add_tank(Tank* tank) {
    if (!tank) {
        throw OceanariumError(
            "Ошибка добавления: Попытка добавить пустой указатель на Tank.");
    }
    if (std::find(tanks.begin(), tanks.end(), tank) != tanks.end()) {
        throw OceanariumError(
            "Ошибка добавления: Этот танк уже зарегистрирован в океанариуме.");
    }
    tanks.push_back(tank);
    audit_log.record_action("A new tank was added to Oceanarium.");
}

void Oceanarium::add_employee(Employee* employee) {
    if (!employee) {
        throw OceanariumError(
            "Ошибка найма: Попытка добавить пустой указатель на Employee.");
    }
    if (std::find(employees.begin(), employees.end(), employee) != employees.end()) {
        throw OceanariumError(
            "Ошибка найма: Этот сотрудник уже работает в океанариуме.");
    }
    employees.push_back(employee);
    audit_log.record_action("A new employee was hired.");
}
