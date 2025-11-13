# Greatest Common Divisor (GCD) — Euclid's algorithm

## Overview

The greatest common divisor (GCD) of two integers `a` and `b` is the largest positive integer `d` that divides both. Euclid's algorithm computes the GCD efficiently by repeated remainder reduction.

## Problem statement

Given integers `a` and `b` (not both zero), compute

```
gcd(a, b) = max{ d ∈ Z_{>0} | d | a and d | b }.
```

We typically return a non-negative `gcd(a,b)` and use the convention `gcd(0,0) = 0` when needed.

## Euclid's algorithm

Key identity (division with remainder): for `a, b` with `b > 0` there exist `q, r` such that

```
a = b * q + r,  0 ≤ r < b.
```

Since `gcd(a,b) = gcd(b,r)`, replace `(a,b)` by `(b,r)` and repeat until `r = 0`.

### Iterative pseudocode

```
function gcd(a, b):
  a = |a|; b = |b|
  while b != 0:
    r = a % b
    a = b
    b = r
  return a
```

### Recursive pseudocode

```
function gcd(a, b):
  a = |a|; b = |b|
  if b == 0: return a
  return gcd(b, a % b)
```

Both versions produce the same result; the iterative form avoids call-stack overhead and is preferred in production code.

## Complexity

Euclid's algorithm runs in time proportional to the number of remainder steps. For integers `a` and `b` the number of division steps is `O(log min(a,b))` and more precisely bounded by a constant times the number of digits in the smaller argument. In practice it is extremely fast — tens to hundreds of nanoseconds for 64-bit inputs on modern CPUs.

## Implementation notes and pitfalls

- Work with absolute values for the algorithm, then restore signs for coefficients when returning from extended gcd.
- Handle zero cases explicitly: `gcd(a,0) = |a|`; `gcd(0,0)` can be defined as `0` for convenience.
- Avoid recursion depth issues: prefer iterative extended Euclid for production code.
- For very large integers (beyond 64-bit) use arbitrary-precision libraries (GMP, boost::multiprecision) and adapt types accordingly.
- For modular inverse and CRT, always check `gcd` before dividing or inverting to avoid undefined behavior.

## Examples

- `gcd(48, 18)`:
  - 48 = 18*2 + 12
  - 18 = 12*1 + 6
  - 12 = 6*2 + 0
  - gcd = 6

## References

- Euclid's Elements — classical source of Euclid's algorithm.
- CP-algorithms: GCD, extended Euclid and modular inverse explanations and code examples.