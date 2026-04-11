#include <algorithms/sorting/interpolation_search/interpolation_search.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <vector>

namespace is = algorithms::sorting::interpolation_search;

// ==================== interpolation_search (exact) ====================

TEST(InterpolationSearch, FindsExistingElement) {
    std::vector<int> a = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    for (int v : a) {
        auto it = is::interpolation_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end()) << "Should find " << v;
        EXPECT_EQ(*it, v);
    }
}

TEST(InterpolationSearch, ReturnsEndForMissing) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 5), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 15), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 55), a.end());
}

TEST(InterpolationSearch, EmptyRange) {
    std::vector<int> a;
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 42), a.end());
}

TEST(InterpolationSearch, SingleElementFound) {
    std::vector<int> a = {7};
    auto it = is::interpolation_search(a.begin(), a.end(), 7);
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 7);
}

TEST(InterpolationSearch, SingleElementNotFound) {
    std::vector<int> a = {7};
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 6), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 8), a.end());
}

TEST(InterpolationSearch, TwoElements) {
    std::vector<int> a = {3, 9};
    EXPECT_NE(is::interpolation_search(a.begin(), a.end(), 3), a.end());
    EXPECT_NE(is::interpolation_search(a.begin(), a.end(), 9), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 5), a.end());
}

TEST(InterpolationSearch, AllIdentical) {
    std::vector<int> a(10, 5);
    auto it = is::interpolation_search(a.begin(), a.end(), 5);
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 4), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 6), a.end());
}

TEST(InterpolationSearch, UniformDistribution) {
    // Perfect uniform: 0, 1, 2, ..., 999
    std::vector<int> a(1000);
    std::iota(a.begin(), a.end(), 0);

    for (int v = 0; v < 1000; ++v) {
        auto it = is::interpolation_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end()) << "Should find " << v;
        EXPECT_EQ(*it, v);
    }
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), -1), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 1000), a.end());
}

TEST(InterpolationSearch, NonUniformDistribution) {
    // Exponential-ish distribution
    std::vector<int> a = {1, 2, 3, 4, 5, 100, 200, 300, 10000, 50000};
    for (int v : a) {
        auto it = is::interpolation_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end()) << "Should find " << v;
        EXPECT_EQ(*it, v);
    }
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 6), a.end());
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 150), a.end());
}

TEST(InterpolationSearch, LargeValues) {
    std::vector<int64_t> a = {-1'000'000'000LL, -500'000'000LL, 0, 500'000'000LL, 1'000'000'000LL};
    for (auto v : a) {
        auto it = is::interpolation_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end());
        EXPECT_EQ(*it, v);
    }
    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), int64_t{42}), a.end());
}

TEST(InterpolationSearch, FloatingPoint) {
    std::vector<double> a = {1.0, 2.5, 3.7, 5.0, 8.1, 10.0};
    auto it = is::interpolation_search(a.begin(), a.end(), 5.0);
    ASSERT_NE(it, a.end());
    EXPECT_DOUBLE_EQ(*it, 5.0);

    EXPECT_EQ(is::interpolation_search(a.begin(), a.end(), 4.0), a.end());
}

// ==================== interpolation_search_exists ====================

TEST(InterpolationSearch, ExistsTrue) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    EXPECT_TRUE(is::interpolation_search_exists(a.begin(), a.end(), 1));
    EXPECT_TRUE(is::interpolation_search_exists(a.begin(), a.end(), 5));
    EXPECT_TRUE(is::interpolation_search_exists(a.begin(), a.end(), 9));
}

TEST(InterpolationSearch, ExistsFalse) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    EXPECT_FALSE(is::interpolation_search_exists(a.begin(), a.end(), 0));
    EXPECT_FALSE(is::interpolation_search_exists(a.begin(), a.end(), 4));
    EXPECT_FALSE(is::interpolation_search_exists(a.begin(), a.end(), 10));
}

// ==================== interpolation_lower_bound ====================

TEST(InterpolationSearch, LowerBoundExact) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    auto it = is::interpolation_lower_bound(a.begin(), a.end(), 30);
    EXPECT_EQ(it - a.begin(), 2);
    EXPECT_EQ(*it, 30);
}

