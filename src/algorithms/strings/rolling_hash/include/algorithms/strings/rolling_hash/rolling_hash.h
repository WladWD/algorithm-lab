#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace algorithms::strings::rolling_hash {

// Rolling Hash (Rabin-Karp sliding window).
//
// Maintains a hash over a fixed-length window as it slides across a string,
// updating in O(1) per step by "rolling out" the leftmost character and
// "rolling in" the rightmost.
//
// hash = S[i]*B^(w-1) + S[i+1]*B^(w-2) + ... + S[i+w-1]*B^0  (mod M)
//
// update: hash_new = (hash - S[i]*B^(w-1)) * B + S[i+w]
//
// Use cases:
//   - Rabin-Karp pattern matching: O(n+m) expected.
//   - Duplicate window detection, anagram matching.
//   - 2D rolling hash over matrices.

static constexpr uint64_t kMod1  = 1'000'000'007ULL;
static constexpr uint64_t kMod2  = 1'000'000'009ULL;
static constexpr uint64_t kBase1 = 131ULL;
static constexpr uint64_t kBase2 = 137ULL;

/// Single-modulus rolling hash window of fixed length `window`.
struct RollingHash {
    /// Build over s[0..window-1]. `window` must be > 0 and ≤ s.size().
    RollingHash(std::string_view s, std::size_t window,
                uint64_t base = kBase1, uint64_t mod = kMod1);

    /// Current hash value.
    [[nodiscard]] uint64_t value() const noexcept { return h_; }

    /// Slide the window one position to the right: drop s[pos], add s[pos+window].
    /// Returns the new hash. `next_char` is s[pos + window].
    uint64_t slide(char drop_char, char add_char);

    std::size_t window() const noexcept { return window_; }
    uint64_t mod()  const noexcept { return mod_; }
    uint64_t base() const noexcept { return base_; }

private:
    uint64_t base_, mod_;
    std::size_t window_;
    uint64_t h_;       // current hash
    uint64_t high_pw_; // base^(window-1) mod M
};

/// Double-modulus rolling hash for reduced collision probability.
struct DoubleRollingHash {
    DoubleRollingHash(std::string_view s, std::size_t window,
                      uint64_t base1 = kBase1, uint64_t mod1 = kMod1,
                      uint64_t base2 = kBase2, uint64_t mod2 = kMod2);

    [[nodiscard]] std::pair<uint64_t, uint64_t> value() const noexcept {
        return {h1_.value(), h2_.value()};
    }

    std::pair<uint64_t, uint64_t> slide(char drop_char, char add_char);

    std::size_t window() const noexcept { return h1_.window(); }

private:
    RollingHash h1_, h2_;
};

/// Rabin-Karp: find all occurrences of pattern `p` in text `t`. O(n+m) expected.
/// Returns sorted list of 0-indexed starting positions.
std::vector<std::size_t> rabin_karp(std::string_view t, std::string_view p,
                                     uint64_t base1 = kBase1, uint64_t mod1 = kMod1,
                                     uint64_t base2 = kBase2, uint64_t mod2 = kMod2);

/// Find the length of the longest duplicate substring using rolling hash + binary search.
/// O(n log n) expected. Returns the length (0 if no duplicate exists).
std::size_t longest_duplicate_substring(std::string_view s,
                                         uint64_t base1 = kBase1, uint64_t mod1 = kMod1,
                                         uint64_t base2 = kBase2, uint64_t mod2 = kMod2);

} // namespace algorithms::strings::rolling_hash

