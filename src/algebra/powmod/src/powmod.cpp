#include "algebra/powmod/powmod.h"

namespace algebra::powmod {
uint64_t mul_mod_u128(uint64_t a, uint64_t b, uint64_t mod) {
    if (mod == 0) throw std::invalid_argument("mul_mod: modulus must be > 0");
    return static_cast<uint64_t>((__int128)a * b % mod);
}

uint64_t powmod_u128(uint64_t base, uint64_t exp, uint64_t mod) {
    if (mod == 1) return 0;
    base %= mod;
    uint64_t res = 1 % mod;
    while (exp > 0) {
        if (exp & 1) res = mul_mod_u128(res, base, mod);
        base = mul_mod_u128(base, base, mod);
        exp >>= 1;
    }
    return res;
}

uint64_t powmod_bigexp(uint64_t base, const std::string &exp_dec, uint64_t mod) {
    if (mod == 1) return 0;
    base %= mod;
    uint64_t res = 1 % mod;
    for (char ch : exp_dec) {
        if (ch < '0' || ch > '9') throw std::invalid_argument("exp_dec must be decimal digits");
        int d = ch - '0';
        res = powmod_u128(res, 10, mod);
        if (d) res = mul_mod_u128(res, powmod_u128(base, d, mod), mod);
    }
    return res;
}
} // namespace algebra::powmod
