# Disjoint Set Union (Union-Find)

A **Disjoint Set Union** (DSU), also known as **Union-Find**, maintains a partition of the set
`{0, 1, ..., n-1}` into disjoint components.

It supports:

- **Find**: get a component representative (leader)
- **Union**: merge two components
- **Same**: check if two vertices are in the same component
- **Component size** and **number of components**

This implementation uses **union by size** + **path compression**, giving near-constant amortized time.

---

## API

Header: `include/data_structures/dsu/dsu.h`

```cpp
#include <data_structures/dsu/dsu.h>

using ds::dsu::DisjointSetUnion;

DisjointSetUnion dsu(5);

dsu.unite(0, 1);
dsu.unite(3, 4);

bool ok = dsu.same(0, 2);  // false
int root = dsu.find(1);    // representative of {0,1}
int sz = dsu.component_size(4); // 2
int comps = dsu.components();   // 3
```

Public methods:

- `DisjointSetUnion(int n)` / `assign(n)`  create `n` singleton sets
- `find(v)`  representative; returns `-1` if `v` invalid
- `unite(a, b)`  merge sets; returns whether merge happened
- `same(a, b)`  whether in same set
- `component_size(v)`  size of vs component
- `components()`  current number of components

### Input validation behavior

Consistent with other modules in this repo:

- `find(v)` returns `-1` for out-of-range `v`.
- `unite(a, b)` returns `false` if inputs are invalid.
- `same(a, b)` returns `false` if inputs are invalid.
- `component_size(v)` returns `0` if `v` is invalid.

---

## Complexity

With union by size and path compression:

- `find`, `unite`, `same`: amortized `O(α(N))`, where `α` is the inverse Ackermann function.
- memory: `O(N)`.

---

## Proof

See [`proof.md`](./proof.md).

---

This module provides two variants:

- `DisjointSetUnion`  **union by size** + path compression (default)
- `DisjointSetUnionRank`  **union by rank** + path compression

Both have the same defensive semantics and asymptotic guarantees.

---

## API

Header (size-based): `include/data_structures/dsu/dsu.h`

Header (rank-based): `include/data_structures/dsu/dsu_rank.h`

```cpp
#include <data_structures/dsu/dsu.h>
#include <data_structures/dsu/dsu_rank.h>

using ds::dsu::DisjointSetUnion;
using ds::dsu::DisjointSetUnionRank;

DisjointSetUnion d1(5);
DisjointSetUnionRank d2(5);

d1.unite(0, 1);
d2.unite(0, 1);
```

Public methods:

- `DisjointSetUnionRank(int n)` / `assign(n)`  create `n` singleton sets
- `find(v)`  representative; returns `-1` if `v` invalid
- `unite(a, b)`  merge sets; returns whether merge happened
- `same(a, b)`  whether in same set
- `component_size(v)`  size of vs component
- `components()`  current number of components

### Input validation behavior

Consistent with other modules in this repo:

- `find(v)` returns `-1` for out-of-range `v`.
- `unite(a, b)` returns `false` if inputs are invalid.
- `same(a, b)` returns `false` if inputs are invalid.
- `component_size(v)` returns `0` if `v` is invalid.

---

## Complexity

With union by rank and path compression:

- `find`, `unite`, `same`: amortized `O(α(N))`, where `α` is the inverse Ackermann function.
- memory: `O(N)`.

---

## Proof

See [`proof.md`](./proof.md).
