# KMP — Knuth-Morris-Pratt Pattern Matching

This module implements the **KMP algorithm** — a linear-time exact string matching algorithm that preprocesses the pattern into a *prefix function* (failure function) and uses it to skip redundant comparisons during search.

Implementation highlights:
- **`prefix_function`** — core O(m) preprocessing: failure function / KMP table
- **`search`** — O(n + m) text scan returning all match positions, no sentinel character allocation
- **`count_occurrences`** — wraps search for a quick count
- **`borders`** — enumerate all borders of a string (proper prefixes that are also suffixes)
- **`period`** — shortest period of a string via the prefix function

---

## When to use

KMP is the standard choice for:
- **Single-pattern exact matching** when you need guaranteed O(n + m) time with O(m) space
- **Finding all overlapping occurrences** of a pattern in a text
- **Prefix function analysis** — detecting string periods, borders, and repetitions

Prefer Aho-Corasick for multi-pattern search, and Z-function for problems phrased in terms of Z-values.

---

## API

Header: `include/algorithms/strings/kmp/kmp.h`

```cpp
#include <algorithms/strings/kmp/kmp.h>

namespace kmp = algorithms::strings::kmp;

// Prefix function (failure function)
auto pi = kmp::prefix_function("ababab");  // {0,0,1,2,3,4}

// Pattern search
auto pos = kmp::search("abracadabra", "abra");  // {0, 7}

// Count
std::size_t cnt = kmp::count_occurrences("aaaa", "aa");  // 3

// Borders of "ababab"
auto b = kmp::borders("ababab");  // {2, 4} ("ab", "abab")

// Period
std::size_t p = kmp::period("ababab");  // 2
```

---

## Complexity

| Operation | Time | Space |
|-----------|------|-------|
| `prefix_function(s)` | O(m) | O(m) |
| `search(text, pattern)` | O(n + m) | O(m) |
| `count_occurrences` | O(n + m) | O(m) |
| `borders(s)` | O(m) | O(m) |
| `period(s)` | O(m) | O(m) |

---

## Proof / correctness

See [`proof.md`](./proof.md).

