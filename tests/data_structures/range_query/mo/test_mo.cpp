#include "data_structures/range_query/mo/mo.h"

#include <gtest/gtest.h>
#include <vector>

using namespace ds::range_query::mo;

TEST(MoTest, ExampleMatchesExpected) {
    std::vector<int> A = {1, 2, 1, 3, 2, 1};
    std::vector<Query> queries = {{0, 2, 0}, {1, 4, 1}, {2, 5, 2}};
    auto ans = mo_solve_distinct(A, queries);
    ASSERT_EQ(ans.size(), 3u);
    EXPECT_EQ(ans[0], 2);
    EXPECT_EQ(ans[1], 3);
    EXPECT_EQ(ans[2], 3);
}

TEST(MoTest, EmptyQueries) {
    std::vector<int> A = {1, 2, 3};
    std::vector<Query> queries;
    auto ans = mo_solve_distinct(A, queries);
    EXPECT_TRUE(ans.empty());
}
