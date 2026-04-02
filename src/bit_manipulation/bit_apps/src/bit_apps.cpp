#include "bit_manipulation/bit_apps/bit_apps.h"

#include <bit>
#include <cmath>

namespace bit_manipulation::bit_apps {

uint64_t xor_find_unique(std::span<const uint64_t> data) noexcept {
    uint64_t result = 0;
    for (auto v : data) {
        result ^= v;
    }
    return result;
}

std::vector<uint64_t> bitwise_sieve(uint64_t n) {
    // Number of 64-bit words needed to cover indices 0..n.
    const uint64_t num_words = n / 64 + 1;
    // Initialize all bits to 1 (assume prime), then clear non-primes.
    std::vector<uint64_t> sieve(num_words, ~uint64_t{0});

    // 0 and 1 are not prime.
    if (n >= 0) sieve[0] &= ~uint64_t{1};          // clear bit 0
    if (n >= 1) sieve[0] &= ~(uint64_t{1} << 1);   // clear bit 1

    const uint64_t limit = static_cast<uint64_t>(std::sqrt(static_cast<double>(n)));
    for (uint64_t i = 2; i <= limit; ++i) {
        if ((sieve[i / 64] >> (i % 64)) & 1) {
            // i is prime — mark all multiples as composite.
            for (uint64_t j = i * i; j <= n; j += i) {
                sieve[j / 64] &= ~(uint64_t{1} << (j % 64));
            }
        }
    }

    // Clear any excess bits beyond n in the last word.
    const uint64_t excess = (n + 1) % 64;
    if (excess != 0) {
        sieve.back() &= (uint64_t{1} << excess) - 1;
    }

    return sieve;
}

uint64_t count_primes(uint64_t n) {
    if (n < 2) return 0;
    auto sieve = bitwise_sieve(n);
    uint64_t count = 0;
    for (auto word : sieve) {
        count += static_cast<uint64_t>(std::popcount(word));
    }
    return count;
}

} // namespace bit_manipulation::bit_apps

