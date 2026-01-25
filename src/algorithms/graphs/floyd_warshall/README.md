# Floyd–Warshall (All-Pairs Shortest Paths)

This module implements **Floyd–Warshall** for **all-pairs shortest paths (APSP)** using the classic dynamic programming formulation.

It supports:
- directed or undirected graphs (use a symmetric matrix for undirected)
- negative edge weights
- negative cycle detection (via `dist[v][v] < 0` after the run)

---

## When to use

Use Floyd–Warshall when:
- you need distances between *all* pairs of vertices
- the graph is relatively small / dense

If you only need a **single source**, prefer:
- **Dijkstra** for non-negative weights
- **Bellman–Ford** for graphs with negative weights and reachable-cycle detection

---

## API

Header: `include/algorithms/graphs/floyd_warshall/floyd_warshall.h`

```cpp
#include <algorithms/graphs/floyd_warshall/floyd_warshall.h>

using algorithms::graphs::floyd_warshall::FloydWarshallResult;
using algorithms::graphs::floyd_warshall::floyd_warshall;

std::vector<std::vector<int32_t>> w = {
    // 0    1    2
    { 0,   5,   FloydWarshallResult::INF },
    { 2,   0,   1 },
    { FloydWarshallResult::INF, FloydWarshallResult::INF, 0 },
};

auto res = floyd_warshall(w);
if (!res.has_negative_cycle) {
    // res.dist[i][j] is the shortest path distance
    // res.next can be used to reconstruct a shortest path
}
```

### Input format

`weights[i][j]` is interpreted as the edge weight `i -> j`.

- Use `FloydWarshallResult::INF` to represent "no edge".
- Usually set `weights[i][i] = 0`.

If the input matrix is not square, the function returns an empty result.

---

## Complexity

Let `n` be the number of vertices.

- Time: `O(n^3)`
- Memory: `O(n^2)`

---

## Proof / correctness

See [`proof.md`](./proof.md).

