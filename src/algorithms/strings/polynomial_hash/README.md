# Polynomial String Hashing

This module implements **Polynomial (Rabin-style) String Hashing** — a technique for mapping substrings to integers in O(1) after O(n) preprocessing, enabling fast pattern matching, substring comparison, and LCP queries.

Implementation highlights:
- **`PolyHash`** — single-modulus prefix hash with O(1) substring queries
- **`DoubleHash`** — two independent hash pairs for near-zero collision probability (~1/10³⁶)
- **`find_all`** — O(n+m) expected pattern search using double hashing
- **`lcp`** — O(log n) longest common prefix via binary search over hashes
- **`substrings_equal`** — O(1) substring equality check

---

## When to use

Polynomial hashing is ideal for:
- **Pattern matching** — find all occurrences of a pattern in O(n+m) expected
- **Substring equality / LCP** — O(1) comparison after O(n) build
- **Duplicate substring detection** — hash all k-length windows, group by hash
- **Rolling window hashing** — compare windows of text efficiently

---

## API

Header: `include/algorithms/strings/polynomial_hash/polynomial_hash.h`

```cpp
#include <algorithms/strings/polynomial_hash/polynomial_hash.h>

namespace ph = algorithms::strings::polynomial_hash;

// Build prefix hash table
ph::DoubleHash dh("abracadabra");

// O(1) substring hash
auto h = dh.query(0, 3);   // hash of "abra"

// O(1) substring equality
bool eq = ph::substrings_equal(dh, 0, 3, dh, 7, 10);  // "abra" == "abra"

// O(log n) LCP
std::size_t l = ph::lcp(dh, 0, 7);  // LCP of s[0..] and s[7..] = 4 ("abra")

// Pattern search
auto pos = ph::find_all("abracadabra", "abra");  // {0, 7}
```

---

## Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Build | O(n) | O(n) |
| Substring hash query | O(1) | — |
| Substring equality | O(1) expected | — |
| LCP | O(log n) expected | — |
| Pattern search (`find_all`) | O(n + m) expected | O(n + m) |

---

## Collision probability

Single hash: ~1/M (1 in 10⁹ per query).
Double hash: ~1/(M₁·M₂) ≈ 1/10¹⁸ per query.

For competitive programming or production use, double hashing is strongly recommended.

---

## Proof / correctness

See [`proof.md`](./proof.md).

