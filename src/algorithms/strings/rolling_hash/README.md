# Rolling Hash (Rabin-Karp Sliding Window)

This module implements **Rolling Hash**, a technique for maintaining a hash over a sliding window of fixed length as it moves through a string — updating in **O(1)** per step instead of recomputing from scratch.

Implementation highlights:
- **`RollingHash`** — single-modulus, O(1) `slide()` operation
- **`DoubleRollingHash`** — two independent (base, mod) pairs for near-zero collision probability
- **`rabin_karp`** — O(n+m) expected pattern matching using the sliding window
- **`longest_duplicate_substring`** — O(n log n) expected via binary search + rolling hash

---

## When to use

Rolling hash is ideal for:
- **Sliding window problems** — compare fixed-length windows of text efficiently
- **Rabin-Karp pattern search** — find all occurrences in O(n+m) expected time
- **Anagram/duplicate detection** — group windows by hash in O(n)
- **Longest duplicate substring** — binary search + set of window hashes

Prefer **Polynomial Hash** (`PolyHash`) when you need arbitrary substring queries (not just sliding windows of one fixed size).

---

## API

Header: `include/algorithms/strings/rolling_hash/rolling_hash.h`

```cpp
#include <algorithms/strings/rolling_hash/rolling_hash.h>

namespace rh = algorithms::strings::rolling_hash;

// Sliding window
rh::DoubleRollingHash win("abcde", 3);  // hash of "abc"
win.slide('a', 'f');                    // now hashes "bcf"

// Rabin-Karp pattern search
auto pos = rh::rabin_karp("abracadabra", "abra");  // {0, 7}

// Longest duplicate substring
std::size_t len = rh::longest_duplicate_substring("banana");  // 3 ("ana")
```

---

## Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Build window | O(m) | O(1) |
| Slide one position | O(1) | — |
| `rabin_karp` | O(n + m) expected | O(1) auxiliary |
| `longest_duplicate_substring` | O(n log n) expected | O(n) |

---

## Difference from Polynomial Hash

| Feature | Rolling Hash | Polynomial Hash |
|---------|-------------|-----------------|
| Window type | Fixed-length sliding | Any substring |
| Build | O(m) per window size | O(n) once |
| Query | O(1) slide | O(1) any range |
| Memory | O(1) | O(n) |
| Best for | Sliding window, Rabin-Karp | Arbitrary substring queries, LCP |

---

## Proof / correctness

See [`proof.md`](./proof.md).

