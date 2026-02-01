#include <gtest/gtest.h>

#include <algorithms/sorting/quicksort/quicksort.h>

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace qs = algorithms::sorting::quicksort;

TEST(QuickSort, HandlesEmptyAndSingleElement) {
    {
        std::vector<int> a;
        qs::quicksort(a.begin(), a.end());
        EXPECT_TRUE(a.empty());
    }
    {
        std::vector<int> a = {42};
        qs::quicksort(a.begin(), a.end());
        EXPECT_EQ(a, (std::vector<int>{42}));
    }
}

TEST(QuickSort, SortsAlreadySortedAndReverseSorted) {
    {
        std::vector<int> a = {1, 2, 3, 4, 5};
        qs::quicksort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    }
    {
        std::vector<int> a = {5, 4, 3, 2, 1};
        qs::quicksort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
    }
}

TEST(QuickSort, SortsWithDuplicates) {
    std::vector<int> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    qs::quicksort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));

    std::vector<int> b = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    std::sort(b.begin(), b.end());
    EXPECT_EQ(a, b);
}

TEST(QuickSort, SupportsCustomComparator) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    qs::quicksort(a.begin(), a.end(), std::greater<int>{});
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end(), std::greater<int>{}));
}

TEST(QuickSort, RandomizedMatchesStdSort) {
    std::mt19937 rng(123456u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);

    for (int it = 0; it < 200; ++it) {
        const int n = (it % 50);
        std::vector<int32_t> a;
        a.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) {
            a.push_back(dist(rng));
        }

        auto b = a;
        qs::quicksort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

