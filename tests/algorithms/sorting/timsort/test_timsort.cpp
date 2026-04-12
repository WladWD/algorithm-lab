#include <algorithms/sorting/timsort/timsort.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <vector>

namespace ts = algorithms::sorting::timsort;

TEST(TimSort, EmptyAndSingle) {
    std::vector<int> a;
    ts::timsort(a.begin(), a.end());
    EXPECT_TRUE(a.empty());

    std::vector<int> b = {42};
    ts::timsort(b.begin(), b.end());
    EXPECT_EQ(b, std::vector<int>{42});
}

TEST(TimSort, AlreadySorted) {
    std::vector<int> a(100);
    std::iota(a.begin(), a.end(), 0);
    ts::timsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(TimSort, ReverseSorted) {
    std::vector<int> a = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    ts::timsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
}

TEST(TimSort, Duplicates) {
    std::vector<int> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    ts::timsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{0, 1, 2, 2, 2, 3, 3, 3, 5}));
}

TEST(TimSort, AllIdentical) {
    std::vector<int> a(100, 5);
    ts::timsort(a.begin(), a.end());
    EXPECT_EQ(a, std::vector<int>(100, 5));
}

TEST(TimSort, CustomComparator) {
    std::vector<int> a = {1, 5, 3, 2, 4};
    ts::timsort(a.begin(), a.end(), std::greater<int>{});
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end(), std::greater<int>{}));
}

TEST(TimSort, IsStable) {
    std::vector<std::pair<int, int>> a = {{3, 0}, {1, 1}, {3, 2}, {2, 3}, {1, 4}, {2, 5}};
    ts::timsort(a.begin(), a.end(),
                [](const auto& x, const auto& y) { return x.first < y.first; });
    for (std::size_t i = 1; i < a.size(); ++i) {
        EXPECT_LE(a[i - 1].first, a[i].first);
        if (a[i - 1].first == a[i].first) {
            EXPECT_LT(a[i - 1].second, a[i].second) << "Stability violated";
        }
    }
}

TEST(TimSort, NaturalRuns) {
    // Already has natural ascending and descending runs
    std::vector<int> a = {1, 3, 5, 7, 9, 8, 6, 4, 2, 0, 10, 20, 30};
    ts::timsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(TimSort, SmallArray) {
    for (int n = 2; n <= 35; ++n) {
        std::vector<int> a(static_cast<std::size_t>(n));
        std::iota(a.begin(), a.end(), 0);
        std::reverse(a.begin(), a.end());
        ts::timsort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end())) << "n=" << n;
    }
}

TEST(TimSort, RandomizedMatchesStdSort) {
    std::mt19937 rng(333u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);
    for (int t = 0; t < 200; ++t) {
        const int n = t % 80;
        std::vector<int32_t> a(static_cast<std::size_t>(n));
        for (auto& x : a) x = dist(rng);
        auto b = a;
        ts::timsort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

TEST(TimSort, LargeArray) {
    std::mt19937 rng(444u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    std::vector<int32_t> a(10'000);
    for (auto& x : a) x = dist(rng);
    ts::timsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

