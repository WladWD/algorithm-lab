# Proof of Edmonds–Karp Algorithm Correctness

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

## 2. Residual Graph and Augmenting Paths

### Definition: Residual Graph

For a flow f, the **residual graph** G_f = (V, E_f) has edges:

E_f = {(u, v) : c_f(u, v) > 0}

where the **residual capacity** is:

c_f(u, v) = c(u, v) − f(u, v)

### Definition: Augmenting Path

An **augmenting path** p is a simple path from s to t in G_f.

The **bottleneck capacity** of p is:

c_f(p) = min_{(u,v) ∈ p} c_f(u, v)

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

## 4. Edmonds–Karp Correctness

The Edmonds–Karp algorithm is the Ford–Fulkerson method where augmenting paths
are found using **Breadth-First Search (BFS)**.

### Lemma 4.1: BFS Finds Shortest Augmenting Path

BFS explores vertices in order of distance from s. Thus, the path found has
minimum number of edges among all augmenting paths.

**Proof**: BFS visits vertices level by level. The first time we reach t,
we have found a shortest path. ∎

### Lemma 4.2: Distance Monotonicity

Let δ_f(s, v) be the shortest-path distance from s to v in residual graph G_f.
After augmenting along path p, for all v ∈ V:

δ_{f'}(s, v) ≥ δ_f(s, v)

That is, distances never decrease.

**Proof**: By induction on δ_{f'}(s, v).

- Base: δ_{f'}(s, s) = 0 = δ_f(s, s). ✓

- Inductive step: Consider v with δ_{f'}(s, v) = k > 0.
  Let (u, v) be the last edge on a shortest s → v path in G_{f'}.
  Then δ_{f'}(s, u) = k − 1.

  By induction: δ_{f'}(s, u) ≥ δ_f(s, u), so δ_f(s, u) ≤ k − 1.

  Case 1: (u, v) ∈ E_f.
    Then δ_f(s, v) ≤ δ_f(s, u) + 1 ≤ k.

  Case 2: (u, v) ∉ E_f.
    Then augmenting p used edge (v, u).
    Since p is a shortest path: δ_f(s, u) = δ_f(s, v) + 1.
    So δ_f(s, v) = δ_f(s, u) − 1 ≤ (k − 1) − 1 = k − 2 < k.

  In both cases: δ_f(s, v) ≤ k = δ_{f'}(s, v). ∎

---

## 5. Complexity Analysis

### Theorem: O(V · E) Augmentations

The Edmonds–Karp algorithm performs at most O(V · E) augmentations.

**Proof**:

An edge (u, v) is **critical** on augmenting path p if c_f(u, v) = c_f(p)
(i.e., it's the bottleneck).

After augmentation, (u, v) is removed from E_f.

For (u, v) to reappear, we must later augment along (v, u).
At that point: δ_{f'}(s, v) = δ_{f'}(s, u) + 1.

Before (u, v) disappeared: δ_f(s, v) = δ_f(s, u) + 1.

Combining with Lemma 4.2:
δ_{f'}(s, u) ≥ δ_f(s, u) = δ_f(s, v) − 1 ≤ δ_{f'}(s, v) − 1 = δ_{f'}(s, u)

Wait—we need: δ_{f'}(s, u) = δ_{f'}(s, v) + 1 ≥ δ_f(s, v) + 1 = δ_f(s, u) + 2.

So each time (u, v) becomes critical again, δ(s, u) increases by at least 2.

Since δ(s, u) < V, edge (u, v) can be critical at most V/2 times.

Total critical edges across all augmentations: O(V · E).

Since each augmentation has at least one critical edge: O(V · E) augmentations. ∎

### Corollary: Total Time Complexity

- Each BFS: O(E)
- Number of augmentations: O(V · E)
- **Total: O(V · E²)**

### Space Complexity

- Adjacency lists: O(E)
- BFS queue and parent array: O(V)
- **Total: O(V + E)**

---

## 6. Correctness of Implementation

### Invariant 1: Flow Conservation

After each augmentation, for all v ∉ {s, t}:

∑_u f(u, v) = ∑_w f(v, w) = 0

**Proof**: Augmentation adds Δ to edges on path and −Δ to reverse edges.
For interior vertices, one incoming and one outgoing edge are modified by ±Δ,
preserving conservation. ∎

### Invariant 2: Capacity Constraint

After each augmentation: 0 ≤ f(u, v) ≤ c(u, v).

**Proof**: We only augment by the bottleneck amount, ensuring no edge
exceeds capacity. Reverse edge flows can become negative (representing
"flow cancellation"), but actual edge flows remain in bounds. ∎

### Invariant 3: Skew Symmetry

f(u, v) = −f(v, u) is maintained.

**Proof**: When we add Δ to f(u, v), we add −Δ to f(v, u). ∎

---

## 7. Termination

### Theorem: Algorithm Terminates

If all capacities are integers, Edmonds–Karp terminates.

**Proof**:

1. Each augmentation increases |f| by at least 1 (bottleneck ≥ 1).
2. Maximum flow is bounded by ∑_{(s,v) ∈ E} c(s, v) < ∞.
3. Stronger: O(V · E) augmentations bound ensures termination even without
   integer assumption. ∎

---

## 8. Example Trace

### Network

```
    s ──10──► a ──10──► t
    │                   ▲
    │         5         │
    └────────►b────────┘
```

Edges: (s,a,10), (s,b,5), (a,t,10), (b,t,5)

### Iteration 1

BFS finds: s → a → t (length 2)
Bottleneck: min(10, 10) = 10
Flow after: 10

### Iteration 2

BFS finds: s → b → t (length 2)
Bottleneck: min(5, 5) = 5
Flow after: 15

### Iteration 3

BFS finds no path (residual capacities exhausted).
**Maximum flow = 15**

---

## 9. Comparison with Ford–Fulkerson

| Aspect | Ford–Fulkerson | Edmonds–Karp |
|--------|----------------|--------------|
| Path selection | Any augmenting path | Shortest (BFS) |
| Complexity | O(E · |f*|) | O(V · E²) |
| Termination | May not terminate (irrational) | Always terminates |
| Implementation | DFS-based | BFS-based |

---

## 10. References

1. Edmonds, J.; Karp, R. M. (1972). "Theoretical improvements in algorithmic
   efficiency for network flow problems". *Journal of the ACM*, 19(2), 248–264.

2. Ford, L. R.; Fulkerson, D. R. (1956). "Maximal flow through a network".
   *Canadian Journal of Mathematics*, 8, 399–404.

3. Cormen, T. H.; Leiserson, C. E.; Rivest, R. L.; Stein, C. — *Introduction to
   Algorithms*, 4th Ed., MIT Press, 2022. Chapter 26.

4. Kleinberg, J.; Tardos, É. — *Algorithm Design*, Pearson, 2005. Chapter 7.

5. Ahuja, R. K.; Magnanti, T. L.; Orlin, J. B. — *Network Flows: Theory,
   Algorithms, and Applications*, Prentice Hall, 1993.

