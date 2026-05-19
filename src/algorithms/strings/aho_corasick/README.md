# Aho-Corasick Multi-Pattern Matching Automaton

This module implements the **Aho-Corasick automaton** — a data structure that simultaneously matches all patterns in a dictionary against a text in **O(n + z)** time (where z = number of matches), after O(M) preprocessing.

Implementation highlights:
- **Full 256-byte alphabet** — handles arbitrary byte strings
- **Failure links** (BFS-computed, like KMP's prefix function) for O(1) transitions
- **Dictionary links** — fast traversal of all patterns ending at each position
- **Goto function** compiled at build time (no backtracking at search time)

---

## When to use

Aho-Corasick is the right choice when:
- You have **multiple patterns** to search simultaneously
- The text is large and patterns are fixed (build once, search many times)
- You need to find **all occurrences** of all patterns in one O(n) pass

For a single pattern, KMP or Z-function is simpler. For a single scan, Rabin-Karp is competitive but probabilistic.

---

## API

Header: `include/algorithms/strings/aho_corasick/aho_corasick.h`

```cpp
#include <algorithms/strings/aho_corasick/aho_corasick.h>

namespace ac = algorithms::strings::aho_corasick;

ac::AhoCorasick automaton;
automaton.add_pattern("he");
automaton.add_pattern("she");
automaton.add_pattern("his");
automaton.add_pattern("hers");
automaton.build();

auto matches = automaton.search("ushers");
// matches: [{end=1, pid=2("his")}, {end=2, pid=1("she")},
//           {end=3, pid=0("he")}, {end=5, pid=3("hers")}]
```

Each `Match` holds:
- `end_pos` — 0-indexed end position (inclusive) in the text
- `pattern_id` — index of the matched pattern in the order added

---

## Complexity

| Operation | Time | Space |
|-----------|------|-------|
| `add_pattern(p)` | O(\|p\|) | O(\|p\| · Σ) |
| `build()` | O(M · Σ) | O(M · Σ) |
| `search(text)` | O(n + z) | O(z) |

Where M = sum of pattern lengths, Σ = 256 (alphabet size), z = total matches.

---

## Proof / correctness

See [`proof.md`](./proof.md).

