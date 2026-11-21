#pragma once

#include <string>

class Animal;
class QuarantineTank;

/** @brief Veterinary examination that records a diagnosis and treatment.
 * @ingroup animals
 */
class VetVisit {
private:
    Animal* animal; // non-owning
    std::string diagnosis;
    std::string treatment;
    bool is_closed;

public:
    VetVisit(Animal* a = nullptr, std::string diag = "", std::string treat = "");

    /**
     * @brief Sets a diagnosis and derives a treatment when further care is indicated.
     * @note Providing a quarantine tank allows the diagnosis to trigger admission.
     */
    void diagnostic(const std::string& new_diagnosis, QuarantineTank* quarantine_tank = nullptr);
    void prescribe_treatment(const std::string& new_treatment, QuarantineTank* quarantine_tank = nullptr);
    void close_visit();

    std::string const& get_diagnosis() const { return diagnosis; }
    std::string const& get_treatment() const { return treatment; }
};
