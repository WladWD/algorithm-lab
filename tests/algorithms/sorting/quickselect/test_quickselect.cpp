#include <algorithms/sorting/quickselect/quickselect.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <vector>

namespace qs = algorithms::sorting::quickselect;

TEST(QuickSelect, SingleElement) {
    std::vector<int> a = {42};
    qs::quickselect(a.begin(), a.begin(), a.end());
    EXPECT_EQ(a[0], 42);
}

TEST(QuickSelect, TwoElementsMin) {
    std::vector<int> a = {5, 3};
    qs::quickselect(a.begin(), a.begin(), a.end());
    EXPECT_EQ(a[0], 3);
}

TEST(QuickSelect, TwoElementsMax) {
    std::vector<int> a = {5, 3};
    qs::quickselect(a.begin(), a.begin() + 1, a.end());
    EXPECT_EQ(a[1], 5);
}

TEST(QuickSelect, MedianElement) {
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());

    auto nth = a.begin() + 4;  // median (0-indexed)
    qs::quickselect(a.begin(), nth, a.end());
    EXPECT_EQ(*nth, sorted[4]);
}

TEST(QuickSelect, FirstElement) {
    std::vector<int> a = {7, 2, 9, 1, 5};
    qs::quickselect(a.begin(), a.begin(), a.end());
    EXPECT_EQ(a[0], 1);  // minimum

    // All elements before nth are <= nth
    for (auto it = a.begin(); it != a.begin(); ++it) {
        EXPECT_LE(*it, a[0]);
    }
}

TEST(QuickSelect, LastElement) {
    std::vector<int> a = {7, 2, 9, 1, 5};
    qs::quickselect(a.begin(), a.begin() + 4, a.end());
    EXPECT_EQ(a[4], 9);  // maximum
}

TEST(QuickSelect, WithDuplicates) {
    std::vector<int> a = {3, 3, 3, 1, 1, 2, 2};
    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());

    for (std::size_t k = 0; k < a.size(); ++k) {
        auto copy = std::vector<int>{3, 3, 3, 1, 1, 2, 2};
        qs::quickselect(copy.begin(), copy.begin() + static_cast<int>(k), copy.end());
        EXPECT_EQ(copy[k], sorted[k]) << "k=" << k;
    }
}

TEST(QuickSelect, PartialOrderingGuarantee) {
    std::vector<int> a = {9, 5, 7, 1, 3, 8, 2, 6, 4};
    auto nth = a.begin() + 4;
    qs::quickselect(a.begin(), nth, a.end());

    int pivot = *nth;
    for (auto it = a.begin(); it != nth; ++it) {
        EXPECT_LE(*it, pivot);
    }
    for (auto it = nth + 1; it != a.end(); ++it) {
        EXPECT_GE(*it, pivot);
    }
}

TEST(QuickSelect, CustomComparator) {
    std::vector<int> a = {3, 1, 4, 1, 5, 9};
    // Select 0th element with greater<> -> should be the maximum
    qs::quickselect(a.begin(), a.begin(), a.end(), std::greater<int>{});
    EXPECT_EQ(a[0], 9);
}

TEST(QuickSelect, RandomizedMatchesNthElement) {
    std::mt19937 rng(12345u);
    std::uniform_int_distribution<int32_t> dist(-1000, 1000);

    for (int it = 0; it < 200; ++it) {
        const int n = (it % 30) + 2;
        std::vector<int32_t> a;
        a.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) a.push_back(dist(rng));

        int k = it % n;
        auto b = a;
        auto sorted = a;
        std::sort(sorted.begin(), sorted.end());

        qs::quickselect(a.begin(), a.begin() + k, a.end());
        EXPECT_EQ(a[k], sorted[k]) << "n=" << n << " k=" << k;
    }
}

TEST(QuickSelect, LargeArray) {
    std::mt19937 rng(999u);
    std::uniform_int_distribution<int32_t> dist(0, 1'000'000);
    constexpr int N = 10'000;
    std::vector<int32_t> a(N);
    for (auto& x : a) x = dist(rng);

    auto sorted = a;
    std::sort(sorted.begin(), sorted.end());

    int k = N / 2;
    qs::quickselect(a.begin(), a.begin() + k, a.end());
    EXPECT_EQ(a[k], sorted[k]);
}

