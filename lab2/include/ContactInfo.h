#pragma once

#include <string>

class ContactInfo {
private:
    std::string phone;
    std::string email;

public:
    ContactInfo(std::string phone = "", std::string email = "");

    bool is_valid_contact() const;

    std::string get_phone() const { return phone; }
    std::string get_email() const { return email; }
};
