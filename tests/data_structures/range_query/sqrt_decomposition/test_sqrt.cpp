#include "data_structures/range_query/sqrt_decomposition/sqrt_decomposition.h"

#include <gtest/gtest.h>
#include <vector>

using namespace ds::range_query::sqrt_decomposition;

TEST(SqrtDecompositionTest, BasicQueries) {
    std::vector<int64_t> arr = {5, 2, 6, 3, 1, 4, 7};
    SqrtDecomposition ds(arr);
    EXPECT_EQ(ds.query(0, 3), 16); // 5+2+6+3
    EXPECT_EQ(ds.query(2, 5), 14); // 6+3+1+4
    EXPECT_EQ(ds.query(4, 6), 12); // 1+4+7
}

TEST(SqrtDecompositionTest, UpdatesWork) {
    std::vector<int64_t> arr = {5, 2, 6, 3, 1, 4, 7};
    SqrtDecomposition ds(arr);
    ds.update(3, 10);              // set index 3 to 10
    EXPECT_EQ(ds.query(0, 3), 23); // 5+2+6+10
    ds.update(5, 0);
    EXPECT_EQ(ds.query(2, 6), 24); // 6+10+1+0+7
}

TEST(SqrtDecompositionTest, EdgeCases) {
    std::vector<int64_t> empty;
    SqrtDecomposition dsEmpty(empty);
    EXPECT_EQ(dsEmpty.query(0, 0), 0);

    std::vector<int64_t> single = {42};
    SqrtDecomposition dsSingle(single);
    EXPECT_EQ(dsSingle.query(0, 0), 42);
    dsSingle.update(0, 7);
    EXPECT_EQ(dsSingle.query(0, 0), 7);
}
