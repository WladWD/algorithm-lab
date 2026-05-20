# Hash Table — Separate Chaining

A **hash table with separate chaining** maps each key to a bucket index; all keys that hash to the same bucket are stored in that bucket's list. Lookup, insert, and erase scan the bucket linearly — O(chain length) worst case, O(1) expected when the load factor is bounded.

## When to use

| Scenario | Recommendation |
|---|---|
| High deletion rate | No tombstones; erase is clean O(1) expected |
| Large or non-trivial keys | Only stores live entries; no empty-slot overhead |
| Unknown or skewed key distributions | Chains degrade gracefully; no clustering penalty |
| Load factor > 1 permitted | Chaining works fine above α = 1; open addressing does not |

Prefer open addressing when cache locality matters and the load factor stays low.

## API

```cpp
#include <data_structures/associative/hash_table_chaining/hash_table_chaining.h>
namespace htc = ds::hash_table_chaining;

htc::HashTableChaining<std::string, int> m;

m.insert("alice", 1);
m.insert("bob",   2);

int* v = m.find("alice");    // &1
m.contains("bob");           // true
m.erase("bob");              // true
m["charlie"] = 3;            // insert-or-assign via operator[]

m.size();          // 2
m.load_factor();   // size / num_buckets
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

Worst-case O(n) requires all keys to hash to the same bucket.

**Rehash threshold:** when `size + 1 > 1.0 × num_buckets` (load factor reaches 1.0), the bucket array doubles and all entries are re-hashed.

**Space:** O(capacity + size) — one `vector` header per bucket plus entries themselves.

## Proof / correctness

See [proof.md](proof.md).