TEST(InterpolationSearch, LowerBoundBetweenValues) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    auto it = is::interpolation_lower_bound(a.begin(), a.end(), 25);
    EXPECT_EQ(it - a.begin(), 2);  // first element >= 25 is 30
    EXPECT_EQ(*it, 30);
}

TEST(InterpolationSearch, LowerBoundBeforeFirst) {
    std::vector<int> a = {10, 20, 30};
    auto it = is::interpolation_lower_bound(a.begin(), a.end(), 5);
    EXPECT_EQ(it, a.begin());
}

TEST(InterpolationSearch, LowerBoundAfterLast) {
    std::vector<int> a = {10, 20, 30};
    auto it = is::interpolation_lower_bound(a.begin(), a.end(), 35);
    EXPECT_EQ(it, a.end());
}

TEST(InterpolationSearch, LowerBoundEmpty) {
    std::vector<int> a;
    EXPECT_EQ(is::interpolation_lower_bound(a.begin(), a.end(), 5), a.end());
}

TEST(InterpolationSearch, LowerBoundDuplicates) {
    std::vector<int> a = {1, 3, 3, 3, 5, 7};
    auto it = is::interpolation_lower_bound(a.begin(), a.end(), 3);
    EXPECT_EQ(it - a.begin(), 1);  // first 3 is at index 1
}

TEST(InterpolationSearch, LowerBoundMatchesStd) {
    // Compare against std::lower_bound on uniform data
    std::vector<int> a(100);
    std::iota(a.begin(), a.end(), 0);

    for (int v = -1; v <= 101; ++v) {
        auto ours = is::interpolation_lower_bound(a.begin(), a.end(), v);
        auto stds = std::lower_bound(a.begin(), a.end(), v);
        EXPECT_EQ(ours - a.begin(), stds - a.begin()) << "for value " << v;
    }
}

TEST(InterpolationSearch, LowerBoundMatchesStdNonUniform) {
    std::vector<int> a = {1, 1, 2, 5, 5, 5, 10, 20, 20, 100};
    for (int v = 0; v <= 105; ++v) {
        auto ours = is::interpolation_lower_bound(a.begin(), a.end(), v);
        auto stds = std::lower_bound(a.begin(), a.end(), v);
        EXPECT_EQ(ours - a.begin(), stds - a.begin()) << "for value " << v;
    }
}

// ==================== Randomized stress test ====================

TEST(InterpolationSearch, RandomizedExactSearch) {
    std::mt19937 rng(54321u);
    std::uniform_int_distribution<int> dist(0, 100'000);

    for (int trial = 0; trial < 50; ++trial) {
        const int n = 100 + trial * 20;
        std::vector<int> a(static_cast<std::size_t>(n));
        for (auto& x : a) x = dist(rng);
        std::sort(a.begin(), a.end());

        // Search for every element that actually exists
        for (int i = 0; i < n; i += std::max(1, n / 50)) {
            auto it = is::interpolation_search(a.begin(), a.end(), a[static_cast<std::size_t>(i)]);
            ASSERT_NE(it, a.end());
            EXPECT_EQ(*it, a[static_cast<std::size_t>(i)]);
        }

        // Search for some values that likely don't exist
        for (int j = 0; j < 20; ++j) {
            int v = dist(rng);
            auto it = is::interpolation_search(a.begin(), a.end(), v);
            if (it != a.end()) {
                EXPECT_EQ(*it, v);
            } else {
                EXPECT_TRUE(!std::binary_search(a.begin(), a.end(), v));
            }
        }
    }
}

TEST(InterpolationSearch, RandomizedLowerBoundMatchesStd) {
    std::mt19937 rng(12345u);
    std::uniform_int_distribution<int> val_dist(0, 1000);

    for (int trial = 0; trial < 50; ++trial) {
        const std::size_t n = 50 + trial * 10;
        std::vector<int> a(n);
        for (auto& x : a) x = val_dist(rng);
        std::sort(a.begin(), a.end());

        for (int v = -5; v <= 1005; v += 7) {
            auto ours = is::interpolation_lower_bound(a.begin(), a.end(), v);
            auto stds = std::lower_bound(a.begin(), a.end(), v);
            EXPECT_EQ(ours - a.begin(), stds - a.begin())
                << "trial=" << trial << " v=" << v;
        }
    }
}

