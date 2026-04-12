#include <algorithms/sorting/shellsort/shellsort.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <vector>

namespace ss = algorithms::sorting::shellsort;

TEST(ShellSort, EmptyAndSingle) {
    std::vector<int> a;
    ss::shellsort(a.begin(), a.end());
    EXPECT_TRUE(a.empty());

    std::vector<int> b = {42};
    ss::shellsort(b.begin(), b.end());
    EXPECT_EQ(b, std::vector<int>{42});
}

TEST(ShellSort, AlreadySorted) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    ss::shellsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(ShellSort, ReverseSorted) {
    std::vector<int> a = {5, 4, 3, 2, 1};
    ss::shellsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(ShellSort, Duplicates) {
    std::vector<int> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    ss::shellsort(a.begin(), a.end());
    EXPECT_EQ(a, (std::vector<int>{0, 1, 2, 2, 2, 3, 3, 3, 5}));
}

TEST(ShellSort, AllIdentical) {
    std::vector<int> a(50, 7);
    ss::shellsort(a.begin(), a.end());
    EXPECT_EQ(a, std::vector<int>(50, 7));
}

TEST(ShellSort, CustomComparator) {
    std::vector<int> a = {1, 2, 3, 4, 5};
    ss::shellsort(a.begin(), a.end(), std::greater<int>{});
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end(), std::greater<int>{}));
}

TEST(ShellSort, RandomizedMatchesStdSort) {
    std::mt19937 rng(111u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);
    for (int t = 0; t < 200; ++t) {
        const int n = t % 60;
        std::vector<int32_t> a(static_cast<std::size_t>(n));
        for (auto& x : a) x = dist(rng);
        auto b = a;
        ss::shellsort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

TEST(ShellSort, LargeArray) {
    std::mt19937 rng(222u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    std::vector<int32_t> a(10'000);
    for (auto& x : a) x = dist(rng);
    ss::shellsort(a.begin(), a.end());
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

