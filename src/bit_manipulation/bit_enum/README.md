# Bit Enumeration — Subset Enumeration & Gray Code

## Overview

Algorithms for enumerating and encoding bit patterns: iterating over subsets of a bitmask, generating the next permutation of bits (Gosper's hack), and converting between binary and Gray code.

## Algorithms

### Gosper's Hack — `next_same_popcount(x)`

Given an unsigned integer `x`, produce the **next** integer (in ascending order) that has the same number of set bits. This is the classic technique for enumerating all k-element subsets of an n-element set.

#### Algorithm

```
lowest  = x & (-x)                         // isolate lowest set bit
ripple  = x + lowest                        // carry propagation clears a run of 1s
ones    = ((x ^ ripple) >> 2) / lowest      // shifted-down correction bits
result  = ripple | ones
```

#### Proof of correctness

Let `x` have its lowest set bit at position `p`, and a run of `r` consecutive 1-bits starting at `p`.

1. `lowest = 2^p`.
2. Adding `lowest` to `x` carries through the run of `r` ones, setting bit `p + r` and clearing bits `p..p+r-1`.
3. The XOR `x ^ ripple` captures exactly the `r + 1` bits that changed (positions `p` through `p + r`).
4. Right-shifting by 2 and dividing by `lowest` moves `r - 1` correction bits to the bottom.
5. OR-ing them in restores the popcount: the result has the same number of set bits as `x`.

**Invariant:** `popcount(next_same_popcount(x)) == popcount(x)` for all `x > 0`.

#### Complexity

O(1) per step.

#### Example

Enumerate all 3-element subsets of {0,1,2,3,4}:

```
x = 0b00111 (7)  → {0,1,2}
x = 0b01011 (11) → {0,1,3}
x = 0b01101 (13) → {0,2,3}
x = 0b01110 (14) → {1,2,3}
x = 0b10011 (19) → {0,1,4}
x = 0b10101 (21) → {0,2,4}
x = 0b10110 (22) → {1,2,4}
x = 0b11001 (25) → {0,3,4}
x = 0b11010 (26) → {1,3,4}
x = 0b11100 (28) → {2,3,4}
```

Total: C(5,3) = 10 subsets.

---

### Submask Enumeration — `enumerate_submasks(mask, fn)`

Iterate over all submasks of a given bitmask (including `mask` itself and `0`).

#### Algorithm

```
for sub = mask; ; sub = (sub - 1) & mask:
    process(sub)
    if sub == 0: break
```

#### Proof of correctness

- `sub - 1` flips the lowest set bit of `sub` and sets all lower bits.
- AND-ing with `mask` restricts the result to only bits present in `mask`.
- Each step strictly decreases `sub` (treating it as an integer), so the loop terminates.
- Every submask of `mask` is visited exactly once because each decrement-and-mask step produces the largest submask strictly less than `sub`.

**Termination:** After processing `sub = 0`, the expression `(0 - 1) & mask = mask` would restart the loop, so we break explicitly.

#### Complexity

O(2^k) where k = popcount(mask). This is optimal since there are exactly 2^k submasks.

#### Example

```
mask = 0b1010 (10)
submasks: 0b1010 (10), 0b1000 (8), 0b0010 (2), 0b0000 (0)
```

---

### Gray Code — `gray_encode(n)` / `gray_decode(g)`

The **reflected binary Gray code** is a reordering of integers such that successive values differ in exactly one bit.

#### Encoding

```
gray(n) = n ^ (n >> 1)
```

#### Decoding

```
function gray_decode(g):
    n = g
    bit = g >> 1
    while bit != 0:
        n ^= bit
        bit >>= 1
    return n
```

#### Proof of correctness (encoding)

Adjacent Gray codes `G(n)` and `G(n+1)` differ in exactly one bit. Consider `n` and `n+1`:
- Let bit position `p` be the lowest bit that changes from `n` to `n+1` (i.e., `n` has a 0→1 transition at `p`).
- For position `p`: `G(n)[p] = n[p] ^ n[p+1]` flips, while `G(n+1)[p] = (n+1)[p] ^ (n+1)[p+1]` also changes.
- For all positions above `p`: `n` and `n+1` are identical, so the XOR is the same.
- For all positions below `p`: they are all 0 in `n+1` and were all 1 in `n` — the XOR with the bit above cancels, leaving them unchanged.

#### Proof of correctness (decoding)

The decode operation computes the prefix-XOR inverse. Since `gray_encode` applies `n ^ (n >> 1)`, decoding iteratively XORs shifted versions: `n = g ^ (g >> 1) ^ (g >> 2) ^ ...`. This converges because eventually the shifted values become 0. We can verify: `gray_decode(gray_encode(n)) = n` for all `n`.

#### Complexity

- Encoding: O(1).
- Decoding: O(log n) (iterates over bit width).
- Sequence generation: O(2^num_bits).

#### Applications

- Karnaugh maps (minimizing logic).
- Rotary encoders (hardware position sensing).
- Minimizing bit transitions in hardware buses.
- Towers of Hanoi solution ordering.

---

### `gray_code_sequence(num_bits)`

Generate the complete Gray code sequence for `num_bits` bits (2^num_bits entries starting from 0).

```
gray_code_sequence(3) = [0, 1, 3, 2, 6, 7, 5, 4]
```

## Implementation Notes

- `next_same_popcount` and `enumerate_submasks` are `constexpr`-capable.
- `collect_submasks` returns results in a `std::vector` for convenience.
- `gray_code_sequence` is a non-template function returning `std::vector<uint64_t>`.

## References

- Gosper's hack: HAKMEM (MIT AI Memo 239, Item 175).
- Submask enumeration: CP-Algorithms — "Enumerating all submasks of a bitmask".
- Gray code: Frank Gray, "Pulse code communication" (U.S. Patent 2,632,058, 1953).
- Donald Knuth, *The Art of Computer Programming*, Vol. 4A — Combinatorial Algorithms.

