# Z-Function (Z-Array)

This module implements the **Z-function algorithm** — an O(n) method to compute the Z-array, where `z[i]` is the length of the longest substring starting at `s[i]` that is also a prefix of `s`.

Implementation highlights:
- **`z_function`** — O(n) Z-array construction using the Z-box window
- **`search`** — O(n + m) pattern matching without concatenating strings (virtual string via lambda)
- **`count_occurrences`** — direct count wrapper
- **`prefix_occurrences`** — count distinct lengths of prefix substrings

---

## When to use

The Z-function is particularly elegant for:
- **Pattern matching** — equivalent power to KMP, often simpler to reason about
- **Checking if a string is a rotation** of another
- **Counting prefix matches** — how many positions in the string start a prefix of a given length
- Problems naturally stated as "how much of this suffix matches the beginning"

Choose **KMP** when the failure function semantics (border lengths) are needed. Choose **Z-function** when prefix-match lengths are more natural.

---

## API

Header: `include/algorithms/strings/z_function/z_function.h`

```cpp
#include <algorithms/strings/z_function/z_function.h>

namespace zf = algorithms::strings::z_function;

// Z-array
auto z = zf::z_function("aabxaa");  // {0, 1, 0, 0, 2, 1}

// Pattern search
auto pos = zf::search("abracadabra", "abra");  // {0, 7}

// Count
std::size_t cnt = zf::count_occurrences("aaaa", "aa");  // 3
```

---

## Complexity

| Operation | Time | Space |
|-----------|------|-------|
| `z_function(s)` | O(n) | O(n) |
| `search(text, pattern)` | O(n + m) | O(m) — virtual concat |
| `count_occurrences` | O(n + m) | O(m) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

