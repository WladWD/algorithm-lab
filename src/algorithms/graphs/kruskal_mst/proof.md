# Kruskals Algorithm  Correctness Notes

This note matches the implementation in `src/kruskal_mst.cpp`.

We are given an undirected weighted graph `G = (V, E)` with vertices `V = {0, 1, ..., n-1}`.

Kruskals algorithm constructs a set of edges `F` by:

1. Sorting all edges by non-decreasing weight.
2. Iterating edges in that order and adding an edge `(u, v)` iff it connects two different DSU components (i.e. it does **not** create a cycle in `F`).

If the graph is connected, the final `F` has `n-1` edges and forms an MST.
If the graph is disconnected, the final `F` forms a minimum spanning forest (MSF).

---

## Key invariant: `F` is always a forest

We only add an edge `(u, v)` when `u` and `v` are in different components of the current forest.
Therefore, each added edge reduces the number of connected components by 1 and cannot create a cycle.
So `F` is always acyclic (a forest).

---

## Cut property (why choosing the lightest crossing edge is safe)

Let `S` be any non-empty proper subset of vertices. Consider the cut `(S, V \ S)`.

**Cut property:** the minimum-weight edge crossing the cut belongs to *some* MST.

Kruskals algorithm, at the moment it considers the first edge crossing a cut between two DSU components, is effectively selecting a minimum-weight edge that connects two current components. Such an edge is a minimum-weight crossing edge of the cut defined by one component vs the rest (ties allowed).

By the cut property, adding this edge preserves the possibility to complete the partial solution into an MST.

---

## Exchange argument (classic proof sketch)

Assume Kruskal adds an edge `e` connecting two different components.
Let `T` be any MST.

- If `e` is already in `T`, were fine.
- Otherwise, adding `e` to `T` creates a unique cycle.
  That cycle must contain some other edge `f` that also crosses between the two components (because `e` connects those components).
  Since `e` was chosen as the lightest available edge connecting those components at this step, we have `w(e) <= w(f)`.
  Replacing `f` with `e` gives another spanning tree `T'` with weight `<= T`.

Thus there exists an MST that contains `e` and all previously chosen edges. By induction over chosen edges, Kruskals final set is an MST.

---

## Disconnected graphs  minimum spanning forest

If `G` is disconnected, no set of edges can connect all vertices. Kruskals algorithm runs independently inside each connected component:

- edges never connect different components of `G` (there are none), so DSU components cannot merge across them;
- inside each component, the same argument as above shows the chosen edges form an MST of that component.

Therefore the result is a minimum spanning **forest** (an MST per connected component).

---

## Complexity

- Sorting dominates: `O(E log E)`.
- DSU finds/unites are amortized `O(α(V))` per operation.

