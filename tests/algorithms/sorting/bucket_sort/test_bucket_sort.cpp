#include <algorithms/sorting/bucket_sort/bucket_sort.h>

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <vector>

namespace bs = algorithms::sorting::bucket_sort;

// ==================== Double variant ====================

TEST(BucketSort, EmptyDouble) {
    std::vector<double> a;
    bs::bucket_sort(a);
    EXPECT_TRUE(a.empty());
}

TEST(BucketSort, SingleDouble) {
    std::vector<double> a = {0.5};
    bs::bucket_sort(a);
    EXPECT_EQ(a.size(), 1u);
    EXPECT_DOUBLE_EQ(a[0], 0.5);
}

TEST(BucketSort, UniformDoubles) {
    std::vector<double> a = {0.42, 0.32, 0.33, 0.52, 0.37, 0.47, 0.51};
    bs::bucket_sort(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(BucketSort, ReverseSortedDoubles) {
    std::vector<double> a = {0.9, 0.7, 0.5, 0.3, 0.1};
    bs::bucket_sort(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(BucketSort, AllIdenticalDoubles) {
    std::vector<double> a(20, 0.5);
    bs::bucket_sort(a);
    EXPECT_EQ(a, std::vector<double>(20, 0.5));
}

TEST(BucketSort, LargeRangeDoubles) {
    std::vector<double> a = {-100.5, 200.3, 0.0, 50.1, -50.2};
    bs::bucket_sort(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(BucketSort, CustomBucketCount) {
    std::vector<double> a = {0.9, 0.1, 0.5, 0.3, 0.7};
    bs::bucket_sort(a, 3);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(BucketSort, RandomizedDoubles) {
    std::mt19937 rng(777u);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (int t = 0; t < 50; ++t) {
        const std::size_t n = 50 + t * 10;
        std::vector<double> a(n);
        for (auto& x : a) x = dist(rng);
        auto b = a;
        bs::bucket_sort(a);
        std::sort(b.begin(), b.end());
        ASSERT_EQ(a.size(), b.size());
        for (std::size_t i = 0; i < a.size(); ++i) {
            EXPECT_DOUBLE_EQ(a[i], b[i]) << "index=" << i;
        }
    }
}

// ==================== Integer variant ====================

TEST(BucketSort, EmptyInt) {
    std::vector<int64_t> a;
    bs::bucket_sort_int(a);
    EXPECT_TRUE(a.empty());
}

TEST(BucketSort, SingleInt) {
    std::vector<int64_t> a = {42};
    bs::bucket_sort_int(a);
    EXPECT_EQ(a, std::vector<int64_t>{42});
}

TEST(BucketSort, BasicInts) {
    std::vector<int64_t> a = {29, 25, 3, 49, 9, 37, 21, 43};
    bs::bucket_sort_int(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(BucketSort, NegativeInts) {
    std::vector<int64_t> a = {-5, 3, -1, 0, 7, -3};
    bs::bucket_sort_int(a);
    EXPECT_EQ(a, (std::vector<int64_t>{-5, -3, -1, 0, 3, 7}));
}

TEST(BucketSort, AllIdenticalInts) {
    std::vector<int64_t> a(20, 5);
    bs::bucket_sort_int(a);
    EXPECT_EQ(a, std::vector<int64_t>(20, 5));
}

TEST(BucketSort, RandomizedInts) {
    std::mt19937 rng(888u);
    std::uniform_int_distribution<int64_t> dist(-10000, 10000);
    for (int t = 0; t < 50; ++t) {
        const std::size_t n = 50 + t * 10;
        std::vector<int64_t> a(n);
        for (auto& x : a) x = dist(rng);
        auto b = a;
        bs::bucket_sort_int(a);
        std::sort(b.begin(), b.end());
        EXPECT_EQ(a, b);
    }
}

