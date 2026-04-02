# Bit Applications — XOR Tricks & Bitwise Sieve

## Overview

Practical applications of bitwise operations: XOR-based algorithms for finding unique elements and swapping values, plus a memory-efficient bit-packed Sieve of Eratosthenes.

## Algorithms

### XOR Swap — `xor_swap(a, b)`

Swap two unsigned integers in place using three XOR operations, without a temporary variable.

#### Algorithm

```
a ^= b
b ^= a    // b now holds original a
a ^= b    // a now holds original b
```

#### Proof of correctness

Let `a₀` and `b₀` denote the original values.

1. After `a ^= b`: `a = a₀ ^ b₀`, `b = b₀`.
2. After `b ^= a`: `b = b₀ ^ (a₀ ^ b₀) = a₀`, `a = a₀ ^ b₀`.
3. After `a ^= b`: `a = (a₀ ^ b₀) ^ a₀ = b₀`.

Result: `a = b₀`, `b = a₀`. ∎

**Caveat:** If `a` and `b` alias the same object, all three XORs yield 0. The implementation includes a self-swap guard.

#### Complexity

O(1).

---

### XOR Find Unique — `xor_find_unique(data)`

Find the single element that appears an odd number of times in a collection where every other element appears an even number of times.

#### Algorithm

XOR all elements together. Since `x ^ x = 0` and `x ^ 0 = x`, paired elements cancel, leaving only the unique one.

#### Proof of correctness

XOR is commutative and associative. For any value `v` appearing `2k` times, the XOR of those occurrences is `0` (since `v ^ v = 0`). The remaining value `u` (appearing `2k+1` times) contributes `u` to the total XOR. ∎

#### Complexity

O(n) time, O(1) space.

#### Example

```
data = [2, 3, 5, 3, 2]
result = 2 ^ 3 ^ 5 ^ 3 ^ 2 = 5
```

---

### Bitwise Sieve — `bitwise_sieve(n)`

A memory-efficient Sieve of Eratosthenes that packs primality flags into 64-bit words (1 bit per integer), using ~8× less memory than a byte-per-element sieve.

#### Algorithm

1. Allocate `⌈(n+1)/64⌉` words, each initialized to all 1s.
2. Clear bits 0 and 1 (not prime).
3. For each `i` from 2 to `√n`, if bit `i` is set, clear all bits at positions `i²`, `i²+i`, `i²+2i`, etc.
4. Clear excess bits beyond `n` in the last word.

#### Marking composites

```
sieve[j / 64] &= ~(1ULL << (j % 64))
```

#### Testing primality

```
is_prime = (sieve[k / 64] >> (k % 64)) & 1
```

#### Proof of correctness

The algorithm is a faithful implementation of the Sieve of Eratosthenes with a bitwise storage optimization. The correctness follows from the standard sieve proof:
- Every composite number `c ≤ n` has a prime factor `p ≤ √n`.
- When processing prime `p`, we mark all multiples `p², p²+p, ...` as composite.
- Starting from `p²` is sufficient because all smaller multiples have already been handled by smaller primes.
- Therefore, every composite is cleared and every prime survives. ∎

#### Complexity

- Time: O(n log log n) — same as the standard Sieve of Eratosthenes.
- Space: O(n / 64) words = O(n / 8) bytes.

#### Example

```
bitwise_sieve(20) produces a bit vector where bits 2, 3, 5, 7, 11, 13, 17, 19 are set.
count_primes(20) = 8.
```

---

### Count Primes — `count_primes(n)`

Count the number of primes up to `n` by generating the bitwise sieve and summing `std::popcount` across all words.

#### Complexity

O(n log log n) for sieve construction + O(n / 64) for counting = O(n log log n) total.

## Implementation Notes

- The bitwise sieve is significantly more cache-friendly than `std::vector<bool>` or `std::vector<char>` for large `n` due to higher data density per cache line.
- `xor_find_unique` works for any XOR-cancellable type (unsigned integers); the implementation uses `uint64_t`.
- All functions handle edge cases (empty input, self-swap, n < 2).

## References

- XOR swap: standard bit-manipulation technique; see Hacker's Delight §2-19.
- XOR find unique: classic interview/competitive programming problem (LeetCode 136).
- Sieve of Eratosthenes: CP-Algorithms — bit-packed variant.
- `std::popcount` (C++20 `<bit>`) for efficient word-level population count.

