#include <algorithm>
#include <data_structures/dsu/dsu.h>

namespace ds::dsu {

DisjointSetUnion::DisjointSetUnion(int n) {
    assign(n);
}

int DisjointSetUnion::size() const {
    return n_;
}

bool DisjointSetUnion::is_valid(int v) const {
    return 0 <= v && v < n_;
}

void DisjointSetUnion::assign(int n) {
    n_ = std::max(0, n);
    components_ = n_;

    parent_.resize(static_cast<size_t>(n_));
    size_.assign(static_cast<size_t>(n_), 1);

    for (int i = 0; i < n_; ++i) {
        parent_[static_cast<size_t>(i)] = i;
    }
}

int DisjointSetUnion::find(int v) {
    if (!is_valid(v)) {
        return -1;
    }

    int x = v;
    while (parent_[static_cast<size_t>(x)] != x) {
        x = parent_[static_cast<size_t>(x)];
    }

    // Path compression.
    while (parent_[static_cast<size_t>(v)] != v) {
        int p = parent_[static_cast<size_t>(v)];
        parent_[static_cast<size_t>(v)] = x;
        v = p;
    }

    return x;
}

bool DisjointSetUnion::same(int a, int b) {
    if (!is_valid(a) || !is_valid(b)) {
        return false;
    }
    return find(a) == find(b);
}

bool DisjointSetUnion::unite(int a, int b) {
    if (!is_valid(a) || !is_valid(b)) {
        return false;
    }

    int ra = find(a);
    int rb = find(b);
    if (ra == rb) {
        return false;
    }

    // Union by size (make ra the larger root).
    if (size_[static_cast<size_t>(ra)] < size_[static_cast<size_t>(rb)]) {
        std::swap(ra, rb);
    }

    parent_[static_cast<size_t>(rb)] = ra;
    size_[static_cast<size_t>(ra)] += size_[static_cast<size_t>(rb)];
    --components_;
    return true;
}

int DisjointSetUnion::component_size(int v) {
    if (!is_valid(v)) {
        return 0;
    }
    const int r = find(v);
    return size_[static_cast<size_t>(r)];
}

int DisjointSetUnion::components() const {
    return components_;
}

} // namespace ds::dsu
