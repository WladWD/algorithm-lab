# Suffix Array + LCP Array

A **suffix array** SA is a sorted array of the starting indices of all suffixes of a string. Together with the **LCP array** (built by Kasai's algorithm), it enables fast pattern matching and many other string operations with no auxiliary data structures beyond O(n) integers.

## When to use

| Scenario | Recommendation |
|---|---|
| All occurrences of a pattern | O(m log n) with binary search on SA — faster than brute force |
| Longest repeated substring | Build LCP, find the maximum value |
| Number of distinct substrings | n*(n+1)/2 − sum(LCP) |
| Multiple pattern queries on a fixed text | Build SA once, binary-search for each |

Prefer the suffix automaton if you also need occurrence counts or substring enumeration, and prefer the suffix tree if you need LCA-based queries.

## API

```cpp
#include <algorithms/strings/suffix_array/suffix_array.h>
namespace sa = algorithms::strings::suffix_array;

auto arr = sa::build("banana");
// arr = {5, 3, 1, 0, 4, 2}  →  "a","ana","anana","banana","na","nana"

auto lcp = sa::build_lcp("banana", arr);
// lcp = {0, 1, 3, 0, 0, 2}

auto pos = sa::search("banana", "ana", arr);
// pos = {1, 3}  (sorted)

int n = sa::count_occurrences("banana", "na", arr);
// n = 2
```

## Complexity

| Function            | Time          | Space  |
|---------------------|---------------|--------|
| `build`             | O(n log² n)   | O(n)   |
| `build_lcp`         | O(n)          | O(n)   |
| `search`            | O(m log n)    | O(z)   |
| `count_occurrences` | O(m log n)    | O(z)   |

n = text length, m = pattern length, z = number of matches.

## Proof / correctness

See [proof.md](proof.md).
