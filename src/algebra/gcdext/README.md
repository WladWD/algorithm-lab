# Extended Euclidean Algorithm (GCD_EXT) 

## Overview

The extended Euclidean algorithm extends the classical Euclid algorithm for gcd(a, b) by computing integers `x` and `y` such that

```
a * x + b * y = g,
```

where `g = gcd(a, b)`. These coefficients are essential for modular arithmetic (modular inverses), solving Diophantine equations, and CRT constructions.

## Problem statement

Given integers `a` and `b` (not both zero), compute the triple `(g, x, y)` satisfying:

```
g = gcd(a, b)
and
a * x + b * y = g
```

We typically return `g >= 0` and choose `x`, `y` that satisfy the linear combination exactly.

## Extended Euclid: goal and identity

Euclid's key identity (division with remainder) is:

```
a = b * q + r,  0 ≤ r < |b|.
```

Because `gcd(a, b) = gcd(b, r)`, if we have representation for `g = gcd(b, r)` as `b * x1 + r * y1 = g`, substituting `r = a - b*q` yields coefficients for `a` and `b`:

```
g = b * x1 + (a - b*q) * y1 = a * y1 + b * (x1 - q*y1).
```

This recurrence underlies the extended algorithm: coefficients propagate backward as the remainder sequence is unwound.

## Algorithm (iterative)

Iterative extended Euclid uses the standard 'extended' form of the Euclidean division while maintaining coefficient pairs. Pseudocode (iterative):

```
function extended_gcd(a, b):
  // returns (g, x, y) with a*x + b*y = g
  a0 = |a|; b0 = |b|
  old_r = a0; r = b0
  old_s = 1; s = 0
  old_t = 0; t = 1
  while r != 0:
    q = old_r // r
    (old_r, r) = (r, old_r - q * r)
    (old_s, s) = (s, old_s - q * s)
    (old_t, t) = (t, old_t - q * t)
  // sign correction to match original a,b
  if a < 0: old_s = -old_s
  if b < 0: old_t = -old_t
  return (old_r, old_s, old_t)
```

### Recursive variant

```
function extended_gcd(a, b):
  if b == 0:
    return (|a|, sign(a), 0)
  (g, x1, y1) = extended_gcd(b, a % b)
  x = y1
  y = x1 - (a // b) * y1
  return (g, x, y)
```

The iterative version is preferred for production (no recursion depth). Both return coefficients that satisfy `a*x + b*y = g`.

## Proof sketch (correctness & invariant)

Let the remainder sequence be defined by repeated Euclidean division:

```
r_{-1} = |a|, r_0 = |b|,
r_{i-1} = q_i * r_i + r_{i+1},  0 ≤ r_{i+1} < r_i
```

The classical Euclidean algorithm stops at `r_{k+1} = 0` and `g = r_k`. The extended algorithm maintains integer pairs `(s_i, t_i)` such that

```
r_i = a0 * s_i + b0 * t_i
```

for all `i`. Base values are `r_{-1} = a0 = a0*1 + b0*0` and `r_0 = b0 = a0*0 + b0*1` so `(s_{-1}, t_{-1}) = (1,0)` and `(s_0, t_0) = (0,1)`.

When we compute `r_{i+1} = r_{i-1} - q_i * r_i` and substitute the representations for `r_{i-1}` and `r_i`, the coefficients update as

```
s_{i+1} = s_{i-1} - q_i * s_i
t_{i+1} = t_{i-1} - q_i * t_i
```

Thus the invariant holds. When the algorithm ends with `r_k = g`, the corresponding `(s_k, t_k)` satisfy `a*s_k + b*t_k = g`.

## Applications

### Modular inverse

If `g = gcd(a, m) = 1`, the modular inverse of `a` modulo `m` exists and equals `x mod m`, where `(g, x, y) = extended_gcd(a, m)`.

Pseudocode:

```
(x) = modinv(a, m):
  (g, x, y) = extended_gcd(a, m)
  if g != 1: error (no inverse)
  return (x % m + m) % m
```

### Linear Diophantine equations

To solve `a*x + b*y = c`, compute `(g, x0, y0) = extended_gcd(a, b)`.
A solution exists iff `g | c`. When it does, one particular solution is

```
x = x0 * (c / g),  y = y0 * (c / g)
```

General solution adds multiples of `(b/g, -a/g)`.

### CRT and solving congruences

Extended gcd is used to compute modular inverses and to merge congruences when performing CRT with possibly non-coprime moduli (via two-modulus merging using gcd checks).

## Complexity

Euclid's algorithm (and extended variant) performs `O(log min(|a|, |b|))` division steps; each step does constant-time arithmetic on machine-word integers. The algorithm is therefore extremely efficient for 64-bit integers. For arbitrary-precision integers, complexity is proportional to the cost of division and scales with operand size.

## Implementation notes and pitfalls

- Use absolute values for remainders, but restore signs for returned coefficients to satisfy the original `a`, `b` signs.
- For languages without guaranteed 128-bit intermediate safety, be careful when computing `q * s` or similar; use wide types if necessary.
- `extended_gcd(0, 0)` can be defined to return `(0, 0, 0)` or handled as a special case depending on your application.
- When using `modinv`, always check `g == 1` before using the returned coefficient.

## Examples

- `extended_gcd(30, 20)` → `(10, 1, -1)` because `30*1 + 20*(-1) = 10`.
- `modinv(3, 11)` → `4` because `3*4 ≡ 1 (mod 11)`.
- Solve `6*x + 15*y = 3`: `extended_gcd(6,15)` gives `(3, -2, 1)` so one solution is `x = -2 * (3/3) = -2`, `y = 1`.

## References

- CP-algorithms (Extended Euclid) — implementation notes and practical examples.
