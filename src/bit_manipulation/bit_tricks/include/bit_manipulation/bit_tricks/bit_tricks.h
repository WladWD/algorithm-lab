#pragma once

#include <bit>
#include <concepts>
#include <cstdint>

namespace bit_manipulation::bit_tricks {

/// Count the number of set bits (population count).
template <std::unsigned_integral T> constexpr int popcount(T x) noexcept {
    return std::popcount(x);
}

/// Check whether x is a power of two (0 returns false).
template <std::unsigned_integral T> constexpr bool is_power_of_two(T x) noexcept {
    return x != 0 && (x & (x - 1)) == 0;
}

/// Isolate the lowest set bit.  Returns 0 when x == 0.
template <std::unsigned_integral T> constexpr T lowest_set_bit(T x) noexcept {
    return x & static_cast<T>(-x);
}

/// Isolate the highest set bit.  Returns 0 when x == 0.
template <std::unsigned_integral T> constexpr T highest_set_bit(T x) noexcept {
    if (x == 0)
        return 0;
    return T{1} << (std::bit_width(x) - 1);
}

/// Clear the lowest set bit (turn off the rightmost 1).
template <std::unsigned_integral T> constexpr T clear_lowest_set_bit(T x) noexcept {
    return x & (x - 1);
}

} // namespace bit_manipulation::bit_tricks
