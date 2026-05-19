#include "algorithms/strings/rolling_hash/rolling_hash.h"

#include <unordered_set>

namespace algorithms::strings::rolling_hash {

// ─── RollingHash ─────────────────────────────────────────────────────────────

RollingHash::RollingHash(std::string_view s, std::size_t window,
                         uint64_t base, uint64_t mod)
    : base_(base), mod_(mod), window_(window), h_(0), high_pw_(1) {

    // Compute base^(window-1) mod M
    for (std::size_t i = 0; i + 1 < window; ++i) {
        high_pw_ = high_pw_ * base_ % mod_;
    }

    // Hash of s[0..window-1]
    for (std::size_t i = 0; i < window; ++i) {
        h_ = (h_ * base_ + static_cast<unsigned char>(s[i])) % mod_;
    }
}

uint64_t RollingHash::slide(char drop_char, char add_char) {
    // Remove contribution of drop_char at the highest power position.
    h_ = (h_ + mod_ - static_cast<unsigned char>(drop_char) * high_pw_ % mod_) % mod_;
    // Shift left and add new character.
    h_ = (h_ * base_ + static_cast<unsigned char>(add_char)) % mod_;
    return h_;
}

// ─── DoubleRollingHash ───────────────────────────────────────────────────────

DoubleRollingHash::DoubleRollingHash(std::string_view s, std::size_t window,
                                     uint64_t base1, uint64_t mod1,
                                     uint64_t base2, uint64_t mod2)
    : h1_(s, window, base1, mod1), h2_(s, window, base2, mod2) {}

std::pair<uint64_t, uint64_t> DoubleRollingHash::slide(char drop_char, char add_char) {
    return {h1_.slide(drop_char, add_char), h2_.slide(drop_char, add_char)};
}

// ─── Rabin-Karp ──────────────────────────────────────────────────────────────

std::vector<std::size_t> rabin_karp(std::string_view t, std::string_view p,
                                     uint64_t base1, uint64_t mod1,
                                     uint64_t base2, uint64_t mod2) {
    std::vector<std::size_t> result;
    const std::size_t n = t.size(), m = p.size();
    if (m == 0 || m > n) return result;

    DoubleRollingHash hp(p, m, base1, mod1, base2, mod2);
    DoubleRollingHash ht(t, m, base1, mod1, base2, mod2);

    auto target = hp.value();

    if (ht.value() == target) result.push_back(0);

    for (std::size_t i = 1; i + m <= n; ++i) {
        ht.slide(t[i - 1], t[i + m - 1]);
        if (ht.value() == target) {
            result.push_back(i);
        }
    }
    return result;
}

// ─── Longest Duplicate Substring ─────────────────────────────────────────────

std::vector<std::size_t> longest_duplicate_substring(std::string_view s,
                                                       std::size_t len,
                                                       uint64_t base1, uint64_t mod1,
                                                       uint64_t base2, uint64_t mod2) {
    const std::size_t n = s.size();
    if (len == 0 || len > n) return {};

    // Use an unordered set keyed on combined (h1, h2).
    struct PairHash {
        std::size_t operator()(std::pair<uint64_t, uint64_t> p) const noexcept {
            return std::hash<uint64_t>{}(p.first) ^ (std::hash<uint64_t>{}(p.second) << 32);
        }
    };

    std::unordered_set<std::pair<uint64_t, uint64_t>, PairHash> seen;
    DoubleRollingHash rh(s, len, base1, mod1, base2, mod2);
    seen.insert(rh.value());

    for (std::size_t i = 1; i + len <= n; ++i) {
        rh.slide(s[i - 1], s[i + len - 1]);
        if (!seen.insert(rh.value()).second) {
            // Collision found — we found a duplicate window of this length.
            return {i};  // return something non-empty to indicate "found"
        }
    }
    return {};
}

std::size_t longest_duplicate_substring(std::string_view s,
                                         uint64_t base1, uint64_t mod1,
                                         uint64_t base2, uint64_t mod2) {
    const std::size_t n = s.size();
    if (n <= 1) return 0;

    std::size_t lo = 0, hi = n - 1;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo + 1) / 2;
        if (!longest_duplicate_substring(s, mid, base1, mod1, base2, mod2).empty()) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

} // namespace algorithms::strings::rolling_hash

