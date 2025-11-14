# Modular inverse

## Overview

Given integers `a` and `m` (with `m >= 1`), a modular inverse of `a` modulo `m` is an integer `x` such that:

```text
a * x ≡ 1 (mod m)
```

When it exists we denote `x = a^{-1} (mod m)`.

## Existence

A modular inverse exists if and only if the greatest common divisor `gcd(a, m) == 1` (i.e., `a` and `m` are coprime). Equivalently, the linear Diophantine equation

```text
a * x + m * y = 1
```

has an integer solution exactly when `gcd(a, m) == 1`.

## Algorithms

Two practical methods are commonly used depending on the modulus:

1) Extended Euclidean Algorithm (general modulus)
- Works for any positive modulus `m`.
- Time complexity: O(log(min(a, m))) i.e. logarithmic in the values.
- Returns an inverse `x` in the range `[0, m-1]` when it exists; otherwise indicates non-existence.

2) Fermat's Little Theorem (prime modulus)
- Requires `m` to be prime and `a % m != 0`.
- Uses the identity `a^(m-2) ≡ a^(-1) (mod m)`.
- Compute via fast modular exponentiation in O(log m) time.

> For composite moduli (including prime powers) prefer the extended Euclid implementation unless you have a specialized method for that modulus.

## Example C++ implementations

The examples below are small, self-contained, and safe for typical competitive programming constraints. They use `long long` for values and `__int128` for intermediate multiplications when squaring or multiplying to avoid overflow on 64-bit ranges.

### Extended Euclidean approach

```cpp
// Extended Euclidean algorithm: compute inverse of a modulo m.
#include <cstdint>
#include <optional>

// returns gcd(a, b); also sets x, y such that a*x + b*y == gcd
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = (a >= 0) ? 1 : -1; // handle sign of a
        y = 0;
        return a >= 0 ? a : -a;
    }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// returns optional inverse in [0, m-1] or std::nullopt if it doesn't exist
std::optional<long long> modinv_ext(long long a, long long m) {
    if (m <= 0) return std::nullopt;
    a %= m;
    if (a < 0) a += m;
    long long x, y;
    long long g = extgcd(a, m, x, y);
    if (g != 1) return std::nullopt; // inverse doesn't exist
    long long inv = (x % m + m) % m;
    return inv;
}
```

Notes:
- This implementation returns `std::optional<long long>` to encode the case when inverse doesn't exist.
- `extgcd` is written to be robust with signs and returns a non-negative gcd.

### Fermat's method for prime modulus

```cpp
// Modular exponentiation and Fermat inverse (requires prime mod)
#include <cstdint>

long long modpow(long long a, long long e, long long mod) {
    long long res = 1;
    a %= mod;
    if (a < 0) a += mod;
    while (e > 0) {
        if (e & 1) res = (long long)((__int128)res * a % mod);
        a = (long long)((__int128)a * a % mod);
        e >>= 1;
    }
    return res;
}

// returns inverse assuming mod is prime and a % mod != 0
long long modinv_fermat(long long a, long long mod) {
    return modpow(a, mod - 2, mod);
}
```

Caveat: do **not** use Fermat method if `mod` is composite; it will produce incorrect results.

## Examples

- Inverse of `3` modulo `11`:

```text
3 * 4 = 12 ≡ 1 (mod 11)  =>  3^{-1} mod 11 = 4
```

- Inverse of `2` modulo `6`:

```text
gcd(2, 6) = 2 != 1  => inverse does not exist
```

## Complexity

- Extended Euclid: O(log m) operations.
- Modular exponentiation (binary exponentiation): O(log exponent) where exponent is `m - 2` for Fermat — so O(log m) multiplications.

Both approaches perform a small number of modular multiplications; use 128-bit intermediates for safety if values can approach 64-bit limits.

## Practical notes & pitfalls

- Always reduce `a` modulo `m` before computing the inverse: `a %= m; if (a < 0) a += m;`.
- For composite `m` Fermat's method is invalid; use extended Euclid or specialized algorithms for prime powers.
- Decide how to signal non-existence: return `std::optional`, `-1`, throw, or document precondition `gcd(a,m)==1`.
- Be careful with negative inputs; normalize to `[0, m-1]` when returning the inverse.
- Watch overflow in modular multiplication; prefer `__int128` or modular multiplication helpers for 64-bit moduli.
