# Hash Table — Open Addressing (Linear Probing)

A **hash table with open addressing** stores all entries directly in a flat array. Collisions are resolved by probing the next slot (linear probing with wrap-around). Deletion uses **tombstone** markers so subsequent probes over deleted slots continue correctly.

## When to use

| Scenario | Recommendation |
|---|---|
| Cache-friendly key lookups | Open addressing has better cache locality than chaining |
| Low-to-moderate load (< 75%) | Expected O(1) per operation; performance degrades near full capacity |
| High deletion rate | Tombstones accumulate — prefer chaining or periodic rehash |
| Embed into latency-critical code | No heap allocation per entry; all data in one array |

Prefer chaining if the key type is large (avoids cache-line waste in tombstone slots) or if deletion is very frequent.

## API

```cpp
#include <data_structures/associative/hash_table_oa/hash_table_oa.h>
namespace ht = ds::hash_table_oa;

ht::HashTableOA<std::string, int> m;

m.insert("alice", 1);
m.insert("bob",   2);

int* v = m.find("alice");    // &1
m.contains("bob");           // true
m.erase("bob");              // true
m["charlie"] = 3;            // insert-or-assign via operator[]

m.size();          // 2
m.load_factor();   // size / capacity
```

Custom hash and equality can be passed as template arguments (same interface as `std::unordered_map`).

## Complexity

| Operation    | Expected | Worst case |
|--------------|----------|------------|
| `insert`     | O(1) amortized | O(n) on rehash |
| `erase`      | O(1)     | O(n) |
| `find`       | O(1)     | O(n) |
| `contains`   | O(1)     | O(n) |
| `operator[]` | O(1) amortized | O(n) |
| `clear`      | O(capacity) | — |

Worst-case O(n) requires adversarial hash collisions; with a uniform hash the expected probe length is 1/(1 − α) where α = load factor ≤ 0.75.

**Rehash threshold:** when `occupied + 1 > 0.75 × capacity` (where `occupied` counts live + tombstone slots), the table doubles and all tombstones are cleared.

## Proof / correctness

See [proof.md](proof.md).
