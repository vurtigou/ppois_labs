#pragma once

#include <string>

/** @brief Phone and email information with basic format validation.
 * @ingroup core
 */
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
