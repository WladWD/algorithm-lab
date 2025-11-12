#include <data_structures/range_query/mo/mo.h>

namespace ds::range_query::mo {
std::vector<int> mo_solve_distinct(std::vector<int> A, std::vector<Query> queries) {
    const size_t N = A.size();
    const size_t Q = queries.size();
    if (Q == 0) return {};

    // Coordinate-compress values in A to small non-negative integers [0..M-1]
    std::vector<int> vals = A;
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    for (size_t i = 0; i < N; ++i) {
        A[i] = static_cast<int>(std::lower_bound(vals.begin(), vals.end(), A[i]) - vals.begin());
    }
    const int M = static_cast<int>(vals.size());

    // Block size: standard sqrt decomposition. Can be tuned.
    int B = std::max<int>(1, static_cast<int>(std::sqrt((double)N)));

    // Sort queries by block(l) and r; use odd-even trick for better locality.
    std::sort(queries.begin(), queries.end(), [B](const Query &x, const Query &y) {
        int bx = x.l / B, by = y.l / B;
        if (bx != by) return bx < by;
        if (bx & 1) return x.r > y.r; // odd block -> descending r
        return x.r < y.r;             // even block -> ascending r
    });

    // Frequency array and current distinct count
    std::vector<int> freq(M);
    int distinct = 0;

    auto add = [&](int pos) {
        int v = A[pos];
        if (++freq[v] == 1) ++distinct;
    };
    auto remove = [&](int pos) {
        int v = A[pos];
        if (--freq[v] == 0) --distinct;
    };

    std::vector<int> ans(Q);
    int curL = 0, curR = -1; // empty window

    for (const auto &q : queries) {
        while (curL > q.l) add(--curL);
        while (curR < q.r) add(++curR);
        while (curL < q.l) remove(curL++);
        while (curR > q.r) remove(curR--);
        ans[q.idx] = distinct;
    }

    return ans;
}
} // namespace ds::range_query::mo