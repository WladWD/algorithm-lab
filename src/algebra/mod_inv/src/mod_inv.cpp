#include "algebra/mod_inv/mod_inv.h"

namespace algebra::mod_inv {
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = (a >= 0) ? 1 : -1;
        y = 0;
        return a >= 0 ? a : -a;
    }
    long long x1 = 0, y1 = 0;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

std::optional<long long> modinv_ext(long long a, long long m) {
    if (m <= 0) return std::nullopt;
    a %= m;
    if (a < 0) a += m;
    long long x = 0, y = 0;
    long long g = extgcd(a, m, x, y);
    if (g != 1) return std::nullopt; // inverse doesn't exist
    long long inv = (x % m + m) % m;
    return inv;
}

long long modpow(long long a, long long e, long long mod) {
    long long res = 1;
    a %= mod;
    if (a < 0) a += mod;
    while (e > 0) {
        if (e & 1) res = static_cast<long long>((__int128)res * a % mod);
        a = static_cast<long long>((__int128)a * a % mod);
        e >>= 1;
    }
    return res;
}

long long modinv_fermat(long long a, long long mod) {
    return modpow(a, mod - 2, mod);
}
} // namespace algebra::mod_inv
