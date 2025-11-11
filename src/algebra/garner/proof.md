# Proof of Garner's algorithm

## 1. Overview

Garner's algorithm reconstructs an integer `x` from its residues

```
x ≡ a_i  (mod m_i),  i = 0..k-1
```

when the moduli `m_0, m_1, ..., m_{k-1}` are pairwise coprime. Instead of computing the full product `M = ∏ m_i` (which may not fit in machine integers), Garner computes `x` in a mixed-radix form

```
x = c_0 + c_1 m_0 + c_2 m_0 m_1 + ... + c_{k-1} m_0 m_1 ... m_{k-2}
```

where each coefficient `c_i` is in `[0, m_i-1]` and can be computed using only modular arithmetic with moduli `m_i`.

## 2. Notation and assumptions

- `m_i` (for `i = 0..k-1`) are positive integers, pairwise coprime: `gcd(m_i, m_j) = 1` for `i != j`.
- `a_i` are the residues with `0 <= a_i < m_i`.
- `P_t` denotes the partial product

```
P_t = m_0 m_1 ... m_{t-1},   P_0 = 1.
```

- By the Chinese Remainder Theorem (CRT) there exists a unique solution `x` modulo `M = ∏_{i=0}^{k-1} m_i`.

## 3. Statement (what Garner constructs)

Garner computes integers `c_0, c_1, ..., c_{k-1}` with `0 <= c_i < m_i` such that

(1)  x = c_0 + c_1 P_1 + c_2 P_2 + ... + c_{k-1} P_{k-1}.

This `x` satisfies `x ≡ a_i (mod m_i)` for all `i`, and the mixed-radix coefficients `c_i` are unique.

## 4. Constructive proof (induction)

We prove by induction on `i` that after computing coefficients `c_0..c_i` the partial sum

```
x_i = c_0 + c_1 P_1 + ... + c_i P_i
```

satisfies

```
for all 0 <= j <= i:  x_i ≡ a_j  (mod m_j).
```

### Base case (`i = 0`)

Take `c_0 = a_0`. Then `x_0 = c_0 = a_0`, so `x_0 ≡ a_0 (mod m_0)`.

### Inductive step

Assume the invariant holds for `i-1`: the partial `x_{i-1}` satisfies `x_{i-1} ≡ a_j (mod m_j)` for all `j < i`.

We need to choose `c_i` such that

```
x_i = x_{i-1} + c_i P_i ≡ a_i  (mod m_i).
```

Rearrange to obtain

```
c_i P_i ≡ (a_i - x_{i-1})  (mod m_i).
```

Because `gcd(P_i, m_i) = 1` (pairwise coprime assumption), `P_i` has a multiplicative inverse modulo `m_i`. Let

```
invP = P_i^{-1} (mod m_i).
```

Multiply both sides by `invP` to get the unique solution for `c_i` in `Z_{m_i}`:

```
c_i ≡ invP * (a_i - x_{i-1})  (mod m_i).
```

Choose the representative `c_i` in `[0, m_i-1]`. This completes the induction.

After `i = k-1` steps we obtain `x = x_{k-1}` satisfying all congruences. Uniqueness modulo `M` follows from CRT; uniqueness of the `c_i` (mixed-radix digits) follows because each `c_i` is chosen in the canonical range `[0, m_i-1]`.

## 5. Triangular (Garner) recurrence — avoiding large products

Directly computing `invP = P_i^{-1} (mod m_i)` requires forming the product `P_i`, which may overflow. Garner avoids this by computing a triangular table of small inverses and applying a recurrence.

Define for `0 <= j < i`:

```
inv[j][i] = (m_j)^{-1} (mod m_i).
```

Start with `c_i = a_i`. For each `j = 0..i-1` update:

```
c_i := inv[j][i] * (c_i - c_j)  (mod m_i).
```

After processing all `j < i` the final `c_i` equals the value obtained by the `invP` formula. Intuitively, each step removes the contribution of `c_j` and effectively divides by `m_j` modulo `m_i`, so the product of these divisions equals `P_i^{-1}` modulo `m_i`.

