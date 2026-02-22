#include <algorithm>
#include <data_structures/range_query/fenwick/fenwick.h>

namespace ds::range_query::fenwick {
FenwickTree::FenwickTree(int n) {
    assign(n);
}

FenwickTree::FenwickTree(const std::vector<int64_t>& arr) {
    assign(static_cast<int>(arr.size()));
    // Build in O(n log n). Simple and clear.
    for (int i = 0; i < n_; ++i) {
        add(i, arr[i]);
    }
}

int FenwickTree::size() const {
    return n_;
}

void FenwickTree::assign(int n) {
    n_ = std::max(0, n);
    tree_.assign(static_cast<size_t>(n_), 0);
}

void FenwickTree::add(int idx, int64_t delta) {
    if (idx < 0 || idx >= n_) {
        return;
    }

    for (int i = idx; i < n_; i |= (i + 1)) {
        tree_[static_cast<size_t>(i)] += delta;
    }
}

int64_t FenwickTree::prefix_sum(int r) const {
    if (n_ <= 0) {
        return 0;
    }
    if (r < 0) {
        return 0;
    }
    r = std::min(r, n_ - 1);

    int64_t res = 0;
    for (int i = r; i >= 0; i = (i & (i + 1)) - 1) {
        res += tree_[static_cast<size_t>(i)];
    }
    return res;
}

int64_t FenwickTree::range_sum(int l, int r) const {
    if (l > r) {
        return 0;
    }
    if (n_ <= 0) {
        return 0;
    }

    // Clamp to valid domain; if completely outside, return 0.
    if (r < 0 || l >= n_) {
        return 0;
    }
    l = std::max(l, 0);
    r = std::min(r, n_ - 1);
    if (l > r) {
        return 0;
    }

    return prefix_sum(r) - prefix_sum(l - 1);
}

} // namespace ds::range_query::fenwick
