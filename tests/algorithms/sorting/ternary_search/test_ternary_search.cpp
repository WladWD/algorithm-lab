#include <algorithms/sorting/ternary_search/ternary_search.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <vector>

namespace ts = algorithms::sorting::ternary_search;

// ==================== Continuous find_min ====================

TEST(TernarySearch, FindMinQuadratic) {
    // f(x) = (x - 3)^2, minimum at x = 3
    auto f = [](double x) { return (x - 3.0) * (x - 3.0); };
    double x = ts::find_min(f, 0.0, 10.0);
    EXPECT_NEAR(x, 3.0, 1e-6);
}

TEST(TernarySearch, FindMinQuadraticNegativeRange) {
    // f(x) = (x + 5)^2, minimum at x = -5
    auto f = [](double x) { return (x + 5.0) * (x + 5.0); };
    double x = ts::find_min(f, -100.0, 100.0);
    EXPECT_NEAR(x, -5.0, 1e-6);
}

TEST(TernarySearch, FindMinAbsValue) {
    // f(x) = |x - 7.5|, minimum at x = 7.5
    auto f = [](double x) { return std::abs(x - 7.5); };
    double x = ts::find_min(f, 0.0, 15.0);
    EXPECT_NEAR(x, 7.5, 1e-6);
}

TEST(TernarySearch, FindMinAtLeftEndpoint) {
    // f(x) = x, monotonically increasing → minimum at lo = 0
    auto f = [](double x) { return x; };
    double x = ts::find_min(f, 0.0, 10.0);
    EXPECT_NEAR(x, 0.0, 1e-6);
}

TEST(TernarySearch, FindMinAtRightEndpoint) {
    // f(x) = -x, monotonically decreasing → minimum at hi = 10
    auto f = [](double x) { return -x; };
    double x = ts::find_min(f, 0.0, 10.0);
    EXPECT_NEAR(x, 10.0, 1e-6);
}

TEST(TernarySearch, FindMinCustomEpsilon) {
    auto f = [](double x) { return (x - 1.0) * (x - 1.0); };
    double x = ts::find_min(f, -10.0, 10.0, 1e-12);
    EXPECT_NEAR(x, 1.0, 1e-9);
}

TEST(TernarySearch, FindMinCosine) {
    // cos(x) has a minimum at x = π on [0, 2π]
    auto f = [](double x) { return std::cos(x); };
    double x = ts::find_min(f, 0.0, 2.0 * M_PI);
    EXPECT_NEAR(x, M_PI, 1e-6);
}

// ==================== Continuous find_max ====================

TEST(TernarySearch, FindMaxQuadratic) {
    // f(x) = -(x - 5)^2 + 25, maximum at x = 5
    auto f = [](double x) { return -(x - 5.0) * (x - 5.0) + 25.0; };
    double x = ts::find_max(f, 0.0, 10.0);
    EXPECT_NEAR(x, 5.0, 1e-6);
}

TEST(TernarySearch, FindMaxSine) {
    // sin(x) has a maximum at x = π/2 on [0, π]
    auto f = [](double x) { return std::sin(x); };
    double x = ts::find_max(f, 0.0, M_PI);
    EXPECT_NEAR(x, M_PI / 2.0, 1e-6);
}

TEST(TernarySearch, FindMaxAtLeftEndpoint) {
    // f(x) = -x, monotonically decreasing → maximum at lo = 0
    auto f = [](double x) { return -x; };
    double x = ts::find_max(f, 0.0, 10.0);
    EXPECT_NEAR(x, 0.0, 1e-6);
}

TEST(TernarySearch, FindMaxAtRightEndpoint) {
    // f(x) = x, monotonically increasing → maximum at hi = 10
    auto f = [](double x) { return x; };
    double x = ts::find_max(f, 0.0, 10.0);
    EXPECT_NEAR(x, 10.0, 1e-6);
}

// ==================== Discrete find_min_discrete ====================

TEST(TernarySearch, DiscreteMinAbsValue) {
    // f(i) = |i - 42|, minimum at i = 42
    auto f = [](int64_t i) { return std::abs(i - 42); };
    EXPECT_EQ(ts::find_min_discrete(f, 0, 100), 42);
}

TEST(TernarySearch, DiscreteMinQuadratic) {
    // f(i) = (i - 73)^2
    auto f = [](int64_t i) { return (i - 73) * (i - 73); };
    EXPECT_EQ(ts::find_min_discrete(f, 0, 200), 73);
}

TEST(TernarySearch, DiscreteMinSmallRange) {
    auto f = [](int64_t i) { return (i - 2) * (i - 2); };
    EXPECT_EQ(ts::find_min_discrete(f, 0, 4), 2);
    EXPECT_EQ(ts::find_min_discrete(f, 0, 2), 2);
    EXPECT_EQ(ts::find_min_discrete(f, 2, 2), 2);
}

TEST(TernarySearch, DiscreteMinSingleElement) {
    auto f = [](int64_t i) { return i * i; };
    EXPECT_EQ(ts::find_min_discrete(f, 5, 5), 5);
}

TEST(TernarySearch, DiscreteMinTwoElements) {
    auto f = [](int64_t i) { return std::abs(i - 10); };
    EXPECT_EQ(ts::find_min_discrete(f, 9, 10), 10);
    EXPECT_EQ(ts::find_min_discrete(f, 10, 11), 10);
}

TEST(TernarySearch, DiscreteMinAtLeftEndpoint) {
    // f(i) = i, minimum at lo = 0
    auto f = [](int64_t i) { return i; };
    EXPECT_EQ(ts::find_min_discrete(f, 0, 1000), 0);
}

