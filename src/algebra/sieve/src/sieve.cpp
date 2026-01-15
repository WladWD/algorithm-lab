#include "algebra/sieve/sieve.h"

namespace algebra::sieve {

std::vector<uint8_t> is_prime_up_to(std::size_t n) {
    // Use uint8_t instead of vector<bool> to keep semantics simple / predictable.
    std::vector<uint8_t> is_prime(n + 1, 1);

    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;

    // Classic sieve: mark multiples starting from p*p.
    for (std::size_t p = 2; p * 1ll * p <= n; ++p) {
        if (!is_prime[p]) continue;
        for (std::size_t x = p * p; x <= n; x += p) {
            is_prime[x] = 0;
        }
    }

    return is_prime;
}

std::vector<uint32_t> primes_up_to(std::size_t n) {
    if (n < 2) return {};

    auto is_prime = is_prime_up_to(n);
    std::vector<uint32_t> primes;
    primes.reserve(n / 10); // rough heuristic

    for (std::size_t i = 2; i <= n; ++i) {
        if (is_prime[i]) primes.push_back(static_cast<uint32_t>(i));
    }

    return primes;
}

} // namespace algebra::sieve

