#include "algorithms/graphs/bellman_ford/bellman_ford.h"

#include <cstddef>

namespace {

bool would_overflow_add_int32(int32_t a, int32_t b) {
    // We treat overflow as "cannot relax" in this project (defensive).
    // This keeps behavior defined even with extreme weights.
    if (b > 0 && a > std::numeric_limits<int32_t>::max() - b) {
        return true;
    }
    if (b < 0 && a < std::numeric_limits<int32_t>::min() - b) {
        return true;
    }
    return false;
}

} // namespace

namespace algorithms::graphs::bellman_ford {

BellmanFordResult bellman_ford(const Graph& g, int32_t s) {
    const int32_t n = static_cast<int32_t>(g.size());
    if (s < 0 || s >= n) {
        return {};
    }

    BellmanFordResult out;
    out.dist.assign(static_cast<size_t>(n), BellmanFordResult::INF);
    out.parent.assign(static_cast<size_t>(n), -1);
    out.has_negative_cycle = false;

    out.dist[static_cast<size_t>(s)] = 0;

    // Relax edges up to V-1 times.
    for (int32_t it = 0; it < n - 1; ++it) {
        bool any = false;

        for (int32_t u = 0; u < n; ++u) {
            const auto su = static_cast<size_t>(u);
            const int32_t du = out.dist[su];
            if (du == BellmanFordResult::INF) {
                continue;
            }

            for (const auto& e : g[su]) {
                const int32_t v = e.to;
                if (v < 0 || v >= n) {
                    continue; // ignore invalid edges defensively
                }

                if (would_overflow_add_int32(du, e.w)) {
                    continue;
                }
                const int32_t cand = du + e.w;

                const auto sv = static_cast<size_t>(v);
                if (out.dist[sv] > cand) {
                    out.dist[sv] = cand;
                    out.parent[sv] = u;
                    any = true;
                }
            }
        }

        if (!any) {
            break; // early stop
        }
    }

    // One more pass: if we can still relax a reachable vertex, there's a reachable negative cycle.
    for (int32_t u = 0; u < n; ++u) {
        const auto su = static_cast<size_t>(u);
        const int32_t du = out.dist[su];
        if (du == BellmanFordResult::INF) {
            continue;
        }

        for (const auto& e : g[su]) {
            const int32_t v = e.to;
            if (v < 0 || v >= n) {
                continue;
            }
            if (would_overflow_add_int32(du, e.w)) {
                continue;
            }
            const int32_t cand = du + e.w;
            const auto sv = static_cast<size_t>(v);
            if (out.dist[sv] > cand) {
                out.has_negative_cycle = true;
                return out;
            }
        }
    }

    return out;
}

} // namespace algorithms::graphs::bellman_ford

