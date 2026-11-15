#pragma once

#include <string>

class Animal;
class QuarantineTank;

class VetVisit {
private:
    Animal* animal; // non-owning
    std::string diagnosis;
    std::string treatment;
    bool is_closed;

public:
    VetVisit(Animal* a = nullptr, std::string diag = "", std::string treat = "");

    void diagnostic(const std::string& new_diagnosis, QuarantineTank* quarantine_tank = nullptr);
    void prescribe_treatment(const std::string& new_treatment, QuarantineTank* quarantine_tank = nullptr);
    void close_visit();

    std::string get_diagnosis() const { return diagnosis; }
    std::string get_treatment() const { return treatment; }
};
