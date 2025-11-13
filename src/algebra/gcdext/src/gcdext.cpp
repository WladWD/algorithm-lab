#include "algebra/gcdext/gcdext.h"

namespace algebra::gcdext {
std::tuple<int64_t, int64_t, int64_t> extended_gcd(int64_t a, int64_t b) {
    int64_t a0 = std::llabs(a);
    int64_t b0 = std::llabs(b);

    int64_t old_r = a0, r = b0;
    int64_t old_s = 1, s = 0;
    int64_t old_t = 0, t = 1;

    while (r != 0) {
        int64_t q = old_r / r;
        int64_t tmp_r = old_r - q * r;
        old_r = r;
        r = tmp_r;
        int64_t tmp_s = old_s - q * s;
        old_s = s;
        s = tmp_s;
        int64_t tmp_t = old_t - q * t;
        old_t = t;
        t = tmp_t;
    }

    // Adjust signs to match original inputs (so that a * x + b * y == g)
    if (a < 0)
        old_s = -old_s;
    if (b < 0)
        old_t = -old_t;

    return {old_r, old_s, old_t};
}
} // namespace algebra::gcdext
