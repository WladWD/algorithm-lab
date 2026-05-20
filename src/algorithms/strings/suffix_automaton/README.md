# Suffix Automaton (SAM)

A **suffix automaton** is the smallest deterministic finite automaton that accepts all suffixes of a string — and only them. Each state represents an equivalence class of substrings (those with the same set of ending positions, the *endpos* set). The SAM is built online in O(n) time and space and gives O(1)—O(m) query times for a wide range of substring problems.

## When to use

| Scenario | Recommendation |
|---|---|
| Count distinct substrings | O(n) — sum over states of `len − link.len` |
| Count occurrences of a pattern | O(m) after O(n) build |
| Check if a string is a substring | O(m) |
| Longest common substring of two strings | Build SAM on one, traverse with the other |

For pattern matching alone, the suffix array is simpler. Use the SAM when you need occurrence counting or substring enumeration on a fixed text with many queries.

## API

```cpp
#include <algorithms/strings/suffix_automaton/suffix_automaton.h>
namespace sam = algorithms::strings::suffix_automaton;

sam::SuffixAutomaton sa("banana");

sa.contains("ana");                     // true
sa.count_occurrences("ana");            // 2
sa.count_distinct_substrings();         // 15
sa.num_states();                        // ≤ 2n
```

## Complexity

| Function                    | Time    | Space  |
|-----------------------------|---------|--------|
| Construction                | O(n)    | O(n)   |
| `contains`                  | O(m)    | O(1)   |
| `count_occurrences`         | O(m)    | O(1)   |
| `count_distinct_substrings` | O(n)    | O(1)   |

Number of states ≤ 2n − 1. Number of transitions ≤ 3n − 4 (for |Σ| = 1 these bounds are tight).

## Proof / correctness

See [proof.md](proof.md).
