#pragma once

#include <string>

class Identifier {
private:
    std::string prefix;
    int number;

public:
    // start_number — номер, который будет выдан первым
    Identifier(std::string prefix = "ID", int start_number = 0);

    std::string generate_next();

    std::string get_prefix() const { return prefix; }
    int get_next_number() const { return number; }
};
