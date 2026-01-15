# Sieve of Eratosthenes

## Overview
The **Sieve of Eratosthenes** generates all prime numbers up to a given limit `n`.
It is one of the fastest practical methods for computing primes in a range `[2..n]`.

This module provides:
- `is_prime_up_to(n)` — a primality table for `0..n`.
- `primes_up_to(n)` — the list of primes up to `n`.

## Problem statement
Given an integer `n >= 0`, compute all primes `p` such that `2 <= p <= n`.

## Algorithm
1. Create an array `is_prime[0..n]` initialized to `true`.
2. Set `is_prime[0] = is_prime[1] = false`.
3. For each `p` from `2` to `floor(sqrt(n))`:
   - if `is_prime[p]` is true, mark all multiples `p*p, p*(p+1), ... <= n` as composite.

Starting from `p*p` is enough because any composite `< p*p` has a prime factor smaller than `p`.

## Complexity
- Time: `O(n log log n)`
- Memory: `O(n)`

## API
Header: `algebra/sieve/sieve.h`

```cpp
#include "algebra/sieve/sieve.h"

auto primes = algebra::sieve::primes_up_to(30);
// primes == {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}

auto is_prime = algebra::sieve::is_prime_up_to(10);
// is_prime[2] == 1, is_prime[4] == 0
```

## Notes / pitfalls
- For very large `n` the memory usage can dominate (`n+1` bytes in this implementation).
- We use `std::vector<uint8_t>` (not `std::vector<bool>`) to avoid surprising proxy semantics.

## References
- Eratosthenes of Cyrene — classical sieve method.
- CP-algorithms: Sieve of Eratosthenes.

