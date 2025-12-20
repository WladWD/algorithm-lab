#pragma once

#include <cstdint>
#include <vector>

namespace ds::range_query::fenwick {
class FenwickTree {
  public:
    FenwickTree() = default;
    explicit FenwickTree(int n);
    explicit FenwickTree(const std::vector<int64_t>& arr);

    // Number of elements tracked by the tree.
    int size() const;

    // Reset to size n, with all values initialized to 0.
    void assign(int n);

    // Add delta to A[idx]. No-op if idx is out of range.
    void add(int idx, int64_t delta);

    // Prefix sum on [0..r] inclusive. Returns 0 if r < 0; clamps r to n-1.
    int64_t prefix_sum(int r) const;

    // Range sum on [l..r] inclusive. Returns 0 for empty/invalid ranges.
    int64_t range_sum(int l, int r) const;

  private:
    int n_ = 0;
    std::vector<int64_t> tree_;
};

} // namespace ds::range_query::fenwick
