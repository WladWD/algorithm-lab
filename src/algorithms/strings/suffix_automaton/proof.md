# Suffix Automaton — Proof of Correctness

## Key definitions

**endpos(w):** For substring `w` of string `s`, `endpos(w) = { i : s[i−|w|+1..i] = w }` — the set of ending positions of all occurrences of `w`.

Two substrings are **endpos-equivalent** if their endpos sets are identical. The equivalence classes are the states of the SAM.

**Suffix link:** Each state `v ≠ initial` has a suffix link `link(v)` pointing to the state whose endpos set is the smallest proper superset of `endpos(v)`. The suffix links form a tree rooted at the initial state.

---

## SAM size bounds

**Theorem:** The SAM of a string of length n has at most 2n − 1 states and at most 3n − 4 transitions (for n ≥ 3).

**Proof sketch:**
- There are exactly n distinct endpos sets (one per position), plus the initial state.
- Each internal split in the suffix-link tree corresponds to adding one clone state.
- The number of clone states is bounded by n − 1 (one per character after the first).
- Total states ≤ n + (n−1) = 2n − 1. ∎

---

## Construction: O(n) online algorithm

### Algorithm (one character `c` at a time)

```
Create cur with len = last.len + 1,  cnt = 1
p = last
while p ≠ null and p has no transition on c:
    p.next[c] = cur
    p = link(p)

if p == null:
    link(cur) = initial
else:
    q = p.next[c]
    if p.len + 1 == q.len:
        link(cur) = q
    else:
        clone = copy of q  with len = p.len + 1,  cnt = 0
        while p ≠ null and p.next[c] == q:
            p.next[c] = clone
            p = link(p)
        link(q) = clone
        link(cur) = clone

last = cur
```

### Correctness

**Invariant:** After processing `s[0..i]`, the automaton accepts exactly the set of all suffixes of `s[0..i]` and all their substrings.

**Case 1 (p == null):** No suffix of the current prefix ends with `c`, so `cur` is linked directly to the initial state. All suffixes of the new string ending with `c` are routed through `cur`. ✓

**Case 2 (p.len + 1 == q.len):** The transition `p → q` on `c` already represents a state whose longest string has length `p.len + 1`, exactly matching the suffix ending here. `link(cur) = q` is correct. ✓

**Case 3 (clone):** State `q` conflates strings of lengths `p.len + 1..q.len`. We split: `clone` handles lengths `p.len + 1..p.len + 1`, and `q` keeps lengths `p.len + 2..q.len`. Redirecting all `p`-chain transitions to `clone` preserves acceptance. ✓

### Time complexity: O(n)

The `while` loop decreases `len` along the suffix-link chain. Since `len` values strictly decrease towards the root and are bounded by n, the total number of iterations across all `extend` calls is O(n). Each call creates at most 2 nodes (cur + optional clone). Total: **O(n)**. ∎

---

## Occurrence counting: O(n) post-processing

After building, propagate `cnt` fields along the suffix-link tree in topological order (decreasing `len`):

```
for each state v in order of decreasing len:
    cnt[link(v)] += cnt[v]
```

**Correctness:** `cnt[v]` starts at 1 for states created as `cur` (corresponding to new endpos entries) and 0 for clones. After propagation, `cnt[v]` equals `|endpos(v)|` — the number of occurrences of any substring in the equivalence class of `v`. ✓

---

## Distinct substrings: O(n)

Each state `v` with suffix link `u` contributes `len(v) − len(u)` distinct substrings (the strings of lengths `len(u)+1..len(v)` in the equivalence class). Summing over all states gives the total count of distinct non-empty substrings. ∎

---

## References

- A. Blumer et al., "The smallest automaton recognizing the subwords of a text," Theoretical Computer Science, 1985.
- CP-Algorithms: "Suffix Automaton" — practical construction treatment.
- M. Crochemore and W. Rytter, *Jewels of Stringology*, 2002.
