# Trie — Correctness and Complexity Analysis

This document covers the correctness invariants and complexity analysis for the
Trie implementation in `src/trie.cpp`.

---

## 1. Structure Definition

A **Trie** over alphabet Σ = {a, b, ..., z} (|Σ| = k = 26) is a rooted tree where:

- Each node has at most k children, one per character.
- Each edge is labelled with a character c ∈ Σ.
- The **key** associated with a node v is the concatenation of edge labels on the
  path from the root to v.

We augment each node with:

- `is_end` ∈ {true, false} — whether a stored word ends at this node.
- `prefix_count` ∈ ℤ≥0 — the number of stored words whose key passes through
  (or terminates at) this node.

---

## 2. Representation Invariants

The following invariants hold after every public operation:

### Invariant 1: Tree Structure

The nodes form a rooted tree (node 0 is the root). Every node is reachable from
the root by following child pointers. There are no cycles.

**Proof**: Nodes are only created in `new_node()`, which appends to a vector and
returns a fresh index. Child pointers are set exactly once per (parent, character)
pair and never form back-edges, since a child's index is always greater than its
parent's. ∎

### Invariant 2: Key Uniqueness

For any string w, there is at most one node whose root-to-node path spells w.

**Proof**: At each node, child pointers are indexed by character. Given a fixed
sequence of characters, the traversal path is deterministic. ∎

### Invariant 3: `is_end` Correctness

`nodes_[v].is_end == true` if and only if the string spelled by the root-to-v path
is a member of the stored set S.

**Proof**:
- `insert(w)`: After traversing/creating the path for w, we set `is_end = true`
  at the terminal node. We only set it if it was previously false (avoiding
  double-counting).
- `erase(w)`: After verifying `search(w) == true`, we traverse the path and set
  `is_end = false` at the terminal node.
- `clear()`: Destroys all nodes and recreates a fresh root with `is_end = false`.
- No other operation modifies `is_end`. ∎

### Invariant 4: `prefix_count` Correctness

For every node v, `nodes_[v].prefix_count` equals the number of stored words w ∈ S
such that the root-to-v path is a prefix of (or equal to) w.

**Proof**:

- **Base**: After construction, the root has `prefix_count = 0` and S = ∅. ✓

- **Insert(w)** (new word): We increment `prefix_count` on the root and on every
  node along the path. Each of these nodes gains exactly one new word passing
  through them.

- **Insert(w)** (duplicate): We detect the duplicate (`is_end` already true) and
  undo all increments, restoring the prior state.

- **Erase(w)**: We verify the word exists, then decrement `prefix_count` on the
  root and on every node along the path. Each node loses exactly the one word
  being removed.

- **Clear()**: Resets everything to the initial state. ✓ ∎

### Invariant 5: `word_count_` Correctness

`word_count_` equals |S|, the number of distinct stored words.

**Proof**:
- Incremented by 1 in `insert` only when `is_end` transitions from false to true.
- Decremented by 1 in `erase` only after verifying the word exists.
- Reset to 0 in `clear`. ∎

---

## 3. Operation Correctness

### `search(w)`

Traverses the path for w. Returns `true` iff the path exists AND the terminal
node has `is_end == true`.

By Invariant 2 (unique path) and Invariant 3 (`is_end` correctness), this
correctly reports w ∈ S. ∎

### `starts_with(p)`

Traverses the path for p. Returns `true` iff the path exists AND the terminal
node has `prefix_count > 0`.

By Invariant 4, `prefix_count > 0` at a node means at least one word in S passes
through that node, i.e., at least one word starts with p. ∎

### `erase(w)`

1. Calls `search(w)`. If false, returns false (no modification).
2. Traverses the path, decrementing `prefix_count` on each node (including root).
3. Sets `is_end = false` at the terminal node.

Correctness follows from Invariants 3 and 4. After erase, no `is_end` flag falsely
claims w ∈ S, and all `prefix_count` values are correctly reduced. ∎

### `count_with_prefix(p)`

Traverses to the node for p and returns its `prefix_count`.
By Invariant 4, this is exactly |{w ∈ S : p is a prefix of w}|. ∎

### `words_with_prefix(p)`

Traverses to the node for p, then recursively collects all words reachable from
that node using DFS. Only visits children with `prefix_count > 0` (pruning
erased subtrees). Collects a word when `is_end == true`.

