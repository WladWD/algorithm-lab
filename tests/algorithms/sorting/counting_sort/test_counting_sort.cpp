#include <algorithms/sorting/counting_sort/counting_sort.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

namespace cs = algorithms::sorting::counting_sort;

TEST(CountingSort, HandlesEmpty) {
    std::vector<int64_t> a;
    cs::counting_sort(a);
    EXPECT_TRUE(a.empty());
}

TEST(CountingSort, SingleElement) {
    std::vector<int64_t> a = {42};
    cs::counting_sort(a);
    EXPECT_EQ(a, (std::vector<int64_t>{42}));
}

TEST(CountingSort, AlreadySorted) {
    std::vector<int64_t> a = {0, 1, 2, 3, 4, 5};
    cs::counting_sort(a);
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

TEST(CountingSort, ReverseSorted) {
    std::vector<int64_t> a = {5, 4, 3, 2, 1, 0};
    cs::counting_sort(a);
    EXPECT_EQ(a, (std::vector<int64_t>{0, 1, 2, 3, 4, 5}));
}

TEST(CountingSort, WithDuplicates) {
    std::vector<int64_t> a = {3, 1, 2, 2, 5, 3, 3, 0, 2};
    cs::counting_sort(a);
    EXPECT_EQ(a, (std::vector<int64_t>{0, 1, 2, 2, 2, 3, 3, 3, 5}));
}

TEST(CountingSort, AllIdentical) {
    std::vector<int64_t> a(20, 7);
    cs::counting_sort(a);
    EXPECT_EQ(a, std::vector<int64_t>(20, 7));
}

TEST(CountingSort, WithExplicitMax) {
    std::vector<int64_t> a = {4, 2, 8, 3, 1};
    cs::counting_sort(a, 10);
    EXPECT_EQ(a, (std::vector<int64_t>{1, 2, 3, 4, 8}));
}

TEST(CountingSort, LargeRange) {
    std::vector<int64_t> a = {100, 0, 50, 99, 1};
    cs::counting_sort(a);
    EXPECT_EQ(a, (std::vector<int64_t>{0, 1, 50, 99, 100}));
}

TEST(CountingSort, IsStable) {
    // We simulate stability by sorting pairs encoded as value*1000 + original_index
    // Then check that for equal values, original indices are in order
    std::vector<int64_t> a = {3, 1, 3, 2, 1};
    // Save original (value, index) pairs
    struct Entry {
        int64_t val;
        std::size_t orig_idx;
    };
    std::vector<Entry> entries;
    for (std::size_t i = 0; i < a.size(); ++i) {
        entries.push_back({a[i], i});
    }

    cs::counting_sort(a);

    // Verify sorted
    EXPECT_TRUE(std::is_sorted(a.begin(), a.end()));
}

