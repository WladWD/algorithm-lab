# Bit Tricks — Low-Level Bit Manipulation Primitives

## Overview

A collection of fundamental bitwise operations that form the building blocks of virtually every bit manipulation technique. These primitives are used throughout competitive programming, systems programming, and compiler intrinsics.

## Functions

### `popcount(x)` — Population Count

Counts the number of set (1) bits in an unsigned integer.

**Algorithm:** Delegates to `std::popcount` (C++20), which maps to the hardware `POPCNT` instruction on modern CPUs.

```
popcount(0b10110) = 3
popcount(0)       = 0
popcount(0xFF)    = 8
```

**Complexity:** O(1) with hardware support; O(log w) software fallback (w = word size).

---

### `is_power_of_two(x)` — Power-of-2 Check

Returns `true` if `x` is a positive power of two.

**Algorithm:** A power of two has exactly one set bit, so `x & (x - 1) == 0`. The check `x != 0` excludes zero.

```
is_power_of_two(16) = true   // 10000
is_power_of_two(15) = false  // 01111
is_power_of_two(0)  = false
```

**Complexity:** O(1).

**Proof:** If `x = 2^k`, its binary representation is `1` followed by `k` zeros. Then `x - 1` is `k` ones, so `x & (x - 1) = 0`. Conversely, if `x` has more than one set bit, clearing the lowest bit still leaves at least one, so the result is non-zero.

---

### `lowest_set_bit(x)` — Isolate Lowest Set Bit

Returns the value of the lowest set bit (i.e., a power of two), or 0 if `x == 0`.

**Algorithm:** `x & (-x)` — two's complement negation flips all bits and adds 1, which flips bits above the lowest set bit.

```
lowest_set_bit(0b10100) = 0b00100 = 4
lowest_set_bit(0b10000) = 0b10000 = 16
lowest_set_bit(0)       = 0
```

**Complexity:** O(1).

**Proof:** In two's complement, `-x = ~x + 1`. All bits below the lowest set bit of `x` are 0 — inverting gives 1s, and adding 1 carries through them, turning them back to 0 and setting the bit at position `p` (the lowest set bit). All bits above `p` are complemented. Thus `x & (-x)` isolates exactly bit `p`.

---

### `highest_set_bit(x)` — Isolate Highest Set Bit

Returns the value of the highest (most significant) set bit, or 0 if `x == 0`.

**Algorithm:** Uses `std::bit_width(x)` to find the position, then shifts `1` left to that position.

```
highest_set_bit(0b10110) = 0b10000 = 16
highest_set_bit(1)       = 1
highest_set_bit(0)       = 0
```

**Complexity:** O(1) with hardware CLZ/BSR support.

---

### `clear_lowest_set_bit(x)` — Turn Off Rightmost 1

Clears (sets to 0) the lowest set bit in `x`.

**Algorithm:** `x & (x - 1)` — subtracting 1 flips the lowest set bit and all trailing zeros.

```
clear_lowest_set_bit(0b10100) = 0b10000
clear_lowest_set_bit(0b10000) = 0b00000
clear_lowest_set_bit(0)       = 0
```

**Complexity:** O(1).

**Proof:** `x - 1` flips bit `p` (the lowest set bit) from 1 to 0, and sets all lower bits to 1. AND-ing with `x` zeroes out bit `p` and all lower bits (which were 0 in `x`), leaving only the bits above `p`. This is the classic "Brian Kernighan" technique — repeatedly applying it counts set bits.

**Note:** Repeatedly applying `clear_lowest_set_bit` is equivalent to iterating over set bits — this is how manual popcount is implemented:

```
count = 0
while x != 0:
    x = x & (x - 1)
    count++
```

## Implementation Notes

- All functions are `constexpr` and usable at compile time.
- Functions are templated on `std::unsigned_integral` (C++20 concepts) for type safety.
- Zero-input edge cases are handled explicitly and documented.

## References

- Hacker's Delight (Henry S. Warren Jr.) — the definitive reference for bit tricks.
- C++20 `<bit>` header — `std::popcount`, `std::bit_width`, `std::countr_zero`.
- Chess Programming Wiki — Bit-Twiddling section.

