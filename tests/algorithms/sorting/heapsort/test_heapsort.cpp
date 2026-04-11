#include <algorithm>
#include <algorithms/sorting/heapsort/heapsort.h>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <vector>

namespace hs = algorithms::sorting::heapsort;

TEST(HeapSort, HandlesEmptyAndSingleElement) {
    {
        std::vector<int> a;
        hs::heapsort(a.begin(), a.end());
        EXPECT_TRUE(a.empty());
    }
    {
        std::vector<int> a = {42};
        hs::heapsort(a.begin(), a.end());
        EXPECT_EQ(a, (std::vector<int>{42}));
    }
}

TEST(HeapSort, SortsAlreadySortedAndReverseSorted) {
    {
        std::vector<int> a = {1, 2, 3, 4, 5};
        hs::heapsort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    }
    {
        std::vector<int> a = {5, 4, 3, 2, 1};
        hs::heapsort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    }
}

TEST(HeapSort, SortsWithDuplicates) {
    std::vector<int> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    hs::heapsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));

    auto expected = std::vector<int>{0, 1, 2, 2, 2, 3, 3, 3, 5};
    EXPECT_EQ(a, expected);
}

TEST(HeapSort, SupportsCustomComparator) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    hs::heapsort(a.begin(), a.end(), std::greater<int>{});
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end(), std::greater<int>{}));
}

TEST(HeapSort, AllIdentical) {
    std::vector<int> a(50, 7);
    hs::heapsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    EXPECT_EQ(a[0], 7);
}

TEST(HeapSort, TwoElements) {
    std::vector<int> a = {2, 1};
    hs::heapsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{1, 2}));
}

TEST(HeapSort, RandomizedMatchesStdSort) {
    std::mt19937 rng(99u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);

    for (int it = 0; it < 200; ++it) {
        const int n = it % 50;
        std::vector<int32_t> a;
        a.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) a.push_back(dist(rng));

        auto b = a;
        hs::heapsort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

TEST(HeapSort, LargeArray) {
    std::mt19937 rng(456u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    std::vector<int32_t> a(10'000);
    for (auto& x : a) x = dist(rng);

    hs::heapsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

