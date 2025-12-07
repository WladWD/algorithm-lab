#pragma once

#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace algo::algebra::pie {

// Count numbers in [1..n] divisible by at least one of the given divisors
// using the Principle of Inclusion–Exclusion (PIE).
// - n: non-negative integer limit
// - divisors: list of positive divisors (zeros and negatives are ignored)
// Complexity: O(2^k) where k = number of non-trivial divisors. Use with k <= ~20.

inline uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

inline __int128 lcm_checked(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) return 0;
    uint64_t g = gcd_u64(a, b);
    __int128 res = (__int128)a / g * b;
    return res;
}

inline uint64_t count_divisible_leq(uint64_t n, const std::vector<uint64_t>& divisors_in) {
    // filter divisors: keep >0 and unique
    std::vector<uint64_t> divisors;
    divisors.reserve(divisors_in.size());
    for (auto d : divisors_in) {
        if (d == 0) continue;
        if (d > n) continue; // divisor larger than n contributes nothing
        divisors.push_back(d);
    }
    if (divisors.empty()) return 0;

    // remove duplicates
    std::sort(divisors.begin(), divisors.end());
    divisors.erase(std::unique(divisors.begin(), divisors.end()), divisors.end());

    // if there's a 1, all numbers are divisible
    if (!divisors.empty() && divisors[0] == 1) return n;

    const size_t k = divisors.size();
    if (k > 63) throw std::overflow_error("Too many divisors for PIE");

    uint64_t total = 0;
    uint64_t subsets = (k >= 63) ? 0 : (1ULL << k);
    for (uint64_t mask = 1; mask < subsets; ++mask) {
        __int128 cur_lcm = 1;
        int bits = 0;
        for (size_t i = 0; i < k; ++i) {
            if (mask & (1ULL << i)) {
                ++bits;
                cur_lcm = lcm_checked((uint64_t)cur_lcm, divisors[i]);
                if (cur_lcm == 0 || cur_lcm > static_cast<__int128>(n)) break; // contributes zero
            }
        }
        if (cur_lcm == 0 || cur_lcm > static_cast<__int128>(n)) continue;
        uint64_t cnt = static_cast<uint64_t>(n / static_cast<uint64_t>(cur_lcm));
        if (bits % 2 == 1)
            total += cnt;
        else
            total -= cnt;
    }
    return total;
}

} // namespace algo::algebra::pie

