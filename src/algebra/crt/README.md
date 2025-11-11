# Chinese Remainder Theorem (CRT)

This document summarizes the Chinese Remainder Theorem (CRT), practical algorithms for reconstructing integers from residues, and implementation notes useful for competitive programming and library code.

Why CRT
- CRT allows reconstructing an integer (mod N, product of moduli) from its residues modulo several pairwise-coprime moduli.
- Useful for combining independent modular computations, working around modulus size limits (FFT/NTT convolution using multiple primes), and solving congruence systems.

Statement (pairwise-coprime moduli)
- Given moduli m1, m2, ..., mk that are pairwise coprime (gcd(mi, mj) = 1 for i != j) and residues a1, a2, ..., ak, there exists a unique solution x modulo M = m1*m2*...*mk such that

  x ≡ ai (mod mi)  for every i.

Constructive solution (standard method)
1. Compute M = ∏ mi and for each i compute Mi = M / mi.
2. Compute the modular inverse yi = Mi^{-1} (mod mi) (use extended Euclid to get yi).
3. The solution is

   x = sum_{i=1..k} ai * Mi * yi  (mod M).

4. Reduce x modulo M to get the canonical representative in [0, M-1].

Notes:
- Compute modular inverse yi = inv(Mi, mi) using extended gcd. Because mi and Mi are coprime, inverse exists.
- Use 128-bit integers or reduce intermediate products modulo M to avoid overflow when M is large (or use big integer library).

Example (pairwise-coprime)
- Solve x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7).
  - M = 105. M1 = 35, M2 = 21, M3 = 15.
  - y1 = inv(35,3)=2 (because 35*2=70 ≡ 1 mod 3), y2 = inv(21,5)=1, y3 = inv(15,7)=1.
  - x = 2*35*2 + 3*21*1 + 2*15*1 = 140 + 63 + 30 = 233 ≡ 23 (mod 105).
  - So x = 23 is the minimal solution.

Generalized CRT (non-coprime moduli)
- For two congruences x ≡ a (mod m) and x ≡ b (mod n) with g = gcd(m, n):
  - A solution exists iff a ≡ b (mod g). If not, no solution.
  - If solution exists, reduce the system: let m' = m/g, n' = n/g. Solve for k in

      m * k ≡ b - a (mod n)

    which reduces to

      (m/g) * k ≡ (b-a)/g (mod n')

    and then x = a + m * k (reduce modulo lcm(m,n) = m'*n'*g = m*n/g).
- For many congruences, combine them pairwise iteratively (merge two at a time) using the above.

Practical two-modulus merge algorithm (sketch)
1. Given (a, m) and (b, n). Compute g = gcd(m, n), diff = b - a.
2. If diff % g != 0 -> no solution.
3. Compute m1 = m/g, n1 = n/g, rhs = diff/g mod n1.
4. Compute inv = inv(m1 mod n1, n1) via extended gcd; k = (rhs * inv) % n1.
5. x = a + m * k; new modulus = m * n1 (which equals lcm(m, n)). Reduce x modulo new modulus.

Garner's algorithm (stable incremental reconstruction)
- Garner's algorithm reconstructs x in mixed-radix form and is numerically stable (avoids big multiplications) when moduli are relatively small but their product is large.
- Works especially well when moduli are pairwise coprime.
- Basic idea: represent x = c0 + c1*m0 + c2*m0*m1 + ... and compute coefficients ci by solving small modular systems.
- Garner computes coefficients incrementally using modular inverses of previous moduli, avoiding computing the full product M.

When to use which method
- Standard Mi * yi method: simple, direct, straightforward when k is small and product M fits in 128-bit (or using big integers).
- Garner: preferred when M is huge (product of many primes) but each mi fits in machine word — Garner avoids large intermediate multiplications and is often used in NTT convolution reconstruction.
- Iterative merge (generalized CRT): necessary when moduli are not coprime.

Implementation notes
- Modular inverse: use extended Euclidean algorithm (extgcd) to compute inverse and to perform the two-modulus merge.
- Overflow: watch for overflow of products Mi * yi * ai — use __int128 on 64-bit builds or perform modular reductions early.
- Negative residues: always normalize residues to [0, mi-1] before processing.
- Deterministic Miller–Rabin + CRT: often used together to reconstruct large integers from residues modulo several 32/64-bit primes after NTT.

Complexity
- Standard method: O(k * log M) for inverse computations (extgcd) plus O(k) for summation; bit complexity depends on integer sizes.
- Garner: O(k^2) modular operations in the naive implementation (can be optimized), but avoids big-integer multiplications.

Common pitfalls
- Forgetting to check compatibility in generalized CRT (a ≡ b (mod gcd(m,n))).
- Overflow when computing M or products — use 128-bit or big integer library.
- Using naive Garner with non-coprime moduli (Garner assumes pairwise-coprime moduli unless modified).

Minimal C++ helpers (pseudocode)

- Modular inverse via extgcd (conceptual):

  // returns (g, x, y) such that a*x + b*y = g = gcd(a,b)
  tuple<long long,long long,long long> egcd(long long a, long long b);

  // inv(a, mod) -> modular inverse in [0,mod-1], assumes gcd(a,mod)==1
  long long inv(long long a, long long mod) {
    auto [g, x, y] = egcd(a, mod);
    // assert(g == 1);
    x %= mod; if (x < 0) x += mod;
    return x;
  }

References and further reading
- Wikipedia: Chinese Remainder Theorem
- Graham, Knuth, Patashnik — Concrete Mathematics (section on CRT)
- Garner's algorithm — useful notes and implementation tips (search for "Garner's algorithm CRT")


