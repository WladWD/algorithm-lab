#pragma once

#include <vector>
#include <cstdint>
#include <tuple>
#include <stdexcept>
#include <type_traits>

namespace algebra::garner {
// Extended GCD: returns (g, x, y) with a*x + b*y = g = gcd(a,b)
std::tuple<int64_t, int64_t, int64_t> ext_gcd(int64_t a, int64_t b);

// Modular inverse: returns x in [0, mod-1] such that (a * x) % mod == 1.
// Throws std::invalid_argument if inverse doesn't exist (gcd != 1).
uint64_t modinv(uint64_t a, uint64_t mod);

// Compute Garner mixed-radix coefficients c[0..k-1] given residues a and pairwise-coprime moduli m.
// Returns vector c (same size as a). Throws on size mismatch or invalid inverses.
std::vector<uint64_t> garner_mixed(std::vector<uint64_t> a, const std::vector<uint64_t>& m);

// Reconstruct x modulo MOD using Garner. MOD must be > 0. If MOD equals the product M and M fits
// in the provided integer type, this returns canonical x mod M; otherwise pick a suitable MOD (e.g. 2^64 wrap).
uint64_t garner_mod(const std::vector<uint64_t>& a, const std::vector<uint64_t>& m, uint64_t MOD);
} // namespace algebra::garner
