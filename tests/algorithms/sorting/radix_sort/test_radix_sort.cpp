#include <algorithms/sorting/radix_sort/radix_sort.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <vector>

namespace rs = algorithms::sorting::radix_sort;

// ---------- Unsigned radix sort ----------

TEST(RadixSort, HandlesEmpty) {
    std::vector<uint64_t> a;
    rs::radix_sort(a);
    EXPECT_TRUE(a.empty());
}

TEST(RadixSort, SingleElement) {
    std::vector<uint64_t> a = {42};
    rs::radix_sort(a);
    EXPECT_EQ(a, (std::vector<uint64_t>{42}));
}

TEST(RadixSort, AlreadySorted) {
    std::vector<uint64_t> a = {1, 2, 3, 4, 5};
    rs::radix_sort(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(RadixSort, ReverseSorted) {
    std::vector<uint64_t> a = {5, 4, 3, 2, 1};
    rs::radix_sort(a);
    EXPECT_EQ(a, (std::vector<uint64_t>{1, 2, 3, 4, 5}));
}

TEST(RadixSort, WithDuplicates) {
    std::vector<uint64_t> a = {170, 45, 75, 90, 802, 24, 2, 66};
    rs::radix_sort(a);
    EXPECT_EQ(a, (std::vector<uint64_t>{2, 24, 45, 66, 75, 90, 170, 802}));
}

TEST(RadixSort, AllZeros) {
    std::vector<uint64_t> a(10, 0);
    rs::radix_sort(a);
    EXPECT_EQ(a, std::vector<uint64_t>(10, 0));
}

TEST(RadixSort, LargeValues) {
    std::vector<uint64_t> a = {UINT64_MAX, 0, UINT64_MAX / 2, 1};
    rs::radix_sort(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(RadixSort, RandomizedMatchesStdSort) {
    std::mt19937_64 rng(777u);
    std::uniform_int_distribution<uint64_t> dist(0, 1'000'000);

    for (int it = 0; it < 100; ++it) {
        const std::size_t n = it % 50 + 1;
        std::vector<uint64_t> a(n);
        for (auto& x : a) x = dist(rng);

        auto b = a;
        rs::radix_sort(a);
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

// ---------- Signed radix sort ----------

TEST(RadixSortSigned, HandlesEmpty) {
    std::vector<int64_t> a;
    rs::radix_sort_signed(a);
    EXPECT_TRUE(a.empty());
}

TEST(RadixSortSigned, SingleElement) {
    std::vector<int64_t> a = {-5};
    rs::radix_sort_signed(a);
    EXPECT_EQ(a, (std::vector<int64_t>{-5}));
}

TEST(RadixSortSigned, MixedSignValues) {
    std::vector<int64_t> a = {-5, 3, -1, 0, 7, -3};
    rs::radix_sort_signed(a);
    EXPECT_EQ(a, (std::vector<int64_t>{-5, -3, -1, 0, 3, 7}));
}

TEST(RadixSortSigned, AllNegative) {
    std::vector<int64_t> a = {-1, -5, -3, -2, -4};
    rs::radix_sort_signed(a);
    EXPECT_EQ(a, (std::vector<int64_t>{-5, -4, -3, -2, -1}));
}

TEST(RadixSortSigned, RandomizedMatchesStdSort) {
    std::mt19937_64 rng(888u);
    std::uniform_int_distribution<int64_t> dist(-500'000, 500'000);

    for (int it = 0; it < 100; ++it) {
        const std::size_t n = it % 50 + 1;
        std::vector<int64_t> a(n);
        for (auto& x : a) x = dist(rng);

        auto b = a;
        rs::radix_sort_signed(a);
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

