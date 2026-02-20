# Proof of Dinic's Algorithm Correctness

## 1. Problem Definition

Given a **flow network** G = (V, E) with:

- Source s ∈ V and sink t ∈ V
- Capacity function c : E → ℤ⁺

A **flow** f : V × V → ℝ satisfies:

1. **Capacity constraint**: f(u, v) ≤ c(u, v) for all (u, v) ∈ E
2. **Flow conservation**: For all v ∈ V \ {s, t}:
   ∑_{u ∈ V} f(u, v) = ∑_{w ∈ V} f(v, w)
3. **Skew symmetry**: f(u, v) = −f(v, u)

The **value** of flow f is:

|f| = ∑_{v ∈ V} f(s, v) = ∑_{u ∈ V} f(u, t)

---

## 2. Residual Graph and Level Graph

### Definition: Residual Graph

For a flow f, the **residual graph** G_f = (V, E_f) has edges:

E_f = {(u, v) : c_f(u, v) > 0}

where the **residual capacity** is:

c_f(u, v) = c(u, v) − f(u, v)

### Definition: Level Graph

Given a residual graph G_f, define:

level(v) = shortest-path distance (in number of edges) from s to v in G_f

The **level graph** L_f = (V, E_L) contains exactly those edges:

E_L = {(u, v) ∈ E_f : level(v) = level(u) + 1}

All s-to-t paths in L_f have length exactly level(t).

### Definition: Blocking Flow

A flow g in the level graph L_f is a **blocking flow** if every path from s to t
in L_f contains at least one edge saturated by g (i.e., g(u,v) = c_f(u,v) for
some edge on the path).

---

## 3. Max-Flow Min-Cut Theorem

### Theorem (Ford–Fulkerson, 1956)

The following are equivalent:

1. f is a maximum flow
2. The residual graph G_f has no augmenting path from s to t
3. |f| = c(S, T) for some s-t cut (S, T)

where c(S, T) = ∑_{u ∈ S, v ∈ T} c(u, v) is the cut capacity.

### Proof Sketch

(1 ⟹ 2): If an augmenting path exists, we can increase flow—contradiction.

(2 ⟹ 3): Let S = {v : v is reachable from s in G_f}. Since t ∉ S (no augmenting path),
(S, V \ S) is an s-t cut. For all u ∈ S, v ∈ T: f(u, v) = c(u, v) (saturated).
Thus |f| = c(S, T).

(3 ⟹ 1): For any flow f' and cut (S, T): |f'| ≤ c(S, T).
If |f| = c(S, T), then f is maximum.

---

## 4. Dinic's Algorithm Correctness

### Algorithm Outline

```
while BFS from s reaches t in G_f:
    Build level graph L_f
    Find a blocking flow g in L_f
    Augment: f ← f + g
return f
```

### Lemma 4.1: Level Graph is a DAG

The level graph L_f is a directed acyclic graph (DAG).

**Proof**: Every edge (u, v) ∈ E_L satisfies level(v) = level(u) + 1. Along any path,
levels strictly increase, so no cycle can exist. ∎

### Lemma 4.2: Blocking Flow Increases Distance

Let d_f = level_f(t) be the shortest-path distance from s to t in G_f before a phase,
and d_{f'} = level_{f'}(t) be the distance after augmenting by a blocking flow g.

