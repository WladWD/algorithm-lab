# Trie (Prefix Tree)

## Overview

A **Trie** (also called a **prefix tree** or **digital tree**) is a tree-shaped data structure
used to store and retrieve strings from a dictionary. Each edge corresponds to a character and
each path from the root represents a prefix of one or more stored words.

Tries excel at **prefix-based queries** — checking whether any stored word starts with a given
prefix, counting matches, and enumerating completions — all in time proportional to the query
length, independent of the dictionary size.

---

## Problem Definition

Maintain a dynamic set S of strings over an alphabet Σ (|Σ| = k) supporting:

| Operation | Description |
|-----------|-------------|
| `insert(w)` | Add word w to S |
| `search(w)` | Does w ∈ S? |
| `starts_with(p)` | Does any word in S start with prefix p? |
| `erase(w)` | Remove w from S |
| `count_with_prefix(p)` | How many words in S start with p? |
| `words_with_prefix(p)` | Enumerate all words starting with p |
| `longest_prefix_of(w)` | Longest prefix of w that is in S |

---

## Data Structure Description

### Structure

- Each node stores an array of k child pointers (one per alphabet character).
- A boolean `is_end` flag marks nodes where a stored word terminates.
- An integer `prefix_count` tracks how many words pass through each node, enabling
  efficient prefix counting and deletion.

### Key Design Decisions

| Choice | Rationale |
|--------|-----------|
| Array-based children (`int[26]`) | O(1) child lookup; cache-friendly for small alphabets |
| Pool allocation (`std::vector<Node>`) | Nodes stored contiguously; avoids per-node heap allocation |
| `prefix_count` per node | Enables O(L) prefix counting without subtree traversal |
| Lowercase a–z alphabet | Sufficient for most competitive programming / interview settings |

---

## When to Use

### ✅ Best suited for

- **Autocomplete / search suggestions** — enumerate completions in real time
- **Spell checkers** — fast lookup and prefix matching
- **IP routing (longest prefix match)** — `longest_prefix_of` in O(L)
- **Word games** — Boggle, Scrabble move validation
- **Competitive programming** — XOR tries, string matching with prefix constraints

### ❌ Consider alternatives when

- Memory is very constrained → **Hash set** uses less space per word
- Only exact lookups needed → **Hash map** is simpler and often faster
- Need sorted iteration → **Balanced BST** (e.g., `std::set<std::string>`)
- Alphabet is very large (e.g., Unicode) → **Ternary search tree** or **hash-map children**

---

## Complexity

Let L = length of the key/word, k = alphabet size, N = total characters stored.

| Operation | Time | Space |
|-----------|------|-------|
| `insert` | O(L) | O(L) new nodes worst case |
| `search` | O(L) | O(1) |
| `starts_with` | O(L) | O(1) |
| `erase` | O(L) | O(1) |
| `count_with_prefix` | O(L) | O(1) |
| `words_with_prefix` | O(L + output) | O(output) |
| `longest_prefix_of` | O(L) | O(1) |
| **Total space** | — | **O(N · k)** |

Where:
- L = length of the query string
- k = alphabet size (26 for lowercase English)
- N = total number of nodes (≤ sum of all word lengths)

---

## Interface

Header: `include/data_structures/trie/trie.h`

```cpp
#include <data_structures/trie/trie.h>

using ds::trie::Trie;

Trie t;

// Insert words
t.insert("apple");
t.insert("app");
t.insert("application");
t.insert("banana");

// Exact search
t.search("apple");          // true
t.search("appl");           // false

// Prefix queries
t.starts_with("app");       // true
t.count_with_prefix("app"); // 3

// Enumerate completions
auto words = t.words_with_prefix("app");
// → {"app", "apple", "application"}

// Longest stored prefix
t.longest_prefix_of("applesauce"); // "apple"

// Delete
t.erase("app");
t.search("app");            // false
t.search("apple");          // true (unaffected)
t.count_with_prefix("app"); // 2

// Utility
t.size();    // number of stored words
t.empty();   // true if no words stored
t.clear();   // remove everything
```

---

## Example

### Building the Trie

Insert: `"the"`, `"them"`, `"their"`, `"there"`, `"to"`

```
        (root)
       /      \
      t        ...
      |
      h ──── o ●
      |
      e ●
     /|\
    m  i  r
    ●  r  e
       ●  ●
```

`●` = `is_end = true`

### Query: `starts_with("the")`

Traverse root → t → h → e. Node exists and has `prefix_count > 0` → **true**.

### Query: `count_with_prefix("the")`

Traverse to the `e` node. `prefix_count = 4` (the, them, their, there) → **4**.

### Query: `longest_prefix_of("therapy")`

Traverse: t → h → e (is_end ✓, record length 3) → r → no 'a' child → stop.
Return `"the"`.

---

## Applications

1. **Autocomplete systems** — real-time prefix search and ranked completions
2. **Spell checking** — fast dictionary lookup with edit-distance extensions
3. **IP routing tables** — longest prefix match for CIDR routing
4. **Genomics** — DNA/RNA sequence prefix matching (k=4)
5. **Compiler symbol tables** — fast identifier lookup
6. **T9 / predictive text** — phone keyboard word prediction
7. **XOR maximum** — binary trie for finding maximum XOR pair (competitive programming)

---

## Related Data Structures

| Structure | Comparison |
|-----------|------------|
| **Hash Set** | O(L) lookup but no prefix operations; lower memory |
| **Balanced BST** | O(L · log N) lookup; supports ordered iteration |
| **Ternary Search Tree** | Lower memory for sparse alphabets; similar prefix operations |
| **Patricia Trie** (Radix Tree) | Compressed edges; fewer nodes for long shared prefixes |
| **Suffix Tree / Array** | For suffix-based queries (substring search, LCP) |
| **Aho–Corasick Automaton** | Multi-pattern search; built on top of a trie |

---

## References

1. Fredkin, E. (1960). "Trie Memory". *Communications of the ACM*, 3(9), 490–499.
2. Knuth, D. E. — *The Art of Computer Programming*, Vol. 3, Section 6.3.
3. Cormen, T. H.; Leiserson, C. E.; Rivest, R. L.; Stein, C. — *Introduction to
   Algorithms (CLRS)*, 4th Ed., Problem 12-2.
4. Sedgewick, R.; Wayne, K. — *Algorithms*, 4th Ed., Chapter 5.2.

---

## See Also

- [proof.md](./proof.md) — Correctness invariants and complexity analysis

