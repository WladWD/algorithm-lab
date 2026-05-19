# Polynomial String Hashing — Proof of Correctness

## Definition

Given a string `S[0..n-1]` and parameters base `B` and modulus `M`, the **polynomial hash** is:

```
H(S) = S[0]·B^(n-1) + S[1]·B^(n-2) + ... + S[n-1]·B^0  (mod M)
```

The **prefix hash array** is defined as:

```
h[0] = 0
h[i] = h[i-1]·B + S[i-1]   (mod M)
```

So `h[i]` holds the hash of `S[0..i-1]`.

The **power array** is:

```
pw[0] = 1
pw[i] = pw[i-1]·B  (mod M)
```

---

## Substring hash formula

**Claim:** `h[r+1] - h[l]·B^(r-l+1)  ≡  H(S[l..r])  (mod M)`

**Proof:**

```
h[r+1] = S[0]·B^r + S[1]·B^(r-1) + ... + S[l-1]·B^(r-l+1) + S[l]·B^(r-l) + ... + S[r]·B^0

h[l]·B^(r-l+1) = S[0]·B^r + S[1]·B^(r-1) + ... + S[l-1]·B^(r-l+1)
```

Subtracting:

```
h[r+1] - h[l]·B^(r-l+1) = S[l]·B^(r-l) + ... + S[r]·B^0 = H(S[l..r])  ✓
```

Since modular arithmetic is a ring, the subtraction is valid mod M (with an additive `M` to avoid negative values). ∎

---

## Substring equality

**Claim:** If `H(S[la..ra]) == H(T[lb..rb])` (for both hash pairs in double hashing), the substrings are equal with high probability.

**When equal strings collide:** If `S[la..ra] == T[lb..rb]`, their hashes are always equal (hash is deterministic).

**When different strings collide (false positive):** Two distinct strings of the same length `k` collide iff the polynomial `P(x) = Σ (S[i]-T[i]) · x^i` evaluates to 0 mod M. A degree-k polynomial has at most k roots mod M. The probability that a random base B is a root is ≤ k/M.

For M ≈ 10⁹ and k ≤ n ≤ 10⁶: probability per query ≈ 10⁶ / 10⁹ = 10⁻³.  
For double hashing (two independent moduli): ≈ (10⁻³)² = 10⁻⁶ per query, or ~10⁻³⁶ with M ≈ 10⁹ each. ∎

---

## Pattern search (`find_all`) correctness

The algorithm computes the double hash of pattern P, then slides a window over text T, computing the substring hash at each position in O(1). It reports a match wherever hashes agree.

**Correctness:** Every true occurrence is always reported (hashes of identical strings are equal). False positives occur with probability ~1/(M₁·M₂) per position. ∎

**Time:** O(n + m) — O(m) to hash the pattern, O(n) to scan all windows.

---

## LCP via binary search

**Claim:** `lcp(s, ia, ib)` correctly returns the length of the longest common prefix of `s[ia..]` and `s[ib..]`.

**Proof:** The binary search maintains the invariant:
- `lo`: there exists a prefix of length `lo` that matches (both positions agree on first `lo` chars).
- `hi`: all prefixes of length `> hi` differ.

The predicate "s[ia..ia+mid-1] == s[ib..ib+mid-1]" is evaluated in O(1) using double hashing. The binary search converges to the exact LCP length.

**Time:** O(log n) hash queries, each O(1). ∎

---

## Build complexity

**Time:** O(n) — one pass to compute `h[i]` and `pw[i]`.  
**Space:** O(n) — two arrays of length n+1.

---

## References

- M. O. Rabin, "Fingerprinting by random polynomials," TR-15-81, Harvard, 1981.
- CP-Algorithms: "String Hashing" — standard reference for competitive programming.
- D. E. Knuth, "The Art of Computer Programming," Vol. 3, Section 6.3.

