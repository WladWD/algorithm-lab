#pragma once

#include <cstdint>
#include <optional>

namespace algebra::mod_inv {
// Extended Euclidean algorithm: compute gcd(a,b) and coefficients x,y such that a*x + b*y == gcd
// Returns non-negative gcd. x and y are set to one solution (may be negative).
long long extgcd(long long a, long long b, long long& x, long long& y);

// Compute modular inverse of a modulo m using extended Euclidean algorithm.
// Returns std::optional<long long> containing inverse in [0, m-1] if it exists, or std::nullopt otherwise.
std::optional<long long> modinv_ext(long long a, long long m);

// Fast modular exponentiation: computes a^e mod mod. Uses __int128 to avoid overflow when multiplying 64-bit values.
long long modpow(long long a, long long e, long long mod);

// Compute modular inverse assuming `mod` is prime (Fermat's little theorem): a^(mod-2) mod mod
// Behavior is undefined if mod <= 1 or a % mod == 0; caller must ensure preconditions or check result.
long long modinv_fermat(long long a, long long mod);
} // namespace algebra::mod_inv
