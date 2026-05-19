#include "algorithms/strings/polynomial_hash/polynomial_hash.h"

#include <algorithm>
#include <stdexcept>

namespace algorithms::strings::polynomial_hash {

// ─── PolyHash ────────────────────────────────────────────────────────────────

PolyHash::PolyHash(std::string_view s, uint64_t base, uint64_t mod)
    : base_(base), mod_(mod), n_(s.size()),
      h_(s.size() + 1, 0), pw_(s.size() + 1, 1) {

    for (std::size_t i = 0; i < n_; ++i) {
        h_[i + 1]  = (h_[i] * base_ + static_cast<unsigned char>(s[i])) % mod_;
        pw_[i + 1] = pw_[i] * base_ % mod_;
    }
}

uint64_t PolyHash::query(std::size_t l, std::size_t r) const {
    // hash(s[l..r]) = h[r+1] - h[l] * B^(r-l+1)
    uint64_t len = r - l + 1;
    uint64_t val = (h_[r + 1] + mod_ - h_[l] * pw_[len] % mod_) % mod_;
    return val;
}

uint64_t PolyHash::full() const {
    return h_[n_];
}

// ─── DoubleHash ──────────────────────────────────────────────────────────────

DoubleHash::DoubleHash(std::string_view s,
                       uint64_t base1, uint64_t mod1,
                       uint64_t base2, uint64_t mod2)
    : h1_(s, base1, mod1), h2_(s, base2, mod2) {}

std::pair<uint64_t, uint64_t> DoubleHash::query(std::size_t l, std::size_t r) const {
    return {h1_.query(l, r), h2_.query(l, r)};
}

std::pair<uint64_t, uint64_t> DoubleHash::full() const {
    return {h1_.full(), h2_.full()};
}

// ─── Utilities ───────────────────────────────────────────────────────────────

bool substrings_equal(const DoubleHash& a, std::size_t la, std::size_t ra,
                      const DoubleHash& b, std::size_t lb, std::size_t rb) {
    if (ra - la != rb - lb) return false;
    return a.query(la, ra) == b.query(lb, rb);
}

std::size_t lcp(const DoubleHash& s, std::size_t ia, std::size_t ib) {
    const std::size_t n = s.size();
    if (ia == ib) return n - ia;

    std::size_t max_len = std::min(n - ia, n - ib);
    std::size_t lo = 0, hi = max_len;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo + 1) / 2;
        // Check if s[ia..ia+mid-1] == s[ib..ib+mid-1]
        if (s.query(ia, ia + mid - 1) == s.query(ib, ib + mid - 1)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

std::vector<std::size_t> find_all(std::string_view t, std::string_view p,
                                   uint64_t base1, uint64_t mod1,
                                   uint64_t base2, uint64_t mod2) {
    std::vector<std::size_t> result;
    const std::size_t n = t.size(), m = p.size();
    if (m == 0 || m > n) return result;

    DoubleHash ht(t, base1, mod1, base2, mod2);
    DoubleHash hp(p, base1, mod1, base2, mod2);
    auto target = hp.query(0, m - 1);

    for (std::size_t i = 0; i + m <= n; ++i) {
        if (ht.query(i, i + m - 1) == target) {
            result.push_back(i);
        }
    }
    return result;
}

} // namespace algorithms::strings::polynomial_hash

