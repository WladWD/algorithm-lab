# Rolling Hash — Proof of Correctness

## Window hash definition

For a string `S` and window of size `w` starting at position `i`:

```
H(i) = S[i]·B^(w-1) + S[i+1]·B^(w-2) + ... + S[i+w-1]·B^0  (mod M)
```

---

## O(1) slide update

**Claim:** Given `H(i)`, we can compute `H(i+1)` in O(1):

```
H(i+1) = (H(i) - S[i]·B^(w-1)) · B + S[i+w]  (mod M)
```

**Proof:**

```
H(i) = S[i]·B^(w-1) + S[i+1]·B^(w-2) + ... + S[i+w-1]

H(i) - S[i]·B^(w-1) = S[i+1]·B^(w-2) + ... + S[i+w-1]

(H(i) - S[i]·B^(w-1)) · B = S[i+1]·B^(w-1) + ... + S[i+w-1]·B

(H(i) - S[i]·B^(w-1)) · B + S[i+w] = S[i+1]·B^(w-1) + ... + S[i+w] = H(i+1)  ✓
```

All operations are mod M. To avoid negative values, we add M before taking the modulo:
```
H(i+1) = ((H(i) + M - S[i]·B^(w-1) % M) % M · B + S[i+w]) % M
```
∎

---

## Initial hash build

Computing the initial window hash `H(0)` over `s[0..w-1]` takes O(w) using Horner's method:
```
H(0) = (...((s[0]·B + s[1])·B + s[2])·B + ... + s[w-1])
```
This is O(w) and correct by the definition of polynomial evaluation. ∎

---

## Rabin-Karp correctness

**Claim:** `rabin_karp(t, p)` returns all positions `i` where `t[i..i+m-1] == p`.

**Proof:**
- Every true occurrence has `H(t[i..i+m-1]) == H(p)`, so it is always reported.
- A false positive (hash collision) occurs when `H(t[i..i+m-1]) == H(p)` but the substrings differ. The probability per position is ≤ m/(M₁·M₂) ≈ 0 for double hashing.

In practice, for competitive programming or general use, double hashing makes false positives negligible. For guaranteed correctness, verify each hash match with a direct string comparison (O(m) extra per match). ∎

**Time:** O(m) to build pattern hash + O(n) to slide the window = O(n+m). ∎

---

## Longest duplicate substring

**Algorithm:** Binary search on length `[1, n-1]`. For a given length `L`, hash all windows of size L and check if any hash appears twice (using a hash set). If yes, a duplicate of length L exists.

**Correctness:** The binary search finds the largest L for which a duplicate exists. The rolling hash window correctly computes window hashes in O(1) per slide. Hash set insertion/lookup is O(1) amortized. The predicate "there exists a duplicate of length L" is monotone: if length L has a duplicate, so does L-1 (any prefix of the duplicate). Therefore binary search is valid. ∎

**False positives:** With double hashing, the probability of a false positive causing a wrong answer is negligible (≈ n/M₁M₂ ≈ 0). ∎

**Time:** O(n log n) — O(log n) binary search levels, each doing an O(n) rolling hash scan. ∎

---

## Collision probability

For a fixed (wrong) comparison (two different strings of length `L`):
- Single hash: collision probability ≤ L/M ≈ 10⁻³ for M=10⁹, L=10⁶.
- Double hash: ≤ L²/(M₁·M₂) ≈ 10⁻⁶.

Over `n` comparisons, expected false positives ≈ n·L/(M₁·M₂) — effectively 0 for all practical inputs.

---

## References

- R. M. Karp and M. O. Rabin, "Efficient randomized pattern-matching algorithms," IBM Journal of R&D, 1987.
- CP-Algorithms: "Rabin-Karp Algorithm for String Matching."
- Leetcode 1044 "Longest Duplicate Substring" — canonical use of binary search + rolling hash.

