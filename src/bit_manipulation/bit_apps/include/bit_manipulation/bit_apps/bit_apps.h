#pragma once

#include <concepts>
#include <cstdint>
#include <span>
#include <vector>

namespace bit_manipulation::bit_apps {

/// XOR-swap two values in place without a temporary variable.
/// Note: a and b must refer to distinct objects (aliasing causes zeroing).
template <std::unsigned_integral T> constexpr void xor_swap(T& a, T& b) noexcept {
    if (&a == &b)
        return; // self-swap guard
    a ^= b;
    b ^= a;
    a ^= b;
}

/// Find the single element that appears an odd number of times
/// in a collection where every other element appears an even number of times.
/// Returns 0 for an empty span.
uint64_t xor_find_unique(std::span<const uint64_t> data) noexcept;

/// Bit-packed Sieve of Eratosthenes.
/// Returns a vector<bool>-like bitset packed into uint64_t words,
/// where bit i of word (i/64) indicates whether i is prime.
/// sieve[i] is prime iff (result[i / 64] >> (i % 64)) & 1.
///
/// Uses bitwise operations for compact memory layout (~8x denser than
/// a byte-per-element sieve).
std::vector<uint64_t> bitwise_sieve(uint64_t n);

/// Helper: test whether `k` is prime according to a sieve produced
/// by bitwise_sieve(n) (k must be <= n).
inline bool is_prime_in_sieve(const std::vector<uint64_t>& sieve, uint64_t k) noexcept {
    return (sieve[k / 64] >> (k % 64)) & 1;
}

/// Count primes up to n using the bitwise sieve.
uint64_t count_primes(uint64_t n);

} // namespace bit_manipulation::bit_apps
