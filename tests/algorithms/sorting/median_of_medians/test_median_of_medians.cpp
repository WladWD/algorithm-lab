#include <algorithms/sorting/median_of_medians/median_of_medians.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <vector>

namespace mom = algorithms::sorting::median_of_medians;

TEST(MedianOfMedians, SingleElement) {
    std::vector<int> a = {42};
    mom::select(a.begin(), a.begin(), a.end());
    EXPECT_EQ(a[0], 42);
}

TEST(MedianOfMedians, TwoElementsMin) {
    std::vector<int> a = {5, 3};
    mom::select(a.begin(), a.begin(), a.end());
    EXPECT_EQ(a[0], 3);
}

TEST(MedianOfMedians, TwoElementsMax) {
    std::vector<int> a = {5, 3};
    mom::select(a.begin(), a.begin() + 1, a.end());
    EXPECT_EQ(a[1], 5);
}

TEST(MedianOfMedians, Median) {
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());
    mom::select(a.begin(), a.begin() + 4, a.end());
    EXPECT_EQ(a[4], sorted[4]);
}

TEST(MedianOfMedians, FirstElement) {
    std::vector<int> a = {7, 2, 9, 1, 5};
    mom::select(a.begin(), a.begin(), a.end());
    EXPECT_EQ(a[0], 1);
}

TEST(MedianOfMedians, LastElement) {
    std::vector<int> a = {7, 2, 9, 1, 5};
    mom::select(a.begin(), a.begin() + 4, a.end());
    EXPECT_EQ(a[4], 9);
}

TEST(MedianOfMedians, Duplicates) {
    std::vector<int> a = {3, 3, 3, 1, 1, 2, 2};
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());
    for (std::size_t k = 0; k < a.size(); ++k) {
        auto copy = std::vector<int>{3, 3, 3, 1, 1, 2, 2};
        mom::select(copy.begin(), copy.begin() + static_cast<int>(k), copy.end());
        EXPECT_EQ(copy[k], sorted[k]) << "k=" << k;
    }
}

TEST(MedianOfMedians, AllIdentical) {
    std::vector<int> a(20, 7);
    mom::select(a.begin(), a.begin() + 10, a.end());
    EXPECT_EQ(a[10], 7);
}

TEST(MedianOfMedians, PartialOrdering) {
    std::vector<int> a = {9, 5, 7, 1, 3, 8, 2, 6, 4};
    mom::select(a.begin(), a.begin() + 4, a.end());
    int pivot = a[4];
    for (int i = 0; i < 4; ++i) EXPECT_LE(a[static_cast<std::size_t>(i)], pivot);
    for (int i = 5; i < 9; ++i) EXPECT_GE(a[static_cast<std::size_t>(i)], pivot);
}

TEST(MedianOfMedians, CustomComparator) {
    std::vector<int> a = {3, 1, 4, 1, 5, 9};
    mom::select(a.begin(), a.begin(), a.end(), std::greater<int>{});
    EXPECT_EQ(a[0], 9);
}

TEST(MedianOfMedians, FiveElements) {
    std::vector<int> a = {5, 3, 1, 4, 2};
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());
    for (int k = 0; k < 5; ++k) {
        auto copy = std::vector<int>{5, 3, 1, 4, 2};
        mom::select(copy.begin(), copy.begin() + k, copy.end());
        EXPECT_EQ(copy[k], sorted[static_cast<std::size_t>(k)]) << "k=" << k;
    }
}

TEST(MedianOfMedians, SixElements) {
    std::vector<int> a = {6, 5, 4, 3, 2, 1};
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());
    for (int k = 0; k < 6; ++k) {
        auto copy = std::vector<int>{6, 5, 4, 3, 2, 1};
        mom::select(copy.begin(), copy.begin() + k, copy.end());
        EXPECT_EQ(copy[k], sorted[static_cast<std::size_t>(k)]) << "k=" << k;
    }
}

TEST(MedianOfMedians, RandomizedMatchesSorted) {
    std::mt19937 rng(12345u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);
    for (int t = 0; t < 200; ++t) {
        const int n = (t % 40) + 2;
        std::vector<int32_t> a(static_cast<std::size_t>(n));
        for (auto& x : a) x = dist(rng);
        int k = t % n;
        auto sorted = a;
        std::sort(sorted.begin(), sorted.end());
        mom::select(a.begin(), a.begin() + k, a.end());
        EXPECT_EQ(a[static_cast<std::size_t>(k)], sorted[static_cast<std::size_t>(k)])
            << "n=" << n << " k=" << k;
    }
}

TEST(MedianOfMedians, LargeArray) {
    std::mt19937 rng(999u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    constexpr int N = 5'000;
    std::vector<int32_t> a(N);
    for (auto& x : a) x = dist(rng);
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());
    int k = N / 2;
    mom::select(a.begin(), a.begin() + k, a.end());
    EXPECT_EQ(a[static_cast<std::size_t>(k)], sorted[static_cast<std::size_t>(k)]);
}

