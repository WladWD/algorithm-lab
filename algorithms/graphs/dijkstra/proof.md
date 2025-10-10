# Proof of Dijkstra’s Algorithm Correctness

## 1. Problem Definition

Given a **weighted directed graph** \( G = (V, E) \) with non-negative edge weights \( w(u, v) \geq 0 \), and a **source vertex** \( s \in V \), Dijkstra’s algorithm computes the **shortest path distance**
\[
d(s, v) = \min_{p \in P(s,v)} \sum_{(u, v) \in p} w(u, v)
\]
for every vertex \( v \in V \), where \( P(s,v) \) is the set of all paths from \( s \) to \( v \).

---

## 2. Algorithm Overview

The algorithm maintains:
- A **distance array** `dist[v]`, initially ∞ for all \( v \neq s \), and `dist[s] = 0`.
- A **priority queue** of vertices keyed by their tentative distance.

At each step:
1. Extract the vertex \( u \) with the smallest `dist[u]`.
2. For every outgoing edge \( (u, v) \in E \):
   \[
   \text{if } dist[v] > dist[u] + w(u, v) \text{ then update } dist[v] = dist[u] + w(u, v)
   \]

The algorithm stops when the priority queue is empty.

---

## 3. Key Invariant

Let \( S \) be the set of vertices whose shortest distance from \( s \) has been finalized (popped from the queue).

**Invariant:**
> For every vertex \( u \in S \), `dist[u] = d(s, u)` (the true shortest-path distance).

**Proof of Maintenance:**

- Initially, \( S = {} \), so the invariant trivially holds.
- Suppose the invariant holds before an iteration.
- Let \( u \) be the vertex extracted with the minimum `dist[u]`.
- For contradiction, assume \( dist[u] > d(s, u) \).  
  Then there must exist a path \( s \rightsquigarrow u \) going through some vertex \( x \notin S \) whose `dist[x]` was smaller than `dist[u]` — but that vertex would have been extracted earlier, a contradiction.

Hence, when \( u \) is popped, \( dist[u] = d(s, u) \), and adding \( u \) to \( S \) maintains the invariant.

---

## 4. Termination and Correctness

When the priority queue becomes empty:
- All vertices have been added to \( S \),
- For each vertex \( v \in V \), the invariant ensures \( dist[v] = d(s, v) \),
- Thus, all shortest paths are found.

If a vertex \( v \) is unreachable, no edge relaxation will ever update `dist[v]`, so it remains ∞.

Therefore, the algorithm terminates with correct shortest-path distances for all reachable vertices.

---

## 5. Time Complexity

Let:
- \( V = |V| \) (number of vertices),
- \( E = |E| \) (number of edges).

| Implementation | Extract-Min | Total Complexity |
|----------------|--------------|------------------|
| Simple array | \( O(V) \) | \( O(V^2 + E) \) |
| Binary heap | \( O(\log V) \) | \( O((V + E) \log V) \) |
| Fibonacci heap | \( O(1) \) amortized | \( O(V \log V + E) \) |

For **sparse graphs** (typical in competitive programming and real-world maps), \( O((V + E)\log V) \) with a binary heap is optimal.

---

## 6. Limitations

- Requires **non-negative edge weights**.  
  If negative edges exist, Dijkstra’s assumption that the shortest path to a vertex is finalized upon extraction fails.  
  → Use **Bellman–Ford** for graphs with negative weights.
- Not suitable for graphs with **dynamic edge updates** unless re-run or updated incrementally.

---

## 7. Example

Graph:
```
0 --(1)--> 1 --(2)--> 2
 \                         \
  \(5)--------------------->3
```

Starting from node `0`:
- Step 1: `dist = [0, ∞, ∞, ∞]`
- Relax (0→1): `dist[1]=1`; (0→3): `dist[3]=5`
- Extract 1: relax (1→2): `dist[2]=3`
- Extract 2: relax (2→3): `dist[3]=4` (updated)
- Extract 3: done.

Final:  
`dist = [0, 1, 3, 4]`

---

## 8. References

1. E. W. Dijkstra, *A note on two problems in connexion with graphs*, Numerische Mathematik, 1959.
2. Cormen, Leiserson, Rivest, Stein — *Introduction to Algorithms (CLRS)*, 3rd Ed., Chapter 24.
3. Sedgewick & Wayne — *Algorithms, 4th Edition*, Chapter 4.4.
