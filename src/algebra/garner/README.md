# Garner's algorithm — concise guide

Garner's algorithm is a practical method to reconstruct an integer from its residues modulo several pairwise-coprime moduli. Garner is particularly useful when the product of moduli is large (bigger than native integer types) but each modulus fits in machine word.

## Summary:
- Input: residues a0, a1, ..., a_{k-1} and pairwise-coprime moduli m0, m1, ..., m_{k-1}.
- Goal: find x such that x ≡ ai (mod mi) for all i, and represent x in a form that avoids large intermediate multiplications.
- Garner computes coefficients c0..c_{k-1} such that:
  `x = c0 + c1*m0 + c2*m0*m1 + ... + c_{k-1}*m0*m1*...*m_{k-2}`
  Each ci is computed modulo the corresponding modulus (or modulo a target modulus if reconstructing modulo some final modulus).

Why use Garner
- Avoids computing the full product M = ∏ mi and large intermediate products.
- Works well for NTT/FFT convolution reconstruction, where each mi is a 32/64-bit prime and their product does not fit native types.
- Easier to implement safely with machine integers and modular inverses.

## Algorithm (pairwise-coprime moduli)
1. Ensure moduli mi are pairwise coprime. Garner assumes coprimality in the classic form.
2. Prepare an array `coeffs[k]` initialized with the input residues a[i].
3. Prepare a 2D table `inv[i][j]` or compute inverses on the fly: inv[i][j] = inv(m_i mod m_j, m_j) for i < j. These are modular inverses of m_i modulo m_j.
4. For i from 0 to k-1:
   - For j from 0 to i-1:
       `coeffs[i] = (inv[j][i] * (coeffs[i] - coeffs[j])) mod m_i`
     After this inner loop, coeffs[i] is the correct mixed-radix digit c_i in range [0, m_i-1].
5. Reconstruct x under a desired final modulus `MOD` (optional):
   - result = 0; multiplier = 1;
   - `for i from 0 to k-1:
       result = (result + coeffs[i] * multiplier) % MOD
       multiplier = (multiplier * m_i) % MOD`
   - If you want the canonical x modulo M (full product), choose MOD = M (big-integer) or output the mixed-radix vector `coeffs`.

Notes on computing inverses
- inv[j][i] = inverse(m_j, m_i) computed with extended gcd: inv such that m_j * inv ≡ 1 (mod m_i).
- Precomputing the triangular inverse table is O(k^2) modular inverses but each inverse is small (mod < machine word).
- Alternatively compute inverses as needed when i is fixed by repeated extgcd calls.

## Pseudocode (high-level)

```
function garner(a[0..k-1], m[0..k-1], MOD = 0):
  // MOD = 0 means return mixed-radix coeffs; otherwise reconstruct modulo MOD
  // Make local copy
  c = array copy of a
  inv = 2D array k x k, default 0
  for i in 0..k-1:
    for j in 0..i-1:
      inv[j][i] = modinv(m[j] % m[i], m[i])
  for i in 0..k-1:
    for j in 0..i-1:
      c[i] = (inv[j][i] * (c[i] - c[j])) mod m[i]
  if MOD == 0:
    return c  // mixed-radix coefficients
  result = 0
  mult = 1
  for i in 0..k-1:
    result = (result + (c[i] * mult) ) % MOD
    mult = (mult * m[i]) % MOD
  return result
```

C++ sketch (safe use of 128-bit for intermediate multiplications when MOD fits 64-bit)

```cpp
// assumes positive residues and pairwise-coprime moduli
using i64 = long long;
using i128 = __int128_t;

// modular inverse (a * x ≡ 1 (mod m)), returns x in [0,m-1]
i64 modinv(i64 a, i64 m) {
  i64 x0 = 1, y0 = 0;
  // extended gcd iterative or recursive implementation (omitted for brevity)
  // compute x such that a*x % m == 1
}

// Garner: returns x modulo MOD if MOD > 0, otherwise returns mixed-radix coefficients in vector
std::vector<i64> garner(const std::vector<i64>& a, const std::vector<i64>& m, i64 MOD=0) {
  int k = a.size();
  std::vector<i64> c = a;
  std::vector<std::vector<i64>> inv(k, std::vector<i64>(k, 0));
  for (int i = 0; i < k; ++i) {
    for (int j = 0; j < i; ++j) {
      inv[j][i] = modinv(m[j] % m[i], m[i]);
    }
    for (int j = 0; j < i; ++j) {
      i64 t = c[i] - c[j];
      t %= m[i]; if (t < 0) t += m[i];
      c[i] = (i128)inv[j][i] * t % m[i];
    }
  }

  if (MOD == 0) return c; // mixed-radix

  i64 result = 0;
  i64 mult = 1;
  for (int i = 0; i < k; ++i) {
    result = ( (i128)result + (i128)c[i] * mult ) % MOD;
    mult = (i128)mult * m[i] % MOD;
  }
  return std::vector<i64>{result};
}
```

## Example
- Suppose moduli m = [3,5,7], residues a = [2,3,2].
  - Step compute inverses: inv[0][1] = inv(3 mod 5, 5) = 2 (because 3*2=6 ≡1 mod 5), inv[0][2] = inv(3 mod 7,7)=5, inv[1][2] = inv(5 mod 7,7)=3.
  - Compute c[0]=2.
    For i=1: c[1] = (inv[0][1] * (a1 - c0)) mod 5 = 2 * (3-2) mod5 = 2.
    For i=2: first j=0: c2 = inv[0][2]*(2-2)=0; j=1: c2 = inv[1][2]*(0-2)=3*( -2 mod7=5)=3*5=15 mod7=1. So c = [2,2,1].
  - Reconstruct x = 2 + 2*3 + 1*(3*5) = 2 + 6 + 15 = 23 (same as CRT example).

## Complexity and memory
- Time: O(k^2) modular operations for naive implementation (building inv table and coefficient transforms).
- Memory: O(k^2) if precomputing inv table; can be reduced if inverses are computed on the fly (time/space trade-off).

## Pitfalls & practical tips
- Garner needs pairwise-coprime moduli in its classic form. For non-coprime moduli, use generalized CRT merging (pairwise merge with gcd checks) before applying Garner on coprime blocks.
- Always normalize intermediate remainders to [0, mi-1].
- When reconstructing modulo a final MOD (e.g., 64-bit), ensure `mult` and `result` use a type wide enough for intermediate products; use `__int128` for safety when MOD fits 64-bit.
- For NTT reconstruction: typically choose MOD as the target prime (e.g., 2^64 via unsigned wrap) and compute Garner under that MOD to obtain the final coefficients.

## Use-cases
- Reconstruct large integers from residues produced by multi-prime NTT convolutions.
- Combine modular computations performed in different moduli without big-integer arithmetic.

## References
- Garner, R. P. (1962). 'The Residue Number System', Communications of the ACM.
- Competitive programming resources: CP-algorithms Garner's algorithm page and implementation notes.
- Wikipedia: Garner's algorithm, Chinese Remainder Theorem.


