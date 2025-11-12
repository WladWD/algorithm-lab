#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>

namespace ds::range_query::mo {
struct Query {
    int32_t l = -1;
    int32_t r = -1;
    int32_t idx = -1;
};

// Solve distinct-values-in-range queries using Mo's algorithm (classic CP example).
// - A: input array (any integer values)
// - queries: vector of Query (l,r,idx); the function may reorder this vector
// Returns: vector<int> answers where answers[idx] is the answer for the query with that original index.
std::vector<int> mo_solve_distinct(std::vector<int> A, std::vector<Query> queries);
} // namespace ds::range_query::mo
