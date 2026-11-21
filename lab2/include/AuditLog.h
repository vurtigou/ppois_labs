#pragma once

#include <cstddef>
#include <string>
#include <vector>

/** @brief Timestamped record of an oceanarium action.
 * @ingroup core
 */
struct LogEntry {
    std::string timestamp; // "YYYY-MM-DDTHH:MM:SS"
    std::string action;
};

/** @brief Size-limited event history with date-based filtering.
 * @ingroup core
 */
class AuditLog {
private:
    std::vector<LogEntry> entries;
    std::size_t max_size;

public:
    // Бросает std::invalid_argument при max_size == 0
    explicit AuditLog(std::size_t max_size = 1000);

    void record_action(const std::string& action);

    // Даты в формате "YYYY-MM-DD" (включительно). Пустые границы — без ограничения.
    /**
     * @brief Returns formatted entries between inclusive dates.
     * @param start_date First date in YYYY-MM-DD format, or empty for an open start.
     * @param end_date Last date in YYYY-MM-DD format, or empty for an open end.
     */
    std::vector<std::string> filter_by_period(const std::string& start_date,
                                              const std::string& end_date) const;

    void print_logs() const;

    std::size_t size() const { return entries.size(); }
};
