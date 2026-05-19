# Z-Function — Proof of Correctness

## Definition

For a string `s[0..n-1]`, the **Z-array** is defined as:
- `z[0] = 0` (by convention; the full string matches the prefix trivially)
- `z[i] = max { k : s[0..k-1] = s[i..i+k-1] }` for `i ≥ 1`

That is, `z[i]` is the length of the longest prefix of `s` that starts at position `i`.

---

## Z-box invariant

The algorithm maintains a **Z-box** `[l, r)`: the rightmost interval such that `s[l..r-1] = s[0..r-l-1]`. This means position `r` is the furthest right we have "confirmed" a match.

At the start of iteration `i`:
- If `i ≥ r`: no Z-box extends to `i`. Compute `z[i]` by naive extension from 0. Update `[l, r)` if `z[i] > 0`.
- If `i < r`: we know `s[i..r-1] = s[i-l..r-l-1]`. Let `k = i - l`. Then `z[i] ≥ min(z[k], r - i)`.
  - If `z[k] < r - i`: `z[i] = z[k]` (the match length is exactly `z[k]`, since `s[k+z[k]] ≠ s[z[k]]` tells us the Z-box at `k` ends before reaching `r-l`).
  - If `z[k] ≥ r - i`: `z[i] ≥ r - i`, and we extend naively from position `r`.

---

## Correctness

**Claim:** After the algorithm completes, `z[i]` equals the length of the longest prefix of `s` matching `s[i..]`.

**Proof (by invariant):**

The Z-box `[l, r)` satisfies `s[l..r-1] = s[0..r-l-1]` at all times. This is maintained:
- When `i ≥ r`: the naive extension directly computes `z[i]` by definition. If it extends past `r`, the box is updated to `[i, i+z[i])`, preserving the invariant.
- When `i < r` and `z[k] < r-i`: We know `s[k..k+z[k]-1] = s[0..z[k]-1]` and `s[k+z[k]] ≠ s[z[k]]`. Translating via the Z-box: `s[i..i+z[k]-1] = s[0..z[k]-1]` and `s[i+z[k]] ≠ s[z[k]]`. So `z[i] = z[k]` exactly. No extension needed. ✓
- When `i < r` and `z[k] ≥ r-i`: We have at least `r-i` characters confirmed. Naive extension from `r` (in `s`) vs `r-i` (in the prefix) finds the exact `z[i]`. ✓

In all cases, `z[i]` is set to the correct value. ∎

---

## Time complexity: O(n)

`r` is a non-decreasing value in `[0, n]`. Each character of `s` is visited at most twice: once when `r` is extended (i.e., `r` increases), and at most once again when accessed from inside a Z-box (without causing `r` to increase). Total character comparisons: O(n). ∎

---

## Pattern search via Z-function

### Construction

Form the virtual string `V = pattern + '\0' + text` (length `m + 1 + n`). A position `i` in the text corresponds to offset `m + 1 + i` in `V`.

**Claim:** `z_V[m + 1 + i] == m` iff `text[i..i+m-1] == pattern`.

**Proof:** `z_V[j]` is the longest prefix of `V` matching `V[j..]`. Since `'\0'` cannot appear in the pattern or text, the match cannot extend past position `m` in the prefix. Therefore:
- If `text[i..i+m-1] == pattern`, then `z_V[m+1+i] ≥ m`. The sentinel prevents `> m`. So `z_V[m+1+i] = m`. ✓
- If `z_V[m+1+i] = m`, then `V[0..m-1] = V[m+1+i..m+i] `, i.e., `pattern = text[i..i+m-1]`. ✓

∎

### Implementation note

This module avoids allocating the full concatenated string. Instead, a lambda maps virtual indices to actual characters, keeping space at O(m) for the Z-array of the pattern prefix.

---

## References

- CP-Algorithms: "Z-Function and Its Calculation."
- A. Apostolico, Z. Galil, "Pattern Matching Algorithms," Oxford University Press, 1997.

