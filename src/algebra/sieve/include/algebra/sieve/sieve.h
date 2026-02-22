#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace algebra::sieve {

// Return a 0/1 primality table for integers in [0..n].
// Result size is n+1. For n < 1, returns a vector of size n+1 (i.e., 0->size 1) with is_prime[0]=0.
// Convention: 0 and 1 are not prime.
std::vector<uint8_t> is_prime_up_to(std::size_t n);

// Return all primes p such that 2 <= p <= n.
// For n < 2 returns an empty vector.
std::vector<uint32_t> primes_up_to(std::size_t n);

} // namespace algebra::sieve
