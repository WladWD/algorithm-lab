# Principle of Inclusion-Exclusion (PIE) & Bitwise Operations

## 1. Mathematical Concept: Inclusion-Exclusion

The Principle of Inclusion-Exclusion is a counting technique used to compute the size of a union of finite sets. It states that to calculate the size of the union of several sets, one must:
1. Sum the sizes of the individual sets.
2. Subtract the sizes of the intersections of every pair of sets.
3. Add back the sizes of the intersections of every triple of sets.
4. Continue this alternating process.

### General Formula
For finite sets $A_1, \dots, A_n$, the size of their union is:

$$
\left| \bigcup_{i=1}^n A_i \right| = \sum |A_i| - \sum |A_i \cap A_j| + \sum |A_i \cap A_j \cap A_k| - \dots + (-1)^{n-1} |A_1 \cap \dots \cap A_n|
$$

<img width="200" height="160" alt="image" src="https://github.com/user-attachments/assets/3cfb835d-d67c-4ce2-a039-3fda280324f0" />

## 2. Application: "Number of Effective Subsequences"

A common variation of PIE in Competitive Programming is determining how many subsequences of an array have a Bitwise OR sum equal to a specific target $K$.

### The Problem
Given an array `nums`, count the number of subsequences where the **Bitwise OR** of the elements equals the total OR of the entire array.

### The Logic (Mobius Inversion on the Boolean Lattice)
Directly finding subsequences that equal exactly target $M$ is difficult. It is much easier to find subsequences where the OR sum is a **subset** of $M$ (i.e., the OR sum consists only of bits present in $M$).

Let $S$ be a subset of bits from the total OR $M$.
1. Let $count(S)$ be the count of numbers in `nums` such that $(num \ | \ S) == S$ (the number is a submask of $S$).
2. The number of subsequences we can form using only these numbers is $2^{count(S)}$.
3. Using PIE (specifically Mobius Inversion), we can isolate the count for exactly $M$:

$$
\text{Ans} = \sum_{S \subseteq M} (-1)^{|M| - |S|} 2^{count(S)}
$$

*Note: The sign depends on the parity of the number of "missing" bits ($|M| - |S|$).*

---

## 3. Solution Implementation

The following C++ solution implements the strategy described above. It utilizes **Sum Over Subsets (SOS) DP** to efficiently calculate submask frequencies.

### Code Structure

```cpp
#include <vector>
#include <bit>
#include <cstdint>
#include <iostream>

int countEffective(std::vector<int>& nums) {
    static constexpr int64_t MOD = 1'000'000'007;

    // 1. Calculate Target Mask (M)
    // Find the total OR of all numbers. This is our target bits.
    uint32_t m = 0;
    for (auto v: nums) {
        m |= v;
    }
    
    // Determine range for DP (next power of 2)
    uint32_t maxBit = 32 - std::countl_zero(m);

    // 2. Frequency Array
    // Count occurrences of each number.
    std::vector<int32_t> cnt(1 << maxBit);
    for (auto v: nums) {
        ++cnt[v];
    }

    // 3. Sum Over Subsets (SOS) DP / Yates DP
    // Transform cnt[mask] from "count of numbers == mask"
    // to "count of numbers that are submasks of mask".
    for (int32_t i = 0; i < maxBit; ++i) {
        for (int32_t mask = 0; mask < cnt.size(); ++mask) {
            if (mask & (1 << i)) {
                cnt[mask] += cnt[mask ^ (1 << i)];
            }
        }
    }

    // 4. Precompute Powers of 2
    // p2[k] = 2^k % MOD
    std::vector<int32_t> p2(nums.size() + 1);
    p2[0] = 1;
    for (int32_t i = 1; i < p2.size(); ++i) {
        p2[i] = (p2[i - 1] << 1) % MOD;
    }

    // 5. Principle of Inclusion-Exclusion
    // Iterate over all submasks of m to perform Mobius Inversion.
    int32_t mask = (m - 1) & m; 
    int64_t result = 0;
    
    while (1) {
        // x represents the bits missing from the Total OR (m)
        uint32_t x = m ^ mask; 
        
        // Count how many bits are missing
        int32_t c = std::popcount(x);

        // Apply PIE Logic:
        // If odd bits missing, ADD. If even bits missing, SUBTRACT.
        // (Note: Signs may flip depending on definition of empty set logic, 
        // strictly following standard Mobius for Subset Lattice).
        int64_t term = p2[cnt[mask]];
        
        if (c & 1) {
            result = (result + term) % MOD;
        } else {
            result = (result - term + MOD) % MOD;
        }

        if (!mask) break;
        
        // Efficient submask iteration trick
        mask = (mask - 1) & m;
    }
    
    return result;
}
```