TEST(TernarySearch, DiscreteMinAtRightEndpoint) {
    // f(i) = -i, minimum at hi = 1000
    auto f = [](int64_t i) { return -i; };
    EXPECT_EQ(ts::find_min_discrete(f, 0, 1000), 1000);
}

TEST(TernarySearch, DiscreteMinLargeRange) {
    // f(i) = (i - 500000)^2, minimum at i = 500000
    auto f = [](int64_t i) { return (i - 500000LL) * (i - 500000LL); };
    EXPECT_EQ(ts::find_min_discrete(f, 0, 1'000'000), 500000);
}

// ==================== Discrete find_max_discrete ====================

TEST(TernarySearch, DiscreteMaxQuadratic) {
    // f(i) = -(i - 50)^2 + 2500, maximum at i = 50
    auto f = [](int64_t i) { return -(i - 50) * (i - 50) + 2500; };
    EXPECT_EQ(ts::find_max_discrete(f, 0, 100), 50);
}

TEST(TernarySearch, DiscreteMaxSmallRange) {
    auto f = [](int64_t i) { return -(i - 3) * (i - 3); };
    EXPECT_EQ(ts::find_max_discrete(f, 0, 5), 3);
}

TEST(TernarySearch, DiscreteMaxAtLeftEndpoint) {
    // f(i) = -i, maximum at lo = 0
    auto f = [](int64_t i) { return -i; };
    EXPECT_EQ(ts::find_max_discrete(f, 0, 100), 0);
}

// ==================== Array ternary_search ====================

TEST(TernarySearch, ArrayFindsExistingElement) {
    std::vector<int> a = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    for (int v : a) {
        auto it = ts::ternary_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end()) << "Should find " << v;
        EXPECT_EQ(*it, v);
    }
}

TEST(TernarySearch, ArrayReturnsEndForMissing) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 5), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 15), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 55), a.end());
}

TEST(TernarySearch, ArrayEmpty) {
    std::vector<int> a;
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 42), a.end());
}

TEST(TernarySearch, ArraySingleElementFound) {
    std::vector<int> a = {7};
    auto it = ts::ternary_search(a.begin(), a.end(), 7);
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 7);
}

TEST(TernarySearch, ArraySingleElementNotFound) {
    std::vector<int> a = {7};
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 6), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 8), a.end());
}

TEST(TernarySearch, ArrayTwoElements) {
    std::vector<int> a = {3, 9};
    EXPECT_NE(ts::ternary_search(a.begin(), a.end(), 3), a.end());
    EXPECT_NE(ts::ternary_search(a.begin(), a.end(), 9), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 5), a.end());
}

TEST(TernarySearch, ArrayThreeElements) {
    std::vector<int> a = {1, 5, 9};
    EXPECT_NE(ts::ternary_search(a.begin(), a.end(), 1), a.end());
    EXPECT_NE(ts::ternary_search(a.begin(), a.end(), 5), a.end());
    EXPECT_NE(ts::ternary_search(a.begin(), a.end(), 9), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 3), a.end());
}

TEST(TernarySearch, ArrayUniform) {
    std::vector<int> a(1000);
    std::iota(a.begin(), a.end(), 0);

    for (int v = 0; v < 1000; ++v) {
        auto it = ts::ternary_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end()) << "Should find " << v;
        EXPECT_EQ(*it, v);
    }
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), -1), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 1000), a.end());
}

TEST(TernarySearch, ArrayWithDuplicates) {
    std::vector<int> a = {1, 1, 3, 3, 3, 5, 5, 7};
    auto it = ts::ternary_search(a.begin(), a.end(), 3);
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 3);

    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 2), a.end());
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), 4), a.end());
}

TEST(TernarySearch, ArrayLargeValues) {
    std::vector<int64_t> a = {-1'000'000'000LL, -500'000'000LL, 0, 500'000'000LL, 1'000'000'000LL};
    for (auto v : a) {
        auto it = ts::ternary_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end());
        EXPECT_EQ(*it, v);
    }
    EXPECT_EQ(ts::ternary_search(a.begin(), a.end(), int64_t{42}), a.end());
}

// ==================== Randomized stress tests ====================

TEST(TernarySearch, RandomizedArraySearch) {
    std::mt19937 rng(98765u);
    std::uniform_int_distribution<int> dist(0, 100'000);

    for (int trial = 0; trial < 50; ++trial) {
        const std::size_t n = 50 + trial * 20;
        std::vector<int> a(n);
        for (auto& x : a) x = dist(rng);
        std::sort(a.begin(), a.end());

        // Search for existing elements
        for (std::size_t i = 0; i < n; i += std::max(std::size_t{1}, n / 50)) {
            auto it = ts::ternary_search(a.begin(), a.end(), a[i]);
            ASSERT_NE(it, a.end());
            EXPECT_EQ(*it, a[i]);
        }

        // Search for random (possibly non-existent) values
        for (int j = 0; j < 20; ++j) {
            int v = dist(rng);
            auto it = ts::ternary_search(a.begin(), a.end(), v);
            if (it != a.end()) {
                EXPECT_EQ(*it, v);
            } else {
                EXPECT_FALSE(std::binary_search(a.begin(), a.end(), v));
            }
        }
    }
}

TEST(TernarySearch, RandomizedDiscreteMin) {
    std::mt19937 rng(11111u);
    std::uniform_int_distribution<int64_t> center_dist(0, 10000);

    for (int trial = 0; trial < 100; ++trial) {
        int64_t center = center_dist(rng);
        int64_t lo = center - 500;
        int64_t hi = center + 500;
        auto f = [center](int64_t i) { return (i - center) * (i - center); };
        EXPECT_EQ(ts::find_min_discrete(f, lo, hi), center)
            << "center=" << center;
    }
}

