# Ordered Map (AVL Tree)

An **ordered map** backed by an AVL tree stores key-value pairs in sorted key order. The AVL balance invariant — `|height(right) − height(left)| ≤ 1` at every node — guarantees O(log n) height, giving worst-case O(log n) for all structural operations.

## When to use

| Scenario | Recommendation |
|---|---|
| Sorted iteration or range queries | Keys are always in order; natural in-order traversal |
| Worst-case O(log n) guarantees | AVL is strictly balanced; no degenerate cases |
| `min_key` / `max_key` queries | O(log n) — follow left/right spine |
| Small-to-medium datasets | Lower constant factor than B-trees; simpler than red-black |

Prefer an unordered hash table when order is irrelevant and average O(1) lookups matter more than ordering.

## API

```cpp
#include <data_structures/associative/ordered_map/ordered_map.h>
namespace om = ds::ordered_map;

om::OrderedMap<std::string, int> m;

m.insert("banana", 2);
m.insert("apple",  1);
m.insert("cherry", 3);

int* v = m.find("banana");      // &2
m.contains("apple");            // true
m.erase("banana");              // true
m["date"] = 4;                  // insert-or-assign via operator[]

m.min_key();    // ptr to "apple"
m.max_key();    // ptr to "date"
m.size();       // 3
```

A custom comparator can be passed as the third template argument (same convention as `std::map`).

Copy constructor and copy assignment perform deep O(n) copies. Move is O(1).

## Complexity

| Operation    | Time     |
|--------------|----------|
| `insert`     | O(log n) |
| `erase`      | O(log n) |
| `find`       | O(log n) |
| `contains`   | O(log n) |
| `operator[]` | O(log n) |
| `min_key`    | O(log n) |
| `max_key`    | O(log n) |
| `size`       | O(1)     |
| `clear`      | O(n)     |
| Copy         | O(n)     |
| Move         | O(1)     |

Tree height ≤ 1.44 log₂(n + 2) − 0.328 (AVL worst-case height bound).

## Proof / correctness

See [proof.md](proof.md).
