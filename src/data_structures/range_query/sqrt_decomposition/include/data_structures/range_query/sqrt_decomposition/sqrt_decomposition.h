#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace ds::range_query::sqrt_decomposition {
class SqrtDecomposition {
  public:
    explicit SqrtDecomposition(const std::vector<int64_t>& arr = {});

    // Set A[idx] = val
    void update(int idx, int64_t val);

    // Range sum on [l, r] inclusive. Returns 0 for empty ranges or invalid input.
    int64_t query(int64_t l, int64_t r) const;

    int size() const;

  private:
    int64_t N = 0;
    int64_t B = 1;
    int64_t blockCount = 0;
    std::vector<int64_t> A;
    std::vector<int64_t> blockSum;
};
} // namespace ds::range_query::sqrt_decomposition
