#include <algorithms/sorting/binary_search/binary_search.h>

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

namespace bs = algorithms::sorting::binary_search;

// ---------- lower_bound ----------

TEST(BinarySearch, LowerBoundBasic) {
    std::vector<int> a = {1, 3, 3, 5, 7, 9};
    auto it = bs::lower_bound(a.begin(), a.end(), 3);
    EXPECT_EQ(it - a.begin(), 1);  // first 3 at index 1
}

TEST(BinarySearch, LowerBoundMissing) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    auto it = bs::lower_bound(a.begin(), a.end(), 4);
    EXPECT_EQ(it - a.begin(), 2);  // insertion point for 4 is index 2 (before 5)
}

TEST(BinarySearch, LowerBoundBeginning) {
    std::vector<int> a = {5, 10, 15};
    auto it = bs::lower_bound(a.begin(), a.end(), 1);
    EXPECT_EQ(it, a.begin());
}

TEST(BinarySearch, LowerBoundEnd) {
    std::vector<int> a = {5, 10, 15};
    auto it = bs::lower_bound(a.begin(), a.end(), 20);
    EXPECT_EQ(it, a.end());
}

TEST(BinarySearch, LowerBoundEmpty) {
    std::vector<int> a;
    auto it = bs::lower_bound(a.begin(), a.end(), 5);
    EXPECT_EQ(it, a.end());
}

// ---------- upper_bound ----------

TEST(BinarySearch, UpperBoundBasic) {
    std::vector<int> a = {1, 3, 3, 5, 7, 9};
    auto it = bs::upper_bound(a.begin(), a.end(), 3);
    EXPECT_EQ(it - a.begin(), 3);  // first element > 3 at index 3 (value 5)
}

TEST(BinarySearch, UpperBoundMissing) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    auto it = bs::upper_bound(a.begin(), a.end(), 4);
    EXPECT_EQ(it - a.begin(), 2);  // same as lower_bound for missing value
}

TEST(BinarySearch, UpperBoundAllSmaller) {
    std::vector<int> a = {1, 2, 3};
    auto it = bs::upper_bound(a.begin(), a.end(), 5);
    EXPECT_EQ(it, a.end());
}

// ---------- binary_search (existence) ----------

TEST(BinarySearch, SearchFound) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    EXPECT_TRUE(bs::binary_search(a.begin(), a.end(), 5));
    EXPECT_TRUE(bs::binary_search(a.begin(), a.end(), 1));
    EXPECT_TRUE(bs::binary_search(a.begin(), a.end(), 9));
}

TEST(BinarySearch, SearchNotFound) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    EXPECT_FALSE(bs::binary_search(a.begin(), a.end(), 0));
    EXPECT_FALSE(bs::binary_search(a.begin(), a.end(), 4));
    EXPECT_FALSE(bs::binary_search(a.begin(), a.end(), 10));
}

TEST(BinarySearch, SearchEmpty) {
    std::vector<int> a;
    EXPECT_FALSE(bs::binary_search(a.begin(), a.end(), 5));
}

TEST(BinarySearch, SearchSingleElement) {
    std::vector<int> a = {7};
    EXPECT_TRUE(bs::binary_search(a.begin(), a.end(), 7));
    EXPECT_FALSE(bs::binary_search(a.begin(), a.end(), 6));
}

// ---------- equal_range ----------

TEST(BinarySearch, EqualRangeMultiple) {
    std::vector<int> a = {1, 3, 3, 3, 5, 7};
    auto [lo, hi] = bs::equal_range(a.begin(), a.end(), 3);
    EXPECT_EQ(lo - a.begin(), 1);
    EXPECT_EQ(hi - a.begin(), 4);
    EXPECT_EQ(hi - lo, 3);  // three 3s
}

TEST(BinarySearch, EqualRangeSingle) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    auto [lo, hi] = bs::equal_range(a.begin(), a.end(), 5);
    EXPECT_EQ(lo - a.begin(), 2);
    EXPECT_EQ(hi - a.begin(), 3);
    EXPECT_EQ(hi - lo, 1);
}

TEST(BinarySearch, EqualRangeMissing) {
    std::vector<int> a = {1, 3, 5, 7, 9};
    auto [lo, hi] = bs::equal_range(a.begin(), a.end(), 4);
    EXPECT_EQ(lo, hi);  // empty range
    EXPECT_EQ(lo - a.begin(), 2);  // insertion point
}

// ---------- Custom comparator ----------

TEST(BinarySearch, CustomComparator) {
    std::vector<int> a = {9, 7, 5, 3, 1};  // sorted descending
    auto comp = std::greater<int>{};

    auto it = bs::lower_bound(a.begin(), a.end(), 5, comp);
    EXPECT_EQ(*it, 5);

    EXPECT_TRUE(bs::binary_search(a.begin(), a.end(), 7, comp));
    EXPECT_FALSE(bs::binary_search(a.begin(), a.end(), 6, comp));
}

// ---------- Matches std:: ----------

TEST(BinarySearch, MatchesStdLowerBound) {
    std::vector<int> a = {1, 2, 2, 3, 4, 4, 4, 5, 6};
    for (int v = 0; v <= 7; ++v) {
        auto ours = bs::lower_bound(a.begin(), a.end(), v);
        auto stds = std::lower_bound(a.begin(), a.end(), v);
        EXPECT_EQ(ours - a.begin(), stds - a.begin()) << "for value " << v;
    }
}

TEST(BinarySearch, MatchesStdUpperBound) {
    std::vector<int> a = {1, 2, 2, 3, 4, 4, 4, 5, 6};
    for (int v = 0; v <= 7; ++v) {
        auto ours = bs::upper_bound(a.begin(), a.end(), v);
        auto stds = std::upper_bound(a.begin(), a.end(), v);
        EXPECT_EQ(ours - a.begin(), stds - a.begin()) << "for value " << v;
    }
}


