#pragma once

#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <tuple>

namespace algebra::gcd {
// Standard gcd using Euclid's algorithm (iterative)
int64_t gcd(int64_t a, int64_t b);
} // namespace algebra::gcd
