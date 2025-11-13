#pragma once

#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <tuple>

namespace algebra::gcdext {
// Iterative extended Euclidean algorithm.
// Returns (g, x, y) such that a*x + b*y = g = gcd(a,b).
// Guarantees g >= 0. Works for signed 64-bit integers and returns coefficients in signed 64-bit.
std::tuple<int64_t, int64_t, int64_t> extended_gcd(int64_t a, int64_t b);
} // namespace algebra::gcdext