### Formal inner induction (sketch)

Let `c_i^{(r)}` be the temporary value after processing indices `0..r`. One proves by inner induction on `r` that

```
c_i^{(r)} ≡ (a_i - sum_{t=0..r} c_t P_t) * (product_{u=r+1..i-1} m_u)^{-1}  (mod m_i).
```

For `r = i-1` the right-hand product is empty (equals `1`), giving the desired `c_i ≡ invP * (a_i - sum_{t=0..i-1} c_t P_t)`.

## 6. Algorithm (pseudocode)

```
function garner(a[0..k-1], m[0..k-1], MOD = 0):
  // a: residues, m: pairwise-coprime moduli
  // MOD = 0 => return mixed-radix coefficients c[0..k-1]
  // MOD > 0 => return x modulo MOD (useful when reconstructing under a final modulus)

  c = copy(a)
  // Precompute triangular inverses (optional)
  for i in 0..k-1:
    for j in 0..i-1:
      inv[j][i] = modinv(m[j] % m[i], m[i])

  for i in 0..k-1:
    for j in 0..i-1:
      c[i] = (inv[j][i] * (c[i] - c[j])) mod m[i]

  if MOD == 0:
    return c   // mixed-radix coefficients

  result = 0
  mult = 1
  for i in 0..k-1:
    result = (result + c[i] * mult) mod MOD
    mult = (mult * m[i]) mod MOD
  return result
```

Notes on the implementation:
- `modinv(a, m)` is computed with the extended Euclidean algorithm (returns the inverse in `[0, m-1]`).
- If `MOD > 0` and `MOD` fits into machine integer, use a wider intermediate type (e.g. `__int128`) when computing `result` and `mult` to avoid overflow.

## 7. Example

Let `m = [3,5,7]` and `a = [2,3,2]`.

- Precompute inverses (mod small moduli):
  - `inv[0][1] = inv(3 mod 5, 5) = 2` (since `3*2 ≡ 1 (mod 5)`).
  - `inv[0][2] = inv(3 mod 7, 7) = 5`.
  - `inv[1][2] = inv(5 mod 7, 7) = 3`.

- Compute coefficients:
  - `c[0] = a[0] = 2`.
  - `c[1] = inv[0][1] * (a1 - c0) mod 5 = 2 * (3-2) mod 5 = 2`.
  - `c[2]`:
    - after j=0: `c2 = inv[0][2] * (2-2) mod 7 = 0`.
    - after j=1: `c2 = inv[1][2] * (0-2) mod 7 = 3 * 5 mod 7 = 1`.
  - So `c = [2,2,1]`.

- Reconstruct (canonical): `x = 2 + 2*3 + 1*(3*5) = 2 + 6 + 15 = 23`. Check: `23 % 3 = 2`, `23 % 5 = 3`, `23 % 7 = 2`.

## 8. Complexity

- Time: naive Garner with precomputed `inv[j][i]` costs O(k^2) modular inverses (or O(k^2) modular ops if inverses are computed on the fly) and O(k^2) overall arithmetic operations.
- Memory: O(k^2) if you store the inverse table, or O(k) if you compute inverses on demand.

## 9. Practical notes and pitfalls

- Garner assumes pairwise-coprime moduli. For non-coprime moduli, first use the generalized CRT merge (pairwise merge with gcd checks) to reduce to coprime blocks.
- Always normalize residues into `[0, m_i-1]` before processing.
- When reconstructing to a final modulus `MOD` that fits in 64 bits, use `__int128` for intermediate multiplications to avoid overflow.
- If you need the canonical solution modulo `M = ∏ m_i` and `M` does not fit native integers, output the mixed-radix coefficients instead of a single integer.

## 10. References

- R. P. Garner, "The Residue Number System", Communications of the ACM, 1962.
- CP-algorithms: Garner's algorithm (implementation notes and examples).
- Standard textbooks on number theory and CRT.
