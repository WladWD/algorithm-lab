#include <algebra/garner/garner.h>

namespace algebra::garner {
std::tuple<int64_t, int64_t, int64_t> ext_gcd(int64_t a, int64_t b) {
    if (b == 0)
        return {a >= 0 ? a : -a, a >= 0 ? 1 : -1, 0};
    int64_t x0 = 1, y0 = 0;
    int64_t x1 = 0, y1 = 1;
    while (b != 0) {
        int64_t q = a / b;
        int64_t r = a - q * b;
        int64_t x2 = x0 - q * x1;
        int64_t y2 = y0 - q * y1;
        a = b;
        b = r;
        x0 = x1;
        x1 = x2;
        y0 = y1;
        y1 = y2;
    }
    // now a == gcd, and x0,y0 are coefficients
    if (a < 0) {
        a = -a;
        x0 = -x0;
        y0 = -y0;
    }
    return {a, x0, y0};
}

uint64_t modinv(uint64_t a, uint64_t mod) {
    if (mod == 0)
        throw std::invalid_argument("modulus must be > 0");
    int64_t ai = static_cast<int64_t>(a % mod);
    int64_t mi = static_cast<int64_t>(mod);
    auto [g, x, y] = ext_gcd(ai, mi);
    if (g != 1)
        throw std::invalid_argument("modular inverse does not exist (gcd != 1)");
    int64_t inv = x % mi;
    if (inv < 0)
        inv += mi;
    return static_cast<uint64_t>(inv);
}

std::vector<uint64_t> garner_mixed(std::vector<uint64_t> a, const std::vector<uint64_t>& m) {
    size_t k = a.size();
    if (k != m.size())
        throw std::invalid_argument("garner_mixed: sizes of a and m must match");
    // trivial cases
    if (k == 0)
        return {};
    // Precompute triangular inverse table inv[j][i] for 0 <= j < i < k
    std::vector<std::vector<uint64_t>> inv(k, std::vector<uint64_t>(k, 0));
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < i; ++j) {
            uint64_t mj = m[j] % m[i];
            // modinv will throw if gcd != 1
            inv[j][i] = modinv(mj, m[i]);
        }
    }

    // c initially equals residues a; we'll transform in-place
    std::vector<uint64_t> c = std::move(a);

    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < i; ++j) {
            // t = (c[i] - c[j]) mod m[i]
            int64_t diff = static_cast<int64_t>(c[i]) - static_cast<int64_t>(c[j]);
            int64_t mi = static_cast<int64_t>(m[i]);
            diff %= mi;
            if (diff < 0)
                diff += mi;
            // multiply by inv[j][i]
            __int128 prod = static_cast<__int128>(inv[j][i]) * static_cast<__int128>(diff);
            uint64_t newci = static_cast<uint64_t>(prod % mi);
            c[i] = newci;
        }
    }
    return c;
}

uint64_t garner_mod(const std::vector<uint64_t>& a, const std::vector<uint64_t>& m, uint64_t MOD) {
    if (MOD == 0)
        throw std::invalid_argument("garner_mod: MOD must be > 0");
    std::vector<uint64_t> c = garner_mixed(a, m);
    __int128 result = 0;
    __int128 mult = 1;
    size_t k = c.size();
    for (size_t i = 0; i < k; ++i) {
        result += (__int128)c[i] * mult;
        result %= static_cast<__int128>(MOD);
        mult = (mult * static_cast<__int128>(m[i])) % static_cast<__int128>(MOD);
    }
    if (result < 0)
        result += static_cast<__int128>(MOD);
    return static_cast<uint64_t>(result);
}
} // namespace algebra::garner