The result is sorted lexicographically before returning. ∎

### `longest_prefix_of(w)`

Traverses the path for w character by character. Whenever a node with
`is_end == true` is reached, records the current prefix length. Returns the
longest recorded prefix.

By Invariant 3, every recorded length corresponds to a word in S. The algorithm
checks all prefixes of w in order, so it finds the longest one. ∎

---

## 4. Complexity Analysis

### Time Complexity

Let L = |w| (length of the input string), k = |Σ| (alphabet size).

| Operation | Time |
|-----------|------|
| `insert(w)` | O(L) — one pass to traverse/create + potential rollback for duplicates |
| `search(w)` | O(L) — single traversal |
| `starts_with(p)` | O(L) — single traversal |
| `erase(w)` | O(L) — one `search` + one traversal |
| `count_with_prefix(p)` | O(L) — single traversal |
| `words_with_prefix(p)` | O(L + M) — traversal + DFS over M output characters |
| `longest_prefix_of(w)` | O(L) — single traversal |
| `clear()` | O(1) amortized (vector clear + one node allocation) |

Each character lookup within a node is O(1) because children are stored in a
fixed-size array indexed by character.

### Space Complexity

- Each node occupies O(k) space (child array of size k + constant fields).
- The total number of nodes is at most N + 1, where N = Σ_{w ∈ S} |w|
  (sum of all word lengths), plus the root.
- **Total space: O(N · k)**.

For k = 26 and N stored characters, each node is approximately 26 × 4 + 8 = 112
bytes (with `int` children, `int prefix_count`, `bool is_end`, padding).

### Comparison with Hash Set

| Aspect | Trie | Hash Set |
|--------|------|----------|
| Exact lookup | O(L) | O(L) expected |
| Prefix queries | O(L) | Not supported |
| Space per word | O(L · k) | O(L) |
| Cache behaviour | Good (contiguous pool) | Poor (hash collisions) |
| Ordered enumeration | Natural (DFS) | Requires sorting |

---

## 5. Termination

All operations terminate because:

1. **Traversal** follows a path of length at most L, which is finite.
2. **Insert** creates at most L new nodes, each in O(1) time.
3. **DFS in `collect`** visits each node at most once (tree, no cycles).
4. **No infinite loops**: all loops iterate over a bounded range (word length
   or alphabet size).

---

## 6. Example Trace

### Insert "app", "apple", "application"

```
Step 1: insert("app")
  root(1) → a(1) → p(1) → p(1●)
  word_count_ = 1

Step 2: insert("apple")
  root(2) → a(2) → p(2) → p(2●) → l(1) → e(1●)
  word_count_ = 2

Step 3: insert("application")
  root(3) → a(3) → p(3) → p(3●) → l(2) → e(1●)
                                         → i(1) → c(1) → a(1) → t(1) → i(1) → o(1) → n(1●)
  word_count_ = 3
```

(Numbers in parentheses = `prefix_count`; ● = `is_end`)

### erase("app")

```
  root(2) → a(2) → p(2) → p(2) → l(2) → e(1●)
                                        → i(1) → c(1) → a(1) → t(1) → i(1) → o(1) → n(1●)
  word_count_ = 2
```

Node for the second `p` loses `is_end` and all path nodes decrement by 1.

### count_with_prefix("app") → 2

Traverse to `p` (second one). `prefix_count = 2`. ✓

---

## 7. Lazy Deletion Note

This implementation uses **lazy deletion**: erased nodes remain in the tree with
`prefix_count` decremented to 0 (or `is_end` cleared). The nodes are not physically
removed. This simplifies the implementation and avoids pointer invalidation.

Trade-off:
- **Pro**: Simple, no dangling pointer issues, O(L) erase.
- **Con**: Memory is not reclaimed until `clear()` is called.

For applications that perform many insertions and deletions, calling `clear()` and
reinserting the remaining words periodically can reclaim unused memory.

---

## 8. References

1. Fredkin, E. (1960). "Trie Memory". *Communications of the ACM*, 3(9), 490–499.
2. Knuth, D. E. — *The Art of Computer Programming*, Vol. 3, Section 6.3.
3. Brass, P. — *Advanced Data Structures*, Cambridge University Press, 2008. Chapter 3.
4. Sedgewick, R.; Wayne, K. — *Algorithms*, 4th Ed., Pearson, 2011. Chapter 5.2.

