#include "algorithms/graphs/floyd_warshall/floyd_warshall.h"

#include <cstddef>

namespace {

bool would_overflow_add_int32(int32_t a, int32_t b) {
    if (b > 0 && a > std::numeric_limits<int32_t>::max() - b) {
        return true;
    }
    if (b < 0 && a < std::numeric_limits<int32_t>::min() - b) {
        return true;
    }
    return false;
}

} // namespace

namespace algorithms::graphs::floyd_warshall {

FloydWarshallResult floyd_warshall(const std::vector<std::vector<int32_t>>& weights) {
    const int32_t n = static_cast<int32_t>(weights.size());

    // Keep semantics consistent with other modules:
    // - empty input => empty output
    // - invalid input => empty output
    if (n == 0) {
        return FloydWarshallResult{.dist = {}, .next = {}, .has_negative_cycle = false};
    }

    for (const auto& row : weights) {
        if (static_cast<int32_t>(row.size()) != n) {
            return {};
        }
    }

    FloydWarshallResult out;
    out.dist = weights;
    out.next.assign(static_cast<size_t>(n), std::vector<int32_t>(static_cast<size_t>(n), -1));

    // Initialize reconstruction table.
    for (int32_t i = 0; i < n; ++i) {
        const auto si = static_cast<size_t>(i);
        for (int32_t j = 0; j < n; ++j) {
            const auto sj = static_cast<size_t>(j);

            if (i == j) {
                // Be defensive: if diagonal is INF, treat it as 0.
                if (out.dist[si][sj] == FloydWarshallResult::INF) {
                    out.dist[si][sj] = 0;
                }
                out.next[si][sj] = i;
                continue;
            }

            if (out.dist[si][sj] != FloydWarshallResult::INF) {
                out.next[si][sj] = j;
            }
        }
    }

    for (int32_t k = 0; k < n; ++k) {
        const auto sk = static_cast<size_t>(k);
        for (int32_t i = 0; i < n; ++i) {
            const auto si = static_cast<size_t>(i);
            const int32_t dik = out.dist[si][sk];
            if (dik == FloydWarshallResult::INF) {
                continue;
            }

            for (int32_t j = 0; j < n; ++j) {
                const auto sj = static_cast<size_t>(j);
                const int32_t dkj = out.dist[sk][sj];
                if (dkj == FloydWarshallResult::INF) {
                    continue;
                }

                if (would_overflow_add_int32(dik, dkj)) {
                    continue;
                }
                const int32_t cand = dik + dkj;

                if (out.dist[si][sj] > cand) {
                    out.dist[si][sj] = cand;
                    out.next[si][sj] = out.next[si][sk];
                }
            }
        }
    }

    out.has_negative_cycle = false;
    for (int32_t v = 0; v < n; ++v) {
        if (out.dist[static_cast<size_t>(v)][static_cast<size_t>(v)] < 0) {
            out.has_negative_cycle = true;
            break;
        }
    }

    return out;
}

} // namespace algorithms::graphs::floyd_warshall