Then: **d_{f'} > d_f**.

**Proof**:

Let f' = f + g where g is a blocking flow in L_f.

Suppose for contradiction that d_{f'} ≤ d_f. Then there exists a shortest s-t path
P in G_{f'} of length ≤ d_f.

Consider any edge (u, v) on P. Either:

**Case 1**: (u, v) ∈ E_L (it was in the level graph).
Then level_f(v) = level_f(u) + 1.

**Case 2**: (u, v) ∉ E_L but (u, v) ∈ E_{f'}.
Since (u, v) has residual capacity in G_{f'} but was not in L_f, it must be that
the blocking flow g used the reverse edge (v, u) ∈ E_L, which means:
level_f(u) = level_f(v) + 1, i.e., level_f(v) = level_f(u) − 1.

In Case 1, the level increases by 1 along the edge.
In Case 2, the level decreases by 1 along the edge.

Summing over all edges in P:
level_f(t) − level_f(s) = (number of Case 1 edges) − (number of Case 2 edges)

Let P have length k (total edges). Then:
d_f = level_f(t) = (# Case 1) − (# Case 2)
k = (# Case 1) + (# Case 2)

So: # Case 1 = (d_f + k) / 2 and # Case 2 = (k − d_f) / 2.

Since k ≤ d_f (path P has length ≤ d_f in G_{f'}):
# Case 2 = (k − d_f) / 2 ≤ 0, so # Case 2 = 0.

This means every edge on P is a Case 1 edge (i.e., in L_f), and k = d_f.
But then P is a path of length d_f in L_f using only level-graph edges.

Since g is a **blocking flow**, P must contain a saturated edge — meaning
(u, v) has zero residual capacity after g. But then (u, v) ∉ E_{f'}, contradicting
that P is a path in G_{f'}.

Therefore d_{f'} > d_f. ∎

### Theorem 4.3: O(V) Phases

Dinic's algorithm terminates after at most V − 1 phases.

**Proof**: By Lemma 4.2, the shortest-path distance d from s to t strictly
increases after each phase. Since d ≤ V − 1 (a simple path visits at most V
vertices), the algorithm performs at most V − 1 phases. ∎

---

## 5. Complexity Analysis

### Blocking Flow in O(V · E) Time

Using the **current-arc optimization** (maintaining a pointer `iter[v]` for each
vertex v), the DFS-based blocking flow runs in O(V · E) time per phase.

**Proof**:

Each DFS call either:
1. **Advances** — moves to a new vertex along an edge. If the DFS reaches the sink,
   it has found an augmenting path of length ≤ V. At least one edge is saturated,
   removing it from future consideration. Total advance steps across all paths:
   O(V) per path × O(E) paths (each saturated edge is removed) = O(V · E).

2. **Retreats** — the current edge is a dead end. The `iter[v]` pointer advances
   past this edge permanently (within this phase). Total retreat steps: O(E),
   since each edge pointer advances at most once.

Combined: O(V · E) per phase. ∎

### Total Time Complexity

- Number of phases: O(V) (Theorem 4.3)
- Time per phase: O(V · E) (blocking flow) + O(E) (BFS)
- **Total: O(V² · E)**

### Space Complexity

- Adjacency lists: O(E)
- Level array and iterator array: O(V)
- BFS queue: O(V)
- **Total: O(V + E)**

### Special Case: Unit-Capacity Networks

When all edges have capacity 1, the maximum flow |f*| ≤ min(|δ⁺(s)|, |δ⁻(t)|).

More importantly, each blocking flow saturates at least one edge per augmenting path,
and the number of phases is bounded by O(√V) (via a counting argument on the level
graph structure).

Each phase takes O(E) time (each augmenting path has length ≤ V, but total work is
bounded by the number of edges).

**Total: O(E · √V)** for unit-capacity networks.

---

## 6. Correctness of Implementation

### Invariant 1: Flow Conservation

After each augmentation, for all v ∉ {s, t}:

∑_u f(u, v) = ∑_w f(v, w) = 0

**Proof**: The DFS augments along a complete s-t path. For every interior vertex on the
path, exactly one incoming and one outgoing edge are modified by ±Δ, preserving net flow
conservation. ∎

### Invariant 2: Capacity Constraint

After each augmentation: 0 ≤ f(u, v) ≤ c(u, v).

**Proof**: We only augment by the minimum residual capacity along the path (the bottleneck),
ensuring no edge exceeds capacity. Reverse edge flows can become negative (representing
"flow cancellation"), but actual edge flows remain in bounds. ∎

### Invariant 3: Skew Symmetry

f(u, v) = −f(v, u) is maintained.

**Proof**: When we add Δ to f(u, v), we add −Δ to f(v, u) via the reverse edge update. ∎

### Invariant 4: Level Graph Consistency

The DFS only follows edges where level(v) = level(u) + 1.

**Proof**: The `if` condition in the DFS explicitly checks `level_[e.to] != level_[v] + 1`
and skips such edges. ∎

### Invariant 5: Current-Arc Optimization

Once `iter[v]` advances past edge index i, edge i is never reconsidered in the same phase.

**Proof**: The `iter_` reference is incremented in the for-loop and persists across
recursive DFS calls. ∎

---

## 7. Termination

### Theorem: Algorithm Terminates

Dinic's algorithm always terminates.

**Proof**:

1. Each phase finds a blocking flow that increases the shortest-path distance
   from s to t by at least 1 (Lemma 4.2).
2. The shortest-path distance is bounded by V − 1.
3. When BFS cannot reach t (distance = ∞), the algorithm terminates.
4. Therefore, at most V − 1 phases are executed. ∎

---

## 8. Example Trace

### Network

```
    s ──10──► a ──10──► t
    │                   ▲
    │         5         │
    └────────►b────────┘
```

Edges: (s, a, 10), (s, b, 5), (a, t, 10), (b, t, 5)

### Phase 1

**BFS** from s:
- level(s) = 0
- level(a) = 1, level(b) = 1
- level(t) = 2

**Level graph** edges: s→a, s→b, a→t, b→t

**DFS blocking flow**:
1. Path s → a → t, push min(10, 10) = 10
2. Path s → b → t, push min(5, 5) = 5

Blocking flow value: 15

### Phase 2

**BFS** from s in residual graph:
- s has no outgoing edges with residual capacity (s→a saturated at 10, s→b saturated at 5)
- t is unreachable

**Algorithm terminates. Maximum flow = 15**

---

## 9. Comparison with Other Max-Flow Algorithms

| Aspect | Ford–Fulkerson | Edmonds–Karp | Dinic | Push-Relabel |
|--------|----------------|--------------|-------|--------------|
| Strategy | Any augmenting path | Shortest path (BFS) | Blocking flow on level graph | Local push + relabel |
| Path selection | DFS / arbitrary | BFS | DFS on level graph | N/A (vertex-based) |
| Time complexity | O(E · \|f*\|) | O(V · E²) | O(V² · E) | O(V² · E) or O(V³) |
| Unit-capacity | O(E · \|f*\|) | O(E · min(E^{1/2}, V^{2/3})) | **O(E · √V)** | O(E · √V) |
| Termination | May not (irrational) | Always | Always | Always |
| Implementation | Simple | Simple | Moderate | Complex |
| Practical speed | Slow | Moderate | Fast | Often fastest |

---

## 10. References

1. Dinic, E. A. (1970). "Algorithm for solution of a problem of maximum flow in
   a network with power estimation". *Soviet Mathematics Doklady*, 11, 1277–1280.

2. Even, S.; Tarjan, R. E. (1975). "Network flow and testing graph connectivity".
   *SIAM Journal on Computing*, 4(4), 507–518.

3. Ford, L. R.; Fulkerson, D. R. (1956). "Maximal flow through a network".
   *Canadian Journal of Mathematics*, 8, 399–404.

4. Cormen, T. H.; Leiserson, C. E.; Rivest, R. L.; Stein, C. — *Introduction to
   Algorithms*, 4th Ed., MIT Press, 2022. Chapter 26.

5. Kleinberg, J.; Tardos, É. — *Algorithm Design*, Pearson, 2005. Chapter 7.

6. Ahuja, R. K.; Magnanti, T. L.; Orlin, J. B. — *Network Flows: Theory,
   Algorithms, and Applications*, Prentice Hall, 1993.

