# Rabin-Karp — Proof of Correctness

## Hash function definition

For a string `s[0..m-1]` over a byte alphabet, the **polynomial hash** with base B and modulus P is:

```
h(s) = s[0]·B^(m-1) + s[1]·B^(m-2) + … + s[m-1]·B^0  (mod P)
```

This is the evaluation of `s` as a base-B number modulo P.

---

## Rolling hash: O(1) window update

Given the hash of window `s[i..i+m-1]`, the hash of the next window `s[i+1..i+m]` is:

```
h(s[i+1..i+m]) = h(s[i..i+m-1]) · B  −  s[i] · B^m  +  s[i+m]   (mod P)
```

**Derivation:**

```
h(s[i..i+m-1]) · B = s[i]·B^m + s[i+1]·B^(m-1) + … + s[i+m-1]·B^1

Subtract s[i]·B^m:
  s[i+1]·B^(m-1) + … + s[i+m-1]·B^1

Add s[i+m]:
  s[i+1]·B^(m-1) + … + s[i+m-1]·B^1 + s[i+m]·B^0  =  h(s[i+1..i+m])  ∎
```

The high-power constant `B^m mod P` is computed once before the scan.

---

## Correctness of pattern matching

**Soundness:** A position `i` is reported only when `wh == ph` AND the window `text[i..i+m-1]` equals `pattern` by direct character comparison. Therefore every reported match is a true occurrence. ✓

**Completeness:** If `text[i..i+m-1] == pattern`, then by the definition of the hash function `wh == ph`. The hash-equal branch is reached, the direct comparison confirms equality, and position `i` is added to the result. No true occurrence is ever skipped. ✓

---

## Time complexity

### Single pattern: expected O(n + m)

- Preprocessing: O(m) to compute `ph` and the initial window hash `wh₀`.
- Main loop runs `n − m + 1` iterations; each iteration does O(1) hash arithmetic.
- Verification (direct comparison) is O(m), but occurs only on hash matches.
- Under a random hash, the probability of a false positive at any position is `1/P`. Expected false positives over `n` positions: `n/P ≈ 0` for P = 10⁹+7 and typical `n`. Total expected comparison cost: O(n/P · m) = O(1).
- **Expected total: O(n + m).**
- Worst case: all windows collide (e.g., all characters equal, adversarial hash), giving O(n·m) comparisons.

### Multi-pattern: expected O(n + k·m)

- Building the hash set and multimap: O(k·m).
- Main loop: O(n) hash updates + O(k·m) expected verification cost.
- **Expected total: O(n + k·m).**

---

## Hash collision probability

A false positive occurs when `h(text[i..i+m-1]) ≡ h(pattern) (mod P)` despite the strings being different. The difference polynomial `Δ(B) = Σ (tᵢ - pⱼ)·B^j` has at most `m` roots modulo a prime P. For a uniformly random base B the false positive probability is at most `m/P`. With B = 131 (fixed) this is not a probabilistic bound, but in practice collisions are negligibly rare for natural inputs.

For adversarial inputs, use a randomly chosen base or double hashing (two independent (B, P) pairs) to reduce the collision probability to near zero.

---

## References

- M. O. Rabin and R. M. Karp, "Efficient randomized pattern-matching algorithms," IBM J. Res. Dev., 1987.
- CP-Algorithms: "Rabin-Karp Algorithm" — practical rolling hash treatment.
- T. H. Cormen et al., *Introduction to Algorithms (CLRS)*, Chapter 32.2.
