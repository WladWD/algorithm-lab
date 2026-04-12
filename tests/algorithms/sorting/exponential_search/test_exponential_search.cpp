#include <algorithms/sorting/exponential_search/exponential_search.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <vector>

namespace es = algorithms::sorting::exponential_search;

// ==================== Exact search ====================

TEST(ExponentialSearch, FindsExisting) {
    std::vector<int> a = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    for (int v : a) {
        auto it = es::exponential_search(a.begin(), a.end(), v);
        ASSERT_NE(it, a.end()) << "Should find " << v;
        EXPECT_EQ(*it, v);
    }
}

TEST(ExponentialSearch, ReturnsEndForMissing) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    EXPECT_EQ(es::exponential_search(a.begin(), a.end(), 5), a.end());
    EXPECT_EQ(es::exponential_search(a.begin(), a.end(), 15), a.end());
    EXPECT_EQ(es::exponential_search(a.begin(), a.end(), 55), a.end());
}

TEST(ExponentialSearch, Empty) {
    std::vector<int> a;
    EXPECT_EQ(es::exponential_search(a.begin(), a.end(), 42), a.end());
}

TEST(ExponentialSearch, SingleFound) {
    std::vector<int> a = {7};
    EXPECT_NE(es::exponential_search(a.begin(), a.end(), 7), a.end());
}

TEST(ExponentialSearch, SingleNotFound) {
    std::vector<int> a = {7};
    EXPECT_EQ(es::exponential_search(a.begin(), a.end(), 6), a.end());
}

TEST(ExponentialSearch, FirstElement) {
    std::vector<int> a(1000);
    std::iota(a.begin(), a.end(), 0);
    auto it = es::exponential_search(a.begin(), a.end(), 0);
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 0);
}

TEST(ExponentialSearch, LastElement) {
    std::vector<int> a(1000);
    std::iota(a.begin(), a.end(), 0);
    auto it = es::exponential_search(a.begin(), a.end(), 999);
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 999);
}

// ==================== Existence ====================

TEST(ExponentialSearch, ExistsTrue) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    EXPECT_TRUE(es::exponential_search_exists(a.begin(), a.end(), 5));
}

TEST(ExponentialSearch, ExistsFalse) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    EXPECT_FALSE(es::exponential_search_exists(a.begin(), a.end(), 4));
}

// ==================== Lower bound ====================

TEST(ExponentialSearch, LowerBoundExact) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    auto it = es::exponential_lower_bound(a.begin(), a.end(), 30);
    EXPECT_EQ(*it, 30);
}

TEST(ExponentialSearch, LowerBoundBetween) {
    std::vector<int> a = {10, 20, 30, 40, 50};
    auto it = es::exponential_lower_bound(a.begin(), a.end(), 25);
    EXPECT_EQ(*it, 30);
}

TEST(ExponentialSearch, LowerBoundBeforeFirst) {
    std::vector<int> a = {10, 20, 30};
    auto it = es::exponential_lower_bound(a.begin(), a.end(), 5);
    EXPECT_EQ(it, a.begin());
}

TEST(ExponentialSearch, LowerBoundAfterLast) {
    std::vector<int> a = {10, 20, 30};
    auto it = es::exponential_lower_bound(a.begin(), a.end(), 35);
    EXPECT_EQ(it, a.end());
}

TEST(ExponentialSearch, LowerBoundMatchesStd) {
    std::vector<int> a(200);
    std::iota(a.begin(), a.end(), 0);
    for (int v = -5; v <= 205; ++v) {
        auto ours = es::exponential_lower_bound(a.begin(), a.end(), v);
        auto stds = std::lower_bound(a.begin(), a.end(), v);
        EXPECT_EQ(ours - a.begin(), stds - a.begin()) << "v=" << v;
    }
}

// ==================== Randomized ====================

TEST(ExponentialSearch, RandomizedSearch) {
    std::mt19937 rng(54321u);
    std::uniform_int_distribution<int> dist(0, 100'000);
    for (int t = 0; t < 50; ++t) {
        const std::size_t n = 100 + t * 20;
        std::vector<int> a(n);
        for (auto& x : a) x = dist(rng);
        std::sort(a.begin(), a.end());
        for (std::size_t i = 0; i < n; i += std::max(std::size_t{1}, n / 30)) {
            auto it = es::exponential_search(a.begin(), a.end(), a[i]);
            ASSERT_NE(it, a.end());
            EXPECT_EQ(*it, a[i]);
        }
    }
}

TEST(ExponentialSearch, CustomComparator) {
    std::vector<int> a = {9, 7, 5, 3, 1}; // sorted descending
    auto it = es::exponential_search(a.begin(), a.end(), 5, std::greater<int>{});
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 5);
}

