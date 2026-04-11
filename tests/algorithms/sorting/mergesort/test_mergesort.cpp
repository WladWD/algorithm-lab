#include <algorithm>
#include <algorithms/sorting/mergesort/mergesort.h>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>

namespace ms = algorithms::sorting::mergesort;

TEST(MergeSort, HandlesEmptyAndSingleElement) {
    {
        std::vector<int> a;
        ms::mergesort(a.begin(), a.end());
        EXPECT_TRUE(a.empty());
    }
    {
        std::vector<int> a = {42};
        ms::mergesort(a.begin(), a.end());
        EXPECT_EQ(a, (std::vector<int>{42}));
    }
}

TEST(MergeSort, SortsAlreadySortedAndReverseSorted) {
    {
        std::vector<int> a = {1, 2, 3, 4, 5};
        ms::mergesort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    }
    {
        std::vector<int> a = {5, 4, 3, 2, 1};
        ms::mergesort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    }
}

TEST(MergeSort, SortsWithDuplicates) {
    std::vector<int> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    ms::mergesort(a.begin(), a.end());
    auto expected = a;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(a, expected);
}

TEST(MergeSort, SupportsCustomComparator) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    ms::mergesort(a.begin(), a.end(), std::greater<int>{});
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end(), std::greater<int>{}));
}

TEST(MergeSort, IsStable) {
    // Pairs of (value, original_index)
    std::vector<std::pair<int, int>> a = {{3, 0}, {1, 1}, {3, 2}, {2, 3}, {1, 4}};
    ms::mergesort(a.begin(), a.end(),
                  [](const auto& x, const auto& y) { return x.first < y.first; });

    // Check sorted by key
    for (std::size_t i = 1; i < a.size(); ++i) {
        EXPECT_LE(a[i - 1].first, a[i].first);
    }
    // Check stability: equal keys should preserve original order
    for (std::size_t i = 1; i < a.size(); ++i) {
        if (a[i - 1].first == a[i].first) {
            EXPECT_LT(a[i - 1].second, a[i].second)
                << "Stability violated for key=" << a[i].first;
        }
    }
}

TEST(MergeSort, RandomizedMatchesStdSort) {
    std::mt19937 rng(42u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);

    for (int it = 0; it < 200; ++it) {
        const int n = it % 50;
        std::vector<int32_t> a;
        a.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) a.push_back(dist(rng));

        auto b = a;
        ms::mergesort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

TEST(MergeSort, LargeArray) {
    std::mt19937 rng(123u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    std::vector<int32_t> a(10'000);
    for (auto& x : a) x = dist(rng);

    ms::mergesort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

