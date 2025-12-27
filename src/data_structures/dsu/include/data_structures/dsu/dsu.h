#pragma once

#include <cstdint>
#include <vector>

namespace ds::dsu {

// Disjoint Set Union (Union-Find) data structure.
//
// Supports merging sets and querying representatives in almost-constant amortized time
// using union by size and path compression.
//
// API is intentionally defensive and consistent with other data structures in this repo:
// operations on invalid vertices are treated as no-ops / safe defaults.
class DisjointSetUnion {
  public:
    DisjointSetUnion() = default;
    explicit DisjointSetUnion(int n);

    // Current number of elements (0..n-1).
    int size() const;

    // Reset the structure to contain n singleton sets.
    void assign(int n);

    // Find representative (leader) of the set containing v.
    // Returns -1 if v is out of range.
    int find(int v);

    // Check whether a and b are in the same set. Returns false if a/b invalid.
    bool same(int a, int b);

    // Merge sets containing a and b.
    // Returns true if a merge happened; false if already same set or invalid inputs.
    bool unite(int a, int b);

    // Size of the component containing v. Returns 0 if v is out of range.
    int component_size(int v);

    // Number of connected components currently tracked.
    int components() const;

  private:
    int n_ = 0;
    int components_ = 0;
    std::vector<int> parent_;
    std::vector<int> size_;

    bool is_valid(int v) const;
};

} // namespace ds::dsu
