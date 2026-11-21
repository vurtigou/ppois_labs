#pragma once

#include <stdexcept>
#include <string>

// Базовое исключение предметной области (Domain Exception).
// От него позже будут наследоваться все остальные специфичные ошибки.
/** @brief Base exception for oceanarium operations.
 * @ingroup errors
 */
class OceanariumError : public std::runtime_error {
public:
    explicit OceanariumError(const std::string& message) 
        : std::runtime_error(message) {}
};

/** @brief A required object pointer was null.
 * @ingroup errors
 */
class NullPointerError : public OceanariumError {
public:
    explicit NullPointerError(const std::string& message) : OceanariumError(message) {}
};

/** @brief An object or record already exists in the relevant collection.
 * @ingroup errors
 */
class DuplicateEntityError : public OceanariumError {
public:
    explicit DuplicateEntityError(const std::string& message) : OceanariumError(message) {}
};

/** @brief A habitat fails an animal’s water or volume requirements.
 * @ingroup errors
 */
class UnsuitableHabitatError : public OceanariumError {
public:
    explicit UnsuitableHabitatError(const std::string& message) : OceanariumError(message) {}
};

/** @brief The requested domain object could not be found.
 * @ingroup errors
 */
class EntityNotFoundError : public OceanariumError {
public:
    explicit EntityNotFoundError(const std::string& message) : OceanariumError(message) {}
};

// Для действий, которые запрещены по правилам (например, двигать детеныша)
/** @brief The current domain state does not allow the requested action.
 * @ingroup errors
 */
class InvalidActionError : public OceanariumError {
public:
    explicit InvalidActionError(const std::string& message) : OceanariumError(message) {}
};

// Для попытки скрестить разных животных
/** @brief A breeding pair contains animals of different species.
 * @ingroup errors
 */
class SpeciesMismatchError : public OceanariumError {
public:
    explicit SpeciesMismatchError(const std::string& message) : OceanariumError(message) {}
};

/** @brief An employee is not authorized to service a device.
 * @ingroup errors
 */
class EmployeePermissionDeniedError : public OceanariumError {
public:
    explicit EmployeePermissionDeniedError(const std::string& message) : OceanariumError(message) {}
};

/** @brief A water measurement is outside its accepted range.
 * @ingroup errors
 */
class WaterQualityViolationError : public OceanariumError {
public:
    explicit WaterQualityViolationError(const std::string& message) : OceanariumError(message) {}
};

/** @brief The date fails calendar or range validation.
 * @ingroup errors
 */
class InvalidDateError : public OceanariumError {
public:
    explicit InvalidDateError(const std::string& message) : OceanariumError(message) {}
};

/** @brief A staffing or shift scheduling rule was violated.
 * @ingroup errors
 */
class StaffManagementError : public OceanariumError {
public:
    explicit StaffManagementError(const std::string& message) : OceanariumError(message) {}
};

// Исключение для поломки оборудования
/** @brief A tool is broken or cannot be used safely.
 * @ingroup errors
 */
class EquipmentFailureError : public OceanariumError {
public:
    explicit EquipmentFailureError(const std::string& message) : OceanariumError(message) {}
};

// Исключение для нехватки ресурсов (вода, батарейка)
/** @brief A consumable resource is unavailable.
 * @ingroup errors
 */
class ResourceDepletedError : public OceanariumError {
public:
    explicit ResourceDepletedError(const std::string& message) : OceanariumError(message) {}
};

// Исключение для нехватки денег у посетителя
/** @brief The visitor has insufficient funds for a purchase.
 * @ingroup errors
 */
class InsufficientFundsError : public OceanariumError {
public:
    explicit InsufficientFundsError(const std::string& message) : OceanariumError(message) {}
};

// Исключение, когда услуга не была куплена
/** @brief A visitor tried to use a service before purchasing it.
 * @ingroup errors
 */
class ServiceNotPurchasedError : public OceanariumError {
public:
    explicit ServiceNotPurchasedError(const std::string& message) : OceanariumError(message) {}
};
