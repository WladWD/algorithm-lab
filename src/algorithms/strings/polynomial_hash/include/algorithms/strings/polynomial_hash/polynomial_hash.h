#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace algorithms::strings::polynomial_hash {

// Polynomial (Rabin-style) string hashing.
//
// Computes hash(S) = S[0]*B^(n-1) + S[1]*B^(n-2) + ... + S[n-1]*B^0  (mod M)
//
// Provides O(1) substring hash queries after O(n) preprocessing.
//
// Double-hashing (two independent (B, M) pairs) is supported to reduce
// collision probability to ~1/M1M2 ≈ 1/10^36 in practice.
//
// Common choices:
//   Base:    131, 137, 31 (avoid 0; choose coprime to M)
//   Modulus: Large primes, e.g. 1e9+7, 1e9+9, or 2^61-1

static constexpr uint64_t kMod1 = 1'000'000'007ULL;  // 1e9+7
static constexpr uint64_t kMod2 = 1'000'000'009ULL;  // 1e9+9
static constexpr uint64_t kBase1 = 131ULL;
static constexpr uint64_t kBase2 = 137ULL;

/// Single-modulus polynomial hash over a string.
/// Supports O(1) substring hash queries after O(n) build.
struct PolyHash {
    explicit PolyHash(std::string_view s,
                      uint64_t base = kBase1,
                      uint64_t mod  = kMod1);

    /// Hash of s[l..r] (inclusive, 0-indexed). O(1).
    [[nodiscard]] uint64_t query(std::size_t l, std::size_t r) const;

    /// Hash of the full string. O(1).
    [[nodiscard]] uint64_t full() const;

    std::size_t size() const noexcept { return n_; }

private:
    uint64_t base_, mod_;
    std::size_t n_;
    std::vector<uint64_t> h_;     // prefix hashes
    std::vector<uint64_t> pw_;    // base powers
};

/// Double-hash: two independent (base, mod) pairs for near-zero collision probability.
struct DoubleHash {
    explicit DoubleHash(std::string_view s,
                        uint64_t base1 = kBase1, uint64_t mod1 = kMod1,
                        uint64_t base2 = kBase2, uint64_t mod2 = kMod2);

    /// Returns {hash1(s[l..r]), hash2(s[l..r])}. O(1).
    [[nodiscard]] std::pair<uint64_t, uint64_t> query(std::size_t l, std::size_t r) const;

    /// Full-string double hash. O(1).
    [[nodiscard]] std::pair<uint64_t, uint64_t> full() const;

    std::size_t size() const noexcept { return h1_.size(); }

private:
    PolyHash h1_, h2_;
};

/// Compare two substrings for equality using double hashing. O(1) expected.
bool substrings_equal(const DoubleHash& a, std::size_t la, std::size_t ra,
                      const DoubleHash& b, std::size_t lb, std::size_t rb);

/// Find the longest common prefix length of s[ia..] and s[ib..] using binary
/// search over the double-hash. O(log n) expected.
std::size_t lcp(const DoubleHash& s, std::size_t ia, std::size_t ib);

/// Find all (0-indexed) starting positions of pattern `p` in text `t`. O((n+m) expected).
std::vector<std::size_t> find_all(std::string_view t, std::string_view p,
                                   uint64_t base1 = kBase1, uint64_t mod1 = kMod1,
                                   uint64_t base2 = kBase2, uint64_t mod2 = kMod2);

} // namespace algorithms::strings::polynomial_hash

