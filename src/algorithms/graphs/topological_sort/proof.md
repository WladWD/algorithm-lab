# Topological Sort (Kahn's Algorithm) — Correctness Notes

This note matches the implementation in `src/topological_sort.cpp`.

We provide correctness notes for two variants implemented in this module:

- **Kahn's algorithm** (in-degree + queue)
- **DFS postorder** (reverse exit-time order)

We are given a directed graph `G = (V, E)` represented by an adjacency list `g`, with
vertices `V = {0, 1, ..., n-1}`.

The algorithm computes a topological ordering when the graph is a DAG, and detects cycles
otherwise.

---

## Outputs

`topological_sort(g)` returns `TopologicalSortResult`:

- `order`: vertices removed by the algorithm in the order they are removed
- `has_cycle`:
  - `false` iff `order` is a full topological ordering of all vertices
  - `true` iff the graph contains at least one directed cycle

---

## Algorithm (high level)

1. Compute `indeg[v]` = the number of incoming edges to `v`.
2. Put all vertices with `indeg[v] == 0` into a FIFO queue.
3. While the queue is non-empty:
   - pop a vertex `u` and append it to `order`
   - for each outgoing edge `u -> v`, decrement `indeg[v]`
     - if `indeg[v]` becomes 0, push `v` into the queue

At the end:

- If `order.size() == n`, return `has_cycle = false`.
- Otherwise, return `has_cycle = true`.

(Edges pointing to invalid vertices are ignored defensively and do not affect correctness
on the intended input domain.)

---

## Invariants

Let us call a vertex **removed** if it has been popped from the queue and appended to `order`.
Let `R` be the set of removed vertices.

> **Invariant A (indegree tracks the remaining graph):**
> For any vertex `v` not in `R`, `indeg[v]` equals the number of edges `u -> v` where `u` is also not in `R`.
>
> In other words, `indeg[v]` is the in-degree of `v` in the subgraph induced by `V \ R`.

**Why Invariant A holds:**

- Initially, `R` is empty, so `indeg[v]` is the true in-degree.
- When we remove `u`, we conceptually delete it (and its outgoing edges) from the remaining graph.
  For every outgoing edge `u -> v`, the algorithm decrements `indeg[v]` exactly once, matching
  the deletion of that single incoming edge into `v`.

> **Invariant B (queue contains exactly the zero-indegree remaining vertices):**
> Whenever the algorithm is about to pop from the queue, every vertex currently in the queue
> has `indeg[v] == 0` in the remaining graph.

This follows from how vertices are inserted: they are enqueued only when their `indeg` becomes 0,
which by Invariant A happens exactly when they have no remaining incoming edges.

---

## Correctness for DAGs

Assume the input graph is a DAG.

### 1) The queue is non-empty until all vertices are removed

A basic property of DAGs is: **every non-empty DAG has at least one vertex with in-degree 0**.

Reason: if every vertex had in-degree ≥ 1, picking any vertex and repeatedly following an incoming
edge would eventually repeat a vertex (finite graph), forming a directed cycle — contradiction.

By Invariant A, at any point the remaining graph is still a DAG (removing vertices cannot create
cycles). Therefore the remaining graph always has a zero-indegree vertex, and by Invariant B it will
eventually be enqueued. Thus the algorithm continues until `R = V`, i.e. `order.size() == n`.

### 2) The produced order is topological

Take any edge `u -> v` in the original graph.

When `v` is removed, by Invariant A it has `indeg[v] == 0` in the remaining graph, so there is no
remaining incoming edge into `v`. In particular, `u` cannot still be remaining at that moment;
otherwise the edge `u -> v` would contribute to `indeg[v]`.

Therefore `u` must have been removed earlier, meaning `u` appears before `v` in `order`.

So `order` is a valid topological ordering.

---

## Cycle detection

If the algorithm finishes with `order.size() < n`, then there are vertices remaining but the queue is empty.
By Invariant B this means every remaining vertex has `indeg[v] > 0` in the remaining graph.

Starting from any remaining vertex and repeatedly following an incoming edge stays within the remaining
graph (because indegree is positive) and must eventually repeat a vertex (finite graph), forming a directed
cycle.

So `has_cycle == true` implies the graph contains a cycle.

Conversely, if the graph contains a directed cycle, then no vertex on that cycle can ever reach indegree 0
while all cycle vertices remain, so the algorithm cannot remove all vertices. Thus it will end with
`order.size() < n` and set `has_cycle == true`.

---

## DFS-based variant (reverse postorder)

This section sketches correctness for `topological_sort_dfs`.

### Algorithm

We run a full DFS forest over vertices in increasing id order. We maintain a `color` array:

- **white**: not visited
- **gray**: on the current DFS recursion stack
- **black**: fully processed

When DFS finishes exploring a vertex `v` (i.e. all outgoing edges have been processed), we append `v` to an array `out`.
Finally, we reverse `out`.

### Invariants

> **Invariant D (postorder property):**
> A vertex `v` is appended to `out` only after every vertex reachable from `v` by a directed path in the DFS tree has finished (becomes black).

This is the standard DFS postorder property: children finish before parents.

### Correctness on DAGs

Assume the graph is a DAG.

Consider any directed edge `u -> v`.

- If DFS discovers `v` (directly or indirectly) while exploring `u`, then `v` finishes before `u` and is appended to `out` before `u`.
- If `v` was already finished when `u` is explored, then `v` is already in `out`.

In both cases, `v` appears before `u` in `out`, so after reversing, `u` appears before `v`.

Therefore the reversed postorder is a valid topological ordering.

### Cycle detection via back-edges

During DFS, encountering an edge `u -> v` where `v` is **gray** means `v` is an ancestor of `u` in the current recursion stack.
This implies there is a directed path `v => ... => u` in the DFS tree, and together with `u -> v` it forms a directed cycle.

So the algorithm sets `has_cycle = true` exactly when it finds such a back-edge.

Conversely, if the graph contains a directed cycle, then during DFS of that cycle, some edge will eventually point to a vertex that is still gray (an ancestor on the stack), producing a back-edge and detecting the cycle.

---

## Complexity

Computing in-degrees takes `O(E)` total work. Each vertex is enqueued/dequeued at most once, and each edge is
processed once when its source is removed.

Total time is `O(V + E)` and extra space is `O(V)`.
