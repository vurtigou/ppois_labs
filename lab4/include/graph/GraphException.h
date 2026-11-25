#pragma once

#include <stdexcept>
#include <string>

/// Reports invalid graph operations and iterator boundaries.
class GraphException : public std::runtime_error {
public:
    explicit GraphException(const std::string& message);
};
