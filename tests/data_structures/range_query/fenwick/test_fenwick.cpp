#include <data_structures/range_query/fenwick/fenwick.h>

#include <gtest/gtest.h>

#include <cstdint>
#include <random>
#include <vector>

namespace {
using ds::range_query::fenwick::FenwickTree;

TEST(FenwickTree, Empty) {
    FenwickTree ft;

    EXPECT_EQ(ft.size(), 0);
    EXPECT_EQ(ft.prefix_sum(-1), 0);
    EXPECT_EQ(ft.prefix_sum(0), 0);
    EXPECT_EQ(ft.range_sum(0, 0), 0);

    ft.add(0, 123); // no-op
    EXPECT_EQ(ft.prefix_sum(0), 0);
}

TEST(FenwickTree, SimpleBuildAndQueries) {
    FenwickTree ft(std::vector<int64_t>{1, 2, 3, 4});

    EXPECT_EQ(ft.size(), 4);
    EXPECT_EQ(ft.prefix_sum(0), 1);
    EXPECT_EQ(ft.prefix_sum(1), 3);
    EXPECT_EQ(ft.prefix_sum(3), 10);

    EXPECT_EQ(ft.range_sum(0, 3), 10);
    EXPECT_EQ(ft.range_sum(1, 2), 5);
    EXPECT_EQ(ft.range_sum(3, 3), 4);
}

TEST(FenwickTree, Updates) {
    FenwickTree ft(5);

    ft.add(2, 10);
    ft.add(0, 1);
    ft.add(4, -3);

    EXPECT_EQ(ft.prefix_sum(0), 1);
    EXPECT_EQ(ft.prefix_sum(2), 11);
    EXPECT_EQ(ft.range_sum(2, 4), 7);

    ft.add(2, -4);
    EXPECT_EQ(ft.range_sum(0, 4), 4);
}

TEST(FenwickTree, DefensiveClamping) {
    FenwickTree ft(std::vector<int64_t>{5, 6, 7});

    EXPECT_EQ(ft.prefix_sum(-10), 0);
    EXPECT_EQ(ft.prefix_sum(100), 18);

    EXPECT_EQ(ft.range_sum(-10, -1), 0);
    EXPECT_EQ(ft.range_sum(3, 10), 0);
    EXPECT_EQ(ft.range_sum(-5, 1), 11);
    EXPECT_EQ(ft.range_sum(1, 100), 13);
    EXPECT_EQ(ft.range_sum(2, 1), 0);

    ft.add(-1, 123); // no-op
    ft.add(3, 123);  // no-op
    EXPECT_EQ(ft.range_sum(0, 2), 18);
}

TEST(FenwickTree, RandomAgainstNaive) {
    constexpr int N = 200;
    constexpr int OPS = 2000;

    std::mt19937_64 rng(123456);
    std::uniform_int_distribution<int> idxDist(-50, N + 50);
    std::uniform_int_distribution<int64_t> deltaDist(-1000, 1000);

    std::vector<int64_t> a(N, 0);
    FenwickTree ft(N);

    for (int op = 0; op < OPS; ++op) {
        if ((rng() % 2ULL) == 0ULL) {
            const int idx = idxDist(rng);
            const int64_t delta = deltaDist(rng);
            if (0 <= idx && idx < N) {
                a[idx] += delta;
            }
            ft.add(idx, delta);
        } else {
            int l = idxDist(rng);
            int r = idxDist(rng);
            if (l > r) {
                std::swap(l, r);
            }

            // Naive with the same clamping semantics as FenwickTree::range_sum.
            int64_t expected = 0;
            if (!(r < 0 || l >= N)) {
                l = std::max(l, 0);
                r = std::min(r, N - 1);
                if (l <= r) {
                    for (int i = l; i <= r; ++i) {
                        expected += a[i];
                    }
                }
            }

            EXPECT_EQ(ft.range_sum(l, r), expected);
        }
    }
}

} // namespace

