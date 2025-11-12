#include <data_structures/range_query/sqrt_decomposition/sqrt_decomposition.h>

namespace ds::range_query::sqrt_decomposition {
SqrtDecomposition::SqrtDecomposition(const std::vector<int64_t>& arr) {
    A = arr;
    N = static_cast<int64_t>(A.size());
    B = std::max((int64_t)1ll, static_cast<int64_t>(std::sqrt(static_cast<double>(N))));
    blockCount = (N + B - 1) / B;
    blockSum.assign(std::max((int64_t)1, blockCount), 0);
    for (int i = 0; i < N; ++i) {
        blockSum[i / B] += A[i];
    }
}

void SqrtDecomposition::update(int idx, int64_t val) {
    if (idx < 0 || idx >= N) {
        return;
    }
    int64_t b = idx / B;
    blockSum[b] += (val - A[idx]);
    A[idx] = val;
}

int64_t SqrtDecomposition::query(int64_t l, int64_t r) const {
    if (N == 0)
        return 0;
    if (l < 0)
        l = 0;
    if (r >= N)
        r = N - 1;
    if (l > r)
        return 0;
    int64_t bl = l / B;
    int64_t br = r / B;
    int64_t res = 0;
    if (bl == br) {
        for (int i = l; i <= r; ++i)
            res += A[i];
        return res;
    }
    int64_t endL = (bl + 1) * B - 1;
    for (int i = l; i <= endL; ++i) {
        res += A[i];
    }
    for (int b = bl + 1; b <= br - 1; ++b) {
        res += blockSum[b];
    }
    int64_t startR = br * B;
    for (int i = startR; i <= r; ++i) {
        res += A[i];
    }
    return res;
}

int SqrtDecomposition::size() const {
    return N;
}
} // namespace ds::range_query::sqrt_decomposition
