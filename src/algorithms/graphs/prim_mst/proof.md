# Prim’s Algorithm — Correctness Notes

This note matches the implementation in `src/prim_mst.cpp`.

We are given an undirected weighted graph `G = (V, E)` with vertices `V = {0, 1, ..., n-1}`.

Prim’s algorithm constructs a spanning tree by growing a set of visited vertices `S` and repeatedly adding the cheapest edge that connects `S` to `V \ S`.

In this repository we also handle disconnected graphs by running Prim starting from each unvisited vertex, producing a **minimum spanning forest**.

---

## Key invariant: chosen edges never create a cycle

An edge is added only when it goes from a visited vertex to an unvisited one. Therefore every added edge introduces exactly one new vertex into the current component’s tree, so cycles cannot be created.

---

## Cut property (why the greedy choice is safe)

Let `S` be the set of vertices already included in the current partial tree (within one connected component).
Consider the cut `(S, V \ S)`.

**Cut property:** For any cut, the minimum-weight edge crossing the cut belongs to some MST.

At each step, Prim’s algorithm picks a minimum-weight edge leaving `S` (ties allowed). That edge is a minimum-weight crossing edge for the cut `(S, V \ S)`, so by the cut property it is *safe* to add: there exists an MST that includes all currently selected edges.

By induction over the steps, Prim’s algorithm outputs an MST for a connected graph.

---

## Disconnected graphs — minimum spanning forest

If the graph is disconnected, there is no spanning tree that covers all vertices. The implementation runs Prim separately from each unvisited vertex; within each connected component, the same cut-property argument applies.

Thus the result is a minimum spanning **forest** (an MST per connected component).

---

## Complexity

With an adjacency list and a binary heap:

- Building adjacency list: `O(E)`.
- Each edge can be inserted into the heap (in the worst case) `O(1)` times per endpoint, so `O(E)` pushes.
- Each heap operation costs `O(log E)`.

Total time: `O((V + E) log E)`, memory `O(V + E)`.