## Step-by-Step Explanation

### Step 1: Target Identification
We iterate through nums to find m, the Bitwise OR of the entire array. This is the "universe" of bits we care about. Any subsequence with an OR sum equal to m is an "effective" subsequence (based on the problem context).

### Step 2: Sum Over Subsets (SOS) DP
Standard brute force to count submasks takes $O(N * 2^m)$. SOS DP optimizes this to $O(m \cdot 2^m)$.
- Initial state: cnt[x] holds the frequency of number x in the array.
- Transition: We iterate through every bit position i. If a mask has the i-th bit set, we add the count of the mask with the i-th bit turned off.
- Result: After running this for all bits, cnt[mask] contains the count of all numbers in the input array that are subsets of mask (i.e., (num & mask) == num).

### Step 3: Powers of 2
For a given mask, we have cnt[mask] available numbers. The number of subsequences we can form using strictly these numbers is $2^{cnt[mask]}$.

### Step 4: PIE Iteration
We iterate through all submasks of m. The iteration logic mask = (mask - 1) & m ensures we visit every subset of the bits in m efficiently.

The Formula Logic: We calculate the contribution of every submask.

- x = m ^ mask gives us the bits that are missing from the current mask compared to the target m.
- If we are missing an Odd number of bits (c & 1), we Add.
- If we are missing an Even number of bits, we Subtract.

Note on result: Depending on whether you want to calculate the union or the intersection, or count "exactly m" vs "at most m", the starting sign might differ. This implementation assumes an alternating sum structure to filter out subsequences that do not have all bits of m set.

## Appendix: Mathematical Proof of PIE

Why does adding and subtracting intersection sizes result in the correct count of the union? We can prove this by tracking how many times an arbitrary element $x$ is counted.

### The Claim
We want to show that every element $x$ that belongs to at least one of the sets $A_1, \dots, A_n$ contributes exactly **1** to the total sum.

### The Proof
Let $x$ be an element that belongs to exactly $k$ of the sets (where $k \ge 1$).

1.  **First Term ($\sum |A_i|$):**
    Since $x$ is present in $k$ sets, it is counted $\binom{k}{1}$ times.
    *Contribution:* $+\binom{k}{1}$

2.  **Second Term ($-\sum |A_i \cap A_j|$):**
    $x$ is present in the intersection of a pair of sets only if both sets are among the $k$ sets containing $x$. There are $\binom{k}{2}$ such pairs.
    *Contribution:* $-\binom{k}{2}$

3.  **Third Term ($+\sum |A_i \cap A_j \cap A_l|$):**
    $x$ is present in the intersection of a triple only if all three sets are among the $k$ sets.
    *Contribution:* $+\binom{k}{3}$

4.  **General Term:**
    For the intersection of $m$ sets, $x$ is counted $\binom{k}{m}$ times. The sign alternates based on $m$.

### The Total Sum
The total contribution of element $x$ to the PIE formula is:

$$S = \binom{k}{1} - \binom{k}{2} + \binom{k}{3} - \dots + (-1)^{k-1}\binom{k}{k}$$

To solve this, recall the **Binomial Expansion** of $(1 - 1)^k$:

$$(1 - 1)^k = \sum_{j=0}^{k} \binom{k}{j}(-1)^j$$

$$0 = \binom{k}{0} - \binom{k}{1} + \binom{k}{2} - \binom{k}{3} + \dots$$

Since $\binom{k}{0} = 1$, we can rearrange the equation:

$$1 = \binom{k}{1} - \binom{k}{2} + \binom{k}{3} - \dots$$

### Conclusion
The total sum $S$ equals **1**.
Therefore, every element present in the union is counted exactly once. Elements present in 0 sets are counted 0 times.