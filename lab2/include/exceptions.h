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

class NullPointerError : public OceanariumError {
public:
    explicit NullPointerError(const std::string& message) : OceanariumError(message) {}
};

class DuplicateEntityError : public OceanariumError {
public:
    explicit DuplicateEntityError(const std::string& message) : OceanariumError(message) {}
};

class UnsuitableHabitatError : public OceanariumError {
public:
    explicit UnsuitableHabitatError(const std::string& message) : OceanariumError(message) {}
};

class EntityNotFoundError : public OceanariumError {
public:
    explicit EntityNotFoundError(const std::string& message) : OceanariumError(message) {}
};
