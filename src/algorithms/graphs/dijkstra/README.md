# Dijkstra (Binary Heap)

## Idea & Correctness
- Greedy SSSP for non-negative weights; invariant: extracted vertex has final shortest distance.
- See `proof.md` for detailed argument.

## When to use
- Weighted graphs without negative edges (routing, maps).
- Alternatives: SPFA, Bellman-Ford, Dial’s, A* (admissible heuristic).

## Complexity
V => number of vertices, E => number of edges
- Time: O((V + E) log V)
- Space: O(V)

## Interface
```cpp
std::vector<int32_t> dijkstra(const Graph& g, int32_t s);