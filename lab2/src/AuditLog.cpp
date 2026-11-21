#include "AuditLog.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// Возвращает текущее системное время в формате "YYYY-MM-DDTHH:MM:SS"
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

} // namespace

AuditLog::AuditLog(std::size_t max_size) : max_size(max_size) {
    if (max_size == 0) {
        throw std::invalid_argument("AuditLog: max_size must be > 0");
    }
}

// Добавление записи в журнал
void AuditLog::record_action(const std::string& action) {
    if (entries.size() >= max_size) {
        entries.erase(entries.begin());
    }
    entries.push_back(LogEntry{current_timestamp(), action});
}

// Вектор строк с записями, чья дата попадает в заданный диапазон
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

// Выводит отформатированно все записи журнала
void AuditLog::print_logs() const {
    std::cout << "--- Audit Log ---" << std::endl;
    for (const auto& e : entries) {
        std::cout << e.timestamp << "  " << e.action << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}
