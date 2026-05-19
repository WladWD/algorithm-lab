# KMP — Proof of Correctness

## Prefix function definition

For a string `s[0..n-1]`, the **prefix function** `π[i]` is the length of the longest **proper** prefix of `s[0..i]` that is also a suffix of `s[0..i]`.

Formally: `π[i] = max { k : 0 ≤ k < i+1, s[0..k-1] = s[i-k+1..i] }`, with `π[0] = 0`.

---

## Prefix function: O(m) construction

### Algorithm

```
π[0] = 0
for i = 1 to n-1:
    j = π[i-1]
    while j > 0 and s[i] ≠ s[j]:
        j = π[j-1]
    if s[i] == s[j]:
        j++
    π[i] = j
```

### Correctness

**Loop invariant:** At the start of iteration `i`, `j = π[i-1]`, meaning `s[0..j-1] = s[i-j..i-1]`.

**Case 1: `s[i] == s[j]`.** The prefix `s[0..j]` equals the suffix `s[i-j..i]`, so `π[i] = j + 1`. ✓

**Case 2: `s[i] ≠ s[j]`.** We need the next shorter candidate. Any border of `s[0..i-1]` of length `< j` must itself be a border of the border `s[0..j-1]`, and `π[j-1]` gives the longest such. We iterate: `j = π[j-1]`, until either a match is found or `j = 0`. ✓

### Time complexity: O(n)

`j` increases by at most 1 per outer iteration (total `+n`). Each inner while-loop iteration strictly decreases `j`. Therefore, `j` decreases at most `n` times total. All iterations combined: O(n). ∎

---

## KMP Search: O(n + m)

### Algorithm

Build `π` for the pattern `p[0..m-1]`. Then slide over text `t[0..n-1]` maintaining a match-length counter `j`:

```
j = 0
for i = 0 to n-1:
    while j > 0 and t[i] ≠ p[j]:
        j = π[j-1]
    if t[i] == p[j]:
        j++
    if j == m:
        report match at i+1-m
        j = π[m-1]
```

### Correctness

**Invariant:** `j` is the length of the longest prefix of `p` that is also a suffix of `t[0..i-1]`. This is maintained by the same argument as the prefix function construction, but applied across two strings.

**Soundness:** A match is reported iff `j == m`, meaning `p[0..m-1] = t[i+1-m..i]`. ✓

**Completeness:** Every occurrence is reported because no potential match is skipped — after a mismatch, `j = π[j-1]` is the longest shorter candidate, by the same argument as prefix function correctness. ✓

### Time complexity: O(n + m)

Preprocessing: O(m). In the search loop, `j` increases by ≤ 1 per text character (total `+n`). Each while iteration decreases `j` (total `−` bounded by total `+n`). Total iterations: O(n). Combined: **O(n + m)**. ∎

---

## Borders

The borders of `s` are exactly all values reachable by following the `π`-chain from `π[n-1]`:

```
b₁ = π[n-1], b₂ = π[b₁-1], b₃ = π[b₂-1], ...
```

Each `bₖ` gives a border of length `bₖ`. This correctly enumerates all proper prefixes of `s` that are also suffixes. ∎

---

## Period

The **shortest period** `p` of a string satisfies: `n - π[n-1]` is a period, and it is the shortest period iff `n % p == 0`.

**Proof:** After KMP preprocessing, `π[n-1]` is the length of the longest border. The suffix of length `π[n-1]` equals the prefix of the same length. This "overlap" means the string can be extended from a repeated block of size `p = n - π[n-1]`. The repetition is exact (i.e., `n` is a multiple of `p`) iff `p` truly tiles the string. ∎

---

## References

- D. E. Knuth, J. H. Morris, V. R. Pratt, "Fast pattern matching in strings," SIAM J. Comput., 1977.
- CP-Algorithms: "KMP — Prefix Function" — standard competitive programming reference.
- T. H. Cormen et al., *Introduction to Algorithms (CLRS)*, Chapter 32.4.

