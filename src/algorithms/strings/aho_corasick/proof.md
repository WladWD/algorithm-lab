# Aho-Corasick — Proof of Correctness

## Structure

The Aho-Corasick automaton has three components:

1. **Goto function** `g(v, c)` — trie transitions. Undefined transitions at root become self-loops.
2. **Failure function** `f(v)` — the longest proper suffix of the string represented by `v` that is also a prefix of some pattern (analogous to KMP's π).
3. **Output function** `out(v)` — set of patterns that end at node `v` (including those reachable via dictionary links).

---

## Failure link construction (correctness)

**Claim:** After BFS, `f(v)` correctly equals the longest proper suffix of `str(v)` that exists in the trie.

**Proof (by BFS level):**

Base case: All depth-1 nodes have `f = root`. The root is the empty string, which is a proper suffix of any single character. ✓

Inductive step: Suppose all nodes at depth `< d` have correct failure links. For a node `v` at depth `d` reached by character `c` from parent `p`:
- We need `f(v)` = longest proper suffix of `str(p) + c` that is a trie prefix.
- Try `f(p)` — the longest proper suffix of `str(p)`. If `g(f(p), c)` exists, then `str(f(p)) + c` is a valid suffix of `str(v)` that's in the trie. Set `f(v) = g(f(p), c)`.
- If not, follow `f(f(p))` and repeat (like KMP's fallback). The root's self-loops ensure termination.

By induction, all failure links are correct. ∎

---

## Goto function compilation (shortcut transitions)

After failure links are built, any missing `next[v][c]` (i.e., `−1`) is replaced by `next[f(v)][c]`. This allows the automaton to transition in O(1) per character without following failure chains at search time.

**Correctness:** If node `v` has no child for `c`, the correct next state is the deepest ancestor of `v` (via failure links) that has a child `c`. The BFS order ensures `f(v)` is already compiled when `v` is processed, so `next[f(v)][c]` is already the fully-compiled transition. ∎

---

## Dictionary links

`dict(v)` points to the nearest ancestor (via failure chain) of `v` that is the end of a pattern. This allows enumerating all patterns ending at each text position in time proportional to the number of matches, not the trie depth.

**Correctness:** `dict(v) = f(v)` if `f(v)` has an output, else `dict(v) = dict(f(v))`. By induction, `dict` correctly chains through all pattern-ending ancestors. ∎

---

## Search correctness

**Claim:** `search(text)` reports a match `{i, pid}` iff `pattern[pid]` ends at position `i` in `text`.

**Proof:**

At each text position `i`, the automaton is in state `v = δ*(root, text[0..i])` — the state corresponding to the longest suffix of `text[0..i]` that is a prefix of some pattern.

We collect all patterns ending at `i` by:
1. Checking `out(v)` — the pattern ending exactly at `v`.
2. Following `dict(v)`, `dict(dict(v))`, ... — patterns ending at proper suffixes of `str(v)`.

**Soundness:** Any pattern reported at position `i` is a suffix of `text[0..i]` (by failure-link semantics), hence ends at `i`. ✓

**Completeness:** Any pattern `p` ending at `i` satisfies `text[i-|p|+1..i] = p`, so `p` is a suffix of `text[0..i]`. The state `v` represents the longest such suffix; `p` is either `str(v)` or reachable via the failure/dictionary chain. ✓

∎

---

## Complexity

### Build: O(M · Σ)

- Trie insertion: O(M) total.
- BFS failure links: O(|trie| · Σ) — each node processes all Σ characters once.
- For Σ = 256, this is O(256M) ≈ O(M).

### Search: O(n + z)

- Each character advances the automaton in O(1) (compiled goto).
- Each match is reported once, taking O(1) via dictionary links.
- Total: O(n) transitions + O(z) match reporting = **O(n + z)**. ∎

---

## References

- A. V. Aho, M. J. Corasick, "Efficient string matching: an aid to bibliographic search," CACM, 1975.
- CP-Algorithms: "Aho-Corasick Algorithm."
- T. H. Cormen et al., *Introduction to Algorithms (CLRS)*, Chapter 32.

