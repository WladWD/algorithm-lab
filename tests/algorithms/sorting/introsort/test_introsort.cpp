#include <algorithms/sorting/introsort/introsort.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <vector>

namespace is = algorithms::sorting::introsort;

TEST(IntroSort, EmptyAndSingle) {
    std::vector<int> a;
    is::introsort(a.begin(), a.end());
    EXPECT_TRUE(a.empty());

    std::vector<int> b = {42};
    is::introsort(b.begin(), b.end());
    EXPECT_EQ(b, std::vector<int>{42});
}

TEST(IntroSort, AlreadySorted) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    is::introsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(IntroSort, ReverseSorted) {
    std::vector<int> a = {5, 4, 3, 2, 1};
    is::introsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(IntroSort, Duplicates) {
    std::vector<int> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    is::introsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{0, 1, 2, 2, 2, 3, 3, 3, 5}));
}

TEST(IntroSort, AllIdentical) {
    std::vector<int> a(100, 5);
    is::introsort(a.begin(), a.end());
    EXPECT_EQ(a, std::vector<int>(100, 5));
}

TEST(IntroSort, CustomComparator) {
    std::vector<int> a = {1, 5, 3, 2, 4};
    is::introsort(a.begin(), a.end(), std::greater<int>{});
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end(), std::greater<int>{}));
}

TEST(IntroSort, SmallArrays) {
    for (int n = 2; n <= 20; ++n) {
        std::vector<int> a(static_cast<std::size_t>(n));
        std::iota(a.begin(), a.end(), 0);
        std::reverse(a.begin(), a.end());
        is::introsort(a.begin(), a.end());
        EXPECT_TRUE(std::is_sorted(a.begin(), a.end())) << "n=" << n;
    }
}

TEST(IntroSort, AdversarialPipeOrgan) {
    // Pattern that's bad for naive quicksort
    std::vector<int> a(200);
    for (int i = 0; i < 100; ++i) a[static_cast<std::size_t>(i)] = i;
    for (int i = 100; i < 200; ++i) a[static_cast<std::size_t>(i)] = 199 - i;
    is::introsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(IntroSort, RandomizedMatchesStdSort) {
    std::mt19937 rng(555u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);
    for (int t = 0; t < 200; ++t) {
        const int n = t % 80;
        std::vector<int32_t> a(static_cast<std::size_t>(n));
        for (auto& x : a) x = dist(rng);
        auto b = a;
        is::introsort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

TEST(IntroSort, LargeArray) {
    std::mt19937 rng(666u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    std::vector<int32_t> a(10'000);
    for (auto& x : a) x = dist(rng);
    is::introsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

