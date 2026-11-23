#pragma once

#include <stdexcept>
#include <string>

class GraphException : public std::runtime_error {
public:
    explicit GraphException(const std::string& message);
};
