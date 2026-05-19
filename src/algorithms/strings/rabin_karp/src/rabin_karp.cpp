#include "algorithms/strings/rabin_karp/rabin_karp.h"

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace algorithms::strings::rabin_karp {

namespace {

constexpr std::uint64_t kBase = 131ULL;
constexpr std::uint64_t kMod  = 1'000'000'007ULL;

// Compute B^exp mod kMod via fast exponentiation.
std::uint64_t pow_mod(std::uint64_t base, std::size_t exp) {
    std::uint64_t result = 1;
    base %= kMod;
    while (exp > 0) {
        if (exp & 1) result = result * base % kMod;
        base = base * base % kMod;
        exp >>= 1;
    }
    return result;
}

// Compute initial polynomial hash of s: s[0]*B^(n-1) + ... + s[n-1].
std::uint64_t hash_of(std::string_view s) {
    std::uint64_t h = 0;
    for (unsigned char c : s)
        h = (h * kBase + c) % kMod;
    return h;
}

// Roll the window hash one step to the right.
// out_char = character leaving the window (s[i])
// in_char  = character entering the window (s[i+m])
// high     = B^m mod kMod (precomputed)
std::uint64_t roll(std::uint64_t h, unsigned char out_char,
                   unsigned char in_char, std::uint64_t high) {
    h = h * kBase % kMod;
    h = (h + kMod - static_cast<std::uint64_t>(out_char) * high % kMod) % kMod;
    h = (h + in_char) % kMod;
    return h;
}

} // namespace

std::vector<std::size_t> search(std::string_view text, std::string_view pattern) {
    std::vector<std::size_t> result;
    if (pattern.empty() || pattern.size() > text.size()) return result;

    const std::size_t n = text.size(), m = pattern.size();
    const std::uint64_t high = pow_mod(kBase, m);
    const std::uint64_t ph   = hash_of(pattern);
    std::uint64_t wh         = hash_of(text.substr(0, m));

    for (std::size_t i = 0; i <= n - m; ++i) {
        if (wh == ph && text.substr(i, m) == pattern)
            result.push_back(i);
        if (i + m < n)
            wh = roll(wh,
                      static_cast<unsigned char>(text[i]),
                      static_cast<unsigned char>(text[i + m]),
                      high);
    }
    return result;
}

std::size_t count_occurrences(std::string_view text, std::string_view pattern) {
    return search(text, pattern).size();
}

std::vector<std::size_t> search_multiple(std::string_view text,
                                         const std::vector<std::string_view>& patterns) {
    if (patterns.empty()) return {};
    const std::size_t m = patterns[0].size();
    const std::size_t n = text.size();
    if (m == 0 || m > n) return {};

    // Map hash → list of patterns sharing that hash (for collision-safe lookup).
    std::unordered_multimap<std::uint64_t, std::string_view> pat_map;
    std::unordered_set<std::uint64_t> pat_hashes;
    for (auto sv : patterns) {
        std::uint64_t h = hash_of(sv);
        pat_hashes.insert(h);
        pat_map.emplace(h, sv);
    }

    const std::uint64_t high = pow_mod(kBase, m);
    std::uint64_t wh         = hash_of(text.substr(0, m));

    std::vector<std::size_t> result;
    for (std::size_t i = 0; i <= n - m; ++i) {
        if (pat_hashes.count(wh)) {
            std::string_view window = text.substr(i, m);
            auto [lo, hi] = pat_map.equal_range(wh);
            for (auto it = lo; it != hi; ++it) {
                if (it->second == window) {
                    result.push_back(i);
                    break; // at most one distinct string can match this window
                }
            }
        }
        if (i + m < n)
            wh = roll(wh,
                      static_cast<unsigned char>(text[i]),
                      static_cast<unsigned char>(text[i + m]),
                      high);
    }
    std::sort(result.begin(), result.end());
    return result;
}

} // namespace algorithms::strings::rabin_karp
