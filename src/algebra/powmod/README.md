# Modular exponentiation (powmod) — professional guide

## Overview

Modular exponentiation (powmod) is a core primitive in number theory and cryptography. The naive method of computing `base^exponent` and reducing modulo `modulus` is infeasible because the intermediate power grows exponentially. The standard solution is binary exponentiation (exponentiation by squaring) performed under the modulus to keep values small.

## Problem statement

Compute

```
powmod(base, exponent, modulus) = base^{exponent} mod modulus
```

for integers `base`, `exponent >= 0`, and `modulus > 0`.

Return value lies in range `[0, modulus-1]`.

Edge cases to handle explicitly:
- `modulus == 1` → result is `0`.
- `exponent == 0` → result is `1 mod modulus` (provided modulus > 1).
- `base` may be negative — reduce `base` modulo `modulus` first.

## Binary exponentiation (repeated squaring)

Idea: write the exponent in binary and square the base while scanning exponent bits from least-significant to most-significant. Maintain an accumulator `res` initialized to `1`. For each bit of `exponent`:

- If the current bit is 1, set `res = res * base (mod modulus)`.
- Square `base = base * base (mod modulus)`.
- Shift exponent right by one bit.

When `exponent` becomes zero, `res` contains the result.

## Pseudocode

```text
powmod(base, exp, mod):
  base %= mod
  res = 1 % mod
  while exp > 0:
    if exp & 1: res = mul_mod(res, base, mod)
    base = mul_mod(base, base, mod)
    exp >>= 1
  return res
```

`mul_mod(x, y, mod)` must compute `(x * y) % mod` without overflow. On 64-bit hosts prefer `__int128`.

## Variant: very large exponents (big integers / decimal strings)

When `exponent` doesn't fit in machine integers (e.g., given as a decimal string), use the decomposition

```
base^{E} = base^{d_0 + 10*d_1 + 10^2*d_2 + ...} = ∏_{i}( base^{10^i * d_i} )
```

Compute iteratively reading digits left-to-right using the identity:

```
res = 1
for each digit d in decimal exponent E:
  res = powmod(res, 10, mod) * powmod(base, d, mod) mod mod
```

This uses `powmod(res, 10, mod)` (small exponent 10) and `powmod(base, d, mod)` with `d` in `[0..9]`. Complexity is `O(len(E) * log mod)`.

## Fermat / Euler reductions

When `gcd(base, modulus) = 1` and `modulus` is prime `p`, Fermat's little theorem gives

```
base^{p-1} ≡ 1 (mod p)
```

so exponents can be reduced modulo `p-1`:

```
base^{e} mod p = base^{e mod (p-1)} mod p
```

More generally, when `gcd(base, modulus) = 1` and `phi = φ(modulus)` is Euler's totient, Euler's theorem gives

```
base^{phi} ≡ 1 (mod modulus)
```

so exponents may be reduced modulo `phi` when safe. Be careful: reducing exponent modulo `phi` requires `gcd(base, modulus)=1`, and when exponent is small you must not reduce incorrectly. For CRT and mixed-modulus reconstructions, apply reductions per modulus.

## Complexity

Binary exponentiation runs in `O(log exponent)` modular multiplications. Each modular multiplication cost depends on the chosen `mul_mod` implementation: using `__int128` on 64-bit platforms gives constant-time multiplications; Montgomery gives faster repeated multiplications for large batches.

## Implementation notes and pitfalls

- Always reduce `base` modulo `modulus` at the start to keep numbers small.
- Use unsigned types for bit-shifts on exponents; prefer `uint64_t` for exponent when it fits.
- For 64-bit `modulus`, use `__int128` to compute `(a * b) % mod` safely:
  ```cpp
  uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__int128)a * b % mod);
  }
  ```
- If `__int128` is not available (non-GCC/Clang), implement `mul_mod` via Montgomery or use a portable 128-bit library.
- Beware `modulus == 0` (undefined) and `modulus == 1` (result 0).
- Avoid naive `pow` from `<cmath>` — it's floating point and imprecise for integers.

## C++ sketches

### Portable powmod using __int128

```cpp
#include <cstdint>

inline uint64_t mul_mod_u128(uint64_t a, uint64_t b, uint64_t mod) {
  return (uint64_t)((__int128)a * b % mod);
}

uint64_t powmod_u128(uint64_t base, uint64_t exp, uint64_t mod) {
  if (mod == 1) return 0;
  base %= mod;
  uint64_t res = 1 % mod;
  while (exp > 0) {
    if (exp & 1) res = mul_mod_u128(res, base, mod);
    base = mul_mod_u128(base, base, mod);
    exp >>= 1;
  }
  return res;
}
```

### powmod with decimal-string exponent (big exponent)

```cpp
uint64_t powmod_bigexp(uint64_t base, const std::string &exp_dec, uint64_t mod) {
  base %= mod;
  uint64_t res = 1 % mod;
  for (char ch : exp_dec) {
    int d = ch - '0';
    // res = res^10 * base^d mod
    res = powmod_u128(res, 10, mod);
    if (d) res = mul_mod_u128(res, powmod_u128(base, d, mod), mod);
  }
  return res;
}
```

Note: `powmod_u128(base, d, mod)` is cheap because `d` in `[0..9]`.

## Examples

- `powmod(2, 10, 1000) = 1024 mod 1000 = 24`.
- `powmod(3, 1000000000, 1000000007)` — typical example, compute with `powmod_u128`.
- `powmod_bigexp(2, "10000000000000000000", 1000000007)` — exponent too large for 64-bit but handled by decimal exponent method.

## References
- Practical resources: CP-algorithms (modular arithmetic), crypto engineering texts (Montgomery reduction).
