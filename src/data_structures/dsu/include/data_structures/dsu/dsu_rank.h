#pragma once

#include <cstdint>
#include <vector>

namespace ds::dsu {

// Disjoint Set Union (Union-Find) using union by rank + path compression.
//
// Rank is an upper bound on tree height. When merging two roots, the root with the
// smaller rank is attached under the root with the larger rank; if ranks are equal,
// the chosen new root's rank is incremented.
//
// API is defensive:
// - invalid vertex => find = -1, same/unite = false, component_size = 0.
class DisjointSetUnionRank {
  public:
    DisjointSetUnionRank() = default;
    explicit DisjointSetUnionRank(int n);

    int size() const;
    void assign(int n);

    int find(int v);
    bool same(int a, int b);
    bool unite(int a, int b);

    int component_size(int v);
    int components() const;

  private:
    int n_ = 0;
    int components_ = 0;

    std::vector<int> parent_;
    std::vector<int> rank_; // meaningful only for roots
    std::vector<int> size_; // component size stored at roots

    bool is_valid(int v) const;
};

} // namespace ds::dsu
