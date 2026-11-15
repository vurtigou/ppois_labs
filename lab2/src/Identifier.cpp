#include "Identifier.h"

#include <string>
#include <utility>

Identifier::Identifier(std::string prefix, int start_number)
    : prefix(std::move(prefix)), number(start_number) {}

// Возвращает строку "<prefix>-<number>", увеличивает счетчик на 1
std::string Identifier::generate_next() {
    std::string result = prefix + "-" + std::to_string(number);
    ++number;
    return result;
}
