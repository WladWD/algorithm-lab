# Suffix Tree (Ukkonen's Algorithm)

A **suffix tree** is a compressed trie of all suffixes of a string. Ukkonen's algorithm builds it **online in O(n)** by maintaining an *active point* that amortises extension work across all phases. A unique sentinel character (`\1`) is appended internally to make every suffix end at a distinct leaf.

## When to use

| Scenario | Recommendation |
|---|---|
| Pattern matching (all hits) | O(m + z) — follow edges then collect leaves |
| Longest repeated substring | Deepest internal node |
| Longest common substring | Generalised suffix tree over two strings |
| LCA-based queries | Suffix tree provides the natural structure |

For pure pattern matching without LCA, the suffix array + LCP has lower constant factors. Use the suffix tree when you need the explicit tree topology (parent/child relationships).

## API

```cpp
#include <algorithms/strings/suffix_tree/suffix_tree.h>
namespace st = algorithms::strings::suffix_tree;

st::SuffixTree t("abracadabra");

t.contains("abra");                 // true
t.search("abra");                   // {0, 7}  (sorted)
t.count_occurrences("abra");        // 2
t.num_nodes();                      // ≤ 2n + 1
```

**Note:** The sentinel `\1` (ASCII 1) is appended internally. Patterns containing `\1` will not match correctly.

## Complexity

| Operation           | Time     | Space  |
|---------------------|----------|--------|
| Construction        | O(n)     | O(n)   |
| `contains`          | O(m)     | O(1)   |
| `search`            | O(m + z) | O(z)   |
| `count_occurrences` | O(m + z) | O(z)   |

n = text length, m = pattern length, z = number of occurrences.

## Proof / correctness

See [proof.md](proof.md).
