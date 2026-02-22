#include <algorithm>
#include <data_structures/dsu/dsu_rank.h>

namespace ds::dsu {

DisjointSetUnionRank::DisjointSetUnionRank(int n) {
    assign(n);
}

int DisjointSetUnionRank::size() const {
    return n_;
}

bool DisjointSetUnionRank::is_valid(int v) const {
    return 0 <= v && v < n_;
}

void DisjointSetUnionRank::assign(int n) {
    n_ = std::max(0, n);
    components_ = n_;

    parent_.resize(static_cast<size_t>(n_));
    rank_.assign(static_cast<size_t>(n_), 0);
    size_.assign(static_cast<size_t>(n_), 1);

    for (int i = 0; i < n_; ++i) {
        parent_[static_cast<size_t>(i)] = i;
    }
}

int DisjointSetUnionRank::find(int v) {
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

bool DisjointSetUnionRank::same(int a, int b) {
    if (!is_valid(a) || !is_valid(b)) {
        return false;
    }
    return find(a) == find(b);
}

bool DisjointSetUnionRank::unite(int a, int b) {
    if (!is_valid(a) || !is_valid(b)) {
        return false;
    }

    int ra = find(a);
    int rb = find(b);
    if (ra == rb) {
        return false;
    }

    // Union by rank.
    const int rka = rank_[static_cast<size_t>(ra)];
    const int rkb = rank_[static_cast<size_t>(rb)];

    if (rka < rkb) {
        std::swap(ra, rb);
    }

    // Now rank[ra] >= rank[rb]; attach rb under ra.
    parent_[static_cast<size_t>(rb)] = ra;
    size_[static_cast<size_t>(ra)] += size_[static_cast<size_t>(rb)];

    if (rka == rkb) {
        ++rank_[static_cast<size_t>(ra)];
    }

    --components_;
    return true;
}

int DisjointSetUnionRank::component_size(int v) {
    if (!is_valid(v)) {
        return 0;
    }
    const int r = find(v);
    return size_[static_cast<size_t>(r)];
}

int DisjointSetUnionRank::components() const {
    return components_;
}

} // namespace ds::dsu
