#include <algorithm>
#include <data_structures/dsu/dsu.h>
#include <data_structures/dsu/dsu_rank.h>
#include <gtest/gtest.h>
#include <random>
#include <set>
#include <vector>

namespace {
using ds::dsu::DisjointSetUnion;
using ds::dsu::DisjointSetUnionRank;

TEST(DisjointSetUnion, Empty) {
    DisjointSetUnion dsu;

    EXPECT_EQ(dsu.size(), 0);
    EXPECT_EQ(dsu.components(), 0);
    EXPECT_EQ(dsu.find(0), -1);
    EXPECT_FALSE(dsu.same(0, 0));
    EXPECT_FALSE(dsu.unite(0, 1));
    EXPECT_EQ(dsu.component_size(0), 0);
}

TEST(DisjointSetUnion, AssignAndBasics) {
    DisjointSetUnion dsu(5);

    EXPECT_EQ(dsu.size(), 5);
    EXPECT_EQ(dsu.components(), 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(dsu.find(i), i);
        EXPECT_TRUE(dsu.same(i, i));
        EXPECT_EQ(dsu.component_size(i), 1);
    }

    EXPECT_FALSE(dsu.same(0, 1));
    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_TRUE(dsu.same(0, 1));
    EXPECT_EQ(dsu.components(), 4);

    // Re-union is a no-op.
    EXPECT_FALSE(dsu.unite(1, 0));
    EXPECT_EQ(dsu.components(), 4);

    // Size updated.
    EXPECT_EQ(dsu.component_size(0), 2);
    EXPECT_EQ(dsu.component_size(1), 2);
}

TEST(DisjointSetUnion, DefensiveInvalidInputs) {
    DisjointSetUnion dsu(3);

    EXPECT_EQ(dsu.find(-1), -1);
    EXPECT_EQ(dsu.find(3), -1);

    EXPECT_FALSE(dsu.same(-1, 0));
    EXPECT_FALSE(dsu.same(0, 3));
    EXPECT_FALSE(dsu.same(-1, 3));

    EXPECT_FALSE(dsu.unite(-1, 0));
    EXPECT_FALSE(dsu.unite(0, 3));

    EXPECT_EQ(dsu.component_size(-1), 0);
    EXPECT_EQ(dsu.component_size(3), 0);

    EXPECT_EQ(dsu.components(), 3);
}

static int naive_find(std::vector<int>& comp, int v) {
    // comp[v] holds component id, but ids can be arbitrary; normalize by compression.
    return comp[v];
}

static void naive_unite(std::vector<int>& comp, int a, int b) {
    const int ca = comp[a];
    const int cb = comp[b];
    if (ca == cb) {
        return;
    }
    for (auto& x : comp) {
        if (x == cb) {
            x = ca;
        }
    }
}

static int naive_components(const std::vector<int>& comp) {
    std::set<int> s(comp.begin(), comp.end());
    return static_cast<int>(s.size());
}

static int naive_component_size(const std::vector<int>& comp, int v) {
    const int c = comp[v];
    return static_cast<int>(std::count(comp.begin(), comp.end(), c));
}

TEST(DisjointSetUnion, RandomAgainstNaive) {
    constexpr int N = 200;
    constexpr int OPS = 5000;

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int> idxDist(-50, N + 50);

    DisjointSetUnion dsu(N);
    std::vector<int> comp(N);
    for (int i = 0; i < N; ++i) {
        comp[i] = i;
    }

    for (int op = 0; op < OPS; ++op) {
        const int type = static_cast<int>(rng() % 4ULL);

        if (type == 0) {
            // unite
            const int a = idxDist(rng);
            const int b = idxDist(rng);
            const bool ok = dsu.unite(a, b);
            if (0 <= a && a < N && 0 <= b && b < N) {
                const bool beforeSame = (naive_find(comp, a) == naive_find(comp, b));
                naive_unite(comp, a, b);
                EXPECT_EQ(ok, !beforeSame);
            } else {
                EXPECT_FALSE(ok);
            }
        } else if (type == 1) {
            // same
            const int a = idxDist(rng);
            const int b = idxDist(rng);
            const bool got = dsu.same(a, b);
            if (0 <= a && a < N && 0 <= b && b < N) {
                EXPECT_EQ(got, naive_find(comp, a) == naive_find(comp, b));
            } else {
                EXPECT_FALSE(got);
            }
        } else if (type == 2) {
            // component size
            const int v = idxDist(rng);
            const int got = dsu.component_size(v);
            if (0 <= v && v < N) {
                EXPECT_EQ(got, naive_component_size(comp, v));
            } else {
                EXPECT_EQ(got, 0);
            }
        } else {
            // components count
            EXPECT_EQ(dsu.components(), naive_components(comp));
        }
    }
}

TEST(DisjointSetUnionRank, Empty) {
    DisjointSetUnionRank dsu;

    EXPECT_EQ(dsu.size(), 0);
    EXPECT_EQ(dsu.components(), 0);
    EXPECT_EQ(dsu.find(0), -1);
    EXPECT_FALSE(dsu.same(0, 0));
    EXPECT_FALSE(dsu.unite(0, 1));
    EXPECT_EQ(dsu.component_size(0), 0);
}

TEST(DisjointSetUnionRank, AssignAndBasics) {
    DisjointSetUnionRank dsu(5);

    EXPECT_EQ(dsu.size(), 5);
    EXPECT_EQ(dsu.components(), 5);

    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_TRUE(dsu.unite(1, 2));
    EXPECT_TRUE(dsu.same(0, 2));
    EXPECT_EQ(dsu.component_size(0), 3);
    EXPECT_EQ(dsu.components(), 3);

    // Re-union is a no-op.
    EXPECT_FALSE(dsu.unite(0, 2));
}

TEST(DisjointSetUnionRank, DefensiveInvalidInputs) {
    DisjointSetUnionRank dsu(3);

    EXPECT_EQ(dsu.find(-1), -1);
    EXPECT_EQ(dsu.find(3), -1);
    EXPECT_FALSE(dsu.unite(-1, 0));
    EXPECT_FALSE(dsu.same(0, 3));
    EXPECT_EQ(dsu.component_size(3), 0);
    EXPECT_EQ(dsu.components(), 3);
}

TEST(DisjointSetUnionRank, RandomAgainstNaive) {
    constexpr int N = 200;
    constexpr int OPS = 5000;

    std::mt19937_64 rng(43);
    std::uniform_int_distribution<int> idxDist(-50, N + 50);

    DisjointSetUnionRank dsu(N);
    std::vector<int> comp(N);
    for (int i = 0; i < N; ++i) {
        comp[i] = i;
    }

    for (int op = 0; op < OPS; ++op) {
        const int type = static_cast<int>(rng() % 4ULL);

        if (type == 0) {
            const int a = idxDist(rng);
            const int b = idxDist(rng);
            const bool ok = dsu.unite(a, b);
            if (0 <= a && a < N && 0 <= b && b < N) {
                const bool beforeSame = (naive_find(comp, a) == naive_find(comp, b));
                naive_unite(comp, a, b);
                EXPECT_EQ(ok, !beforeSame);
            } else {
                EXPECT_FALSE(ok);
            }
        } else if (type == 1) {
            const int a = idxDist(rng);
            const int b = idxDist(rng);
            const bool got = dsu.same(a, b);
            if (0 <= a && a < N && 0 <= b && b < N) {
                EXPECT_EQ(got, naive_find(comp, a) == naive_find(comp, b));
            } else {
                EXPECT_FALSE(got);
            }
        } else if (type == 2) {
            const int v = idxDist(rng);
            const int got = dsu.component_size(v);
            if (0 <= v && v < N) {
                EXPECT_EQ(got, naive_component_size(comp, v));
            } else {
                EXPECT_EQ(got, 0);
            }
        } else {
            EXPECT_EQ(dsu.components(), naive_components(comp));
        }
    }
}

} // namespace
