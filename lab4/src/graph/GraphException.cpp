#include "graph/GraphException.h"

GraphException::GraphException(const std::string& message)
    : std::runtime_error(message) {}
