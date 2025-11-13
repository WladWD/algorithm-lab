#include "algebra/gcd/gcd.h"
namespace algebra::gcd {
int64_t gcd(int64_t a, int64_t b) {
    a = std::llabs(a);
    b = std::llabs(b);
    while (b != 0) {
        int64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}
} // namespace algebra::gcd