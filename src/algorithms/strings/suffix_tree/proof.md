# Suffix Tree (Ukkonen's Algorithm) — Proof of Correctness

## Definitions

For string `s` of length `n`, a **suffix tree** is a rooted, compressed trie whose edge labels are non-empty substrings of `s`, such that:
1. Every suffix of `s` corresponds to a root-to-leaf path.
2. No two edges leaving the same node share a first character.
3. Every internal node (except possibly the root) has at least 2 children.

A **sentinel character** `'\1'` (not in the original alphabet) is appended to force all suffixes to end at distinct leaves, making the tree *explicit* rather than implicit.

---

## Active-point invariant

Ukkonen's algorithm maintains state `(active_node, active_edge, active_len)` and a counter `remaining` between phases.

**Invariant after phase i (processing `s[i]`):** `remaining` is the number of suffixes of `s[0..i]` that have been extended implicitly (i.e., they are already present in the tree as a prefix of an edge label) but do not yet have their own leaf.

---

## Extension rules

When processing character `s[i]`, the algorithm resolves `remaining` pending suffixes one at a time. For each pending suffix, exactly one of three rules applies:

**Rule 1 — Open leaf:** The active point sits on an open leaf edge (end = ∞). Extending the leaf costs O(1): the open-ended boundary automatically covers the new character. `remaining` is unchanged conceptually (the leaf extension is free).

**Rule 2a — New edge from existing node:** The active edge does not exist from the active node. Create a new leaf. Decrement `remaining`, advance the active point along the suffix link.

**Rule 2b — Split:** The active character in the middle of an edge does not match `s[i]`. Split the edge at the active point, creating a new internal node. Attach a new leaf. Decrement `remaining`, follow suffix link.

**Rule 3 — Implicit extension:** The character `s[i]` already continues the active edge. Increment `active_len` and stop processing further suffixes for this phase. `remaining` is not decremented.

**Correctness of early exit (Rule 3):** If `s[i]` extends the current path, every shorter suffix also extends its corresponding path (they are all suffixes of the current longest path). No new leaves are needed for this phase. Stopping is safe. ✓

---

## Suffix links

When a new internal node `u` is created by splitting, its **suffix link** points to the internal node `v` reached by the next Rule-2 split in the same phase (or to the root if no split follows). Formally, if `u` represents the string `aα` (for character `a` and string `α`), then `link(u)` represents `α`.

**Correctness of suffix links:** After splitting node `u` on string `aα`, the algorithm processes the next shorter suffix, which begins with `α`. The active point descends to the node representing `α`. If that node was also just split (or created), it is exactly `v = link(u)`. This is proven by structural induction on phases; each newly created internal node inherits the invariant from the previous step. ✓

---

## Node count bound: ≤ 2n + 1

- There are exactly `n` suffix leaves (one per non-empty suffix of `s·'\1'`, including the full string).
- Each internal node (other than the root) has ≥ 2 children, so there are at most `n − 1` internal nodes.
- Total nodes ≤ `n + (n − 1) + 1 (root) = 2n`. With the sentinel the bound is `2n + 1`. ∎

---

## Time complexity: O(n)

The algorithm processes `n + 1` characters (including sentinel). For each character `i`:

- **Open-leaf extensions** (Rule 1) are free — no explicit work.
- **Rule 2 extensions** create at most one new leaf and decrement `remaining` by 1. Since `remaining` is incremented by at most 1 per phase, the total number of Rule 2 extensions across all phases is at most `n + 1`.
- **Rule 3 extensions** terminate the phase immediately.
- **Active-point walk-down** after a suffix-link traversal is amortised by the fact that `active_len` can only increase by 1 per phase and decrease by at least 1 per suffix-link traversal. Total walk-down steps ≤ 2n.

Total work = O(n). ∎

---

## Leaf annotation: O(n) DFS

After construction, a single DFS (`mark`) assigns each leaf the starting index of its corresponding suffix. The DFS visits each node exactly once; total cost O(n). ✓

---

## Pattern search: O(m)

`locate(pattern)` follows edges character by character. At each step it either advances within an edge or crosses to a child. The number of character comparisons is ≤ m (the pattern length). Returns the subtree root where the pattern ends, or -1. Cost: **O(m)**. ✓

---

## Occurrence collection: O(z)

`collect(node)` DFS-gathers all leaf `idx` values under a node. Each leaf is visited at most once. Cost: **O(z)** where z = number of leaves = number of occurrences. ✓

---

## References

- E. Ukkonen, "On-line construction of suffix trees," Algorithmica, 1995.
- D. Gusfield, *Algorithms on Strings, Trees, and Sequences*, Cambridge, 1997.
- CP-Algorithms: "Suffix Tree — Ukkonen's Algorithm" — step-by-step construction walkthrough.
