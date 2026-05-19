# Rabin-Karp — Rolling-Hash Pattern Matching

Rabin-Karp finds all occurrences of a pattern (or a set of equal-length patterns) in a text using a **polynomial rolling hash**. A hash match is always verified by direct character comparison, so the algorithm is always correct regardless of hash collisions.

Its key advantage over KMP and Z-function is **multi-pattern search**: by storing all pattern hashes in a hash set, the text is scanned only once for any number of same-length patterns.

## When to use

| Scenario | Recommendation |
|---|---|
| Single pattern, adversarial input | Prefer KMP or Z-function (guaranteed O(n+m)) |
| Single pattern, average-case | Rabin-Karp is fine (O(n+m) expected) |
| Multiple patterns, **same length** | Rabin-Karp is the natural choice — one pass, O(n + k·m) expected |
| Multiple patterns, varying lengths | Use Aho-Corasick instead |

## API

```cpp
#include <algorithms/strings/rabin_karp/rabin_karp.h>
namespace rk = algorithms::strings::rabin_karp;

// Single pattern
auto positions = rk::search("abracadabra", "abra");          // → {0, 7}
auto count     = rk::count_occurrences("aaaa", "aa");        // → 3

// Multi-pattern (all patterns must have the same length)
auto hits = rk::search_multiple("abcabc", {"abc", "bca"});   // → {0, 1, 3}
```

## Complexity

| Function            | Time (expected) | Time (worst case) | Space   |
|---------------------|-----------------|-------------------|---------|
| `search`            | O(n + m)        | O(n·m)            | O(1)    |
| `count_occurrences` | O(n + m)        | O(n·m)            | O(1)    |
| `search_multiple`   | O(n + k·m)      | O(n·k·m)          | O(k·m)  |

Worst case occurs when every window produces a hash collision, which requires a deliberately adversarial input for the chosen hash parameters.

## Hash parameters

The implementation uses:
- **Base** B = 131
- **Modulus** P = 1 000 000 007 (10⁹ + 7)
- **Polynomial** (big-endian): h(s) = s[0]·B^(m−1) + s[1]·B^(m−2) + … + s[m−1]
- **Rolling update**: h_new = h_old · B − s[i] · B^m + s[i+m]  (mod P)

## Proof / correctness

See [proof.md](proof.md).
