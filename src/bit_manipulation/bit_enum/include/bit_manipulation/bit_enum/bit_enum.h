#pragma once

#include <concepts>
#include <cstdint>
#include <functional>
#include <vector>

namespace bit_manipulation::bit_enum {

/// Return the next integer with the same number of set bits (Gosper's hack).
/// Undefined when x == 0.
template <std::unsigned_integral T> constexpr T next_same_popcount(T x) noexcept {
    T lowest = x & static_cast<T>(-x);     // lowest set bit
    T ripple = x + lowest;                 // carry into next group
    T ones = ((x ^ ripple) >> 2) / lowest; // shifted-down correction bits
    return ripple | ones;
}

/// Enumerate every submask of `mask` (including mask itself and 0),
/// invoking `fn(submask)` for each one.
///
/// Iteration order: mask, mask-1 & mask, …, 0.
/// Total iterations: 2^popcount(mask).
template <std::unsigned_integral T, typename Fn>
    requires std::invocable<Fn, T>
constexpr void enumerate_submasks(T mask, Fn&& fn) {
    if (mask == 0) {
        fn(T{0});
        return;
    }
    for (T sub = mask;; sub = (sub - 1) & mask) {
        fn(sub);
        if (sub == 0)
            break;
    }
}

/// Collect all submasks of `mask` into a vector and return them.
template <std::unsigned_integral T> std::vector<T> collect_submasks(T mask) {
    std::vector<T> result;
    enumerate_submasks(mask, [&](T sub) { result.push_back(sub); });
    return result;
}

/// Gray-code encode: standard binary → reflected Gray code.
///   gray(n) = n ^ (n >> 1)
template <std::unsigned_integral T> constexpr T gray_encode(T n) noexcept {
    return n ^ (n >> 1);
}

/// Gray-code decode: reflected Gray code → standard binary.
///   Inverse of gray_encode.
template <std::unsigned_integral T> constexpr T gray_decode(T g) noexcept {
    T n = g;
    for (T bit = g >> 1; bit != 0; bit >>= 1) {
        n ^= bit;
    }
    return n;
}

/// Generate the full Gray code sequence for `num_bits` bits (2^num_bits entries).
std::vector<uint64_t> gray_code_sequence(unsigned num_bits);

} // namespace bit_manipulation::bit_enum
