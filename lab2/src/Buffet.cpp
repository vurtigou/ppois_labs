#include "Buffet.h"

#include <cmath>

#include "exceptions.h"   // InvalidActionError

double Buffet::cash_register = 0.0;

Buffet::Buffet(double price) : price(price) {
    if (!std::isfinite(price) || price < 0.0) {
        throw InvalidActionError("Buffet: Price must be finite and nonnegative.");
    }
}
