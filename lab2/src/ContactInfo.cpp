#include "ContactInfo.h"

#include <cctype>
#include <utility>

ContactInfo::ContactInfo(std::string phone, std::string email)
    : phone(std::move(phone)), email(std::move(email)) {}

bool ContactInfo::is_valid_contact() const {
    // Email: есть '@', непустая локальная часть, есть '.' в домене
    auto at = email.find('@');
    bool valid_email = at != std::string::npos && at > 0 &&
                       email.find('.', at) != std::string::npos &&
                       email.find('.', at) > at + 1;

    // Phone: длина > 5 и только цифры/+/-/пробел/(/)
    bool valid_phone = phone.length() > 5;
    if (valid_phone) {
        for (char c : phone) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!std::isdigit(uc) && c != '+' && c != '-' &&
                c != ' ' && c != '(' && c != ')') {
                valid_phone = false;
                break;
            }
        }
    }
    return valid_email && valid_phone;
}
