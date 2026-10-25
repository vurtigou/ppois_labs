#pragma once

#include <stdexcept>
#include <string>

// Базовое исключение предметной области (Domain Exception).
// От него позже будут наследоваться все остальные специфичные ошибки.
class OceanariumError : public std::runtime_error {
public:
    explicit OceanariumError(const std::string& message) 
        : std::runtime_error(message) {}
};
