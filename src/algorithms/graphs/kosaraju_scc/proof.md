# Kosaraju's Algorithm  Correctness Notes

This note matches the implementation in `src/kosaraju_scc.cpp`.

We are given a directed graph `G = (V, E)` with vertices `V = {0, 1, ..., n-1}` represented as an
adjacency list `g`.

We denote by `G^T` the **transpose** graph (all edges reversed).

The algorithm returns a mapping `component_of[v]` assigning each vertex to its SCC.

---

## Algorithm (high level)

1. Run a full DFS on `G` and record vertices in the order of **DFS finish time** (postorder).
   Let `order` be the list of vertices in **increasing** finish time; then we will process it in reverse.
2. Build the transpose graph `G^T`.
3. Process vertices in **decreasing finish time** (reverse of `order`).
   For each vertex `v` not yet assigned to a component, run DFS from `v` in `G^T` and assign all
   reached vertices to the current component id.

---

## Key lemma: finish times separate SCCs

Consider the condensation graph `C(G)` that contracts each SCC into a single vertex.
It is a DAG.

**Lemma 1.** Let `A` and `B` be two different SCCs in `G`. If there is an edge `A -> B` in `C(G)`,
then the maximum finish time of vertices in `A` is greater than the maximum finish time of vertices in `B`
in the first DFS pass.

**Reason.** During DFS, if we enter any vertex in `A`, we can reach `B` (because of the edge in the condensation DAG).
Since there is no path from `B` back to `A` (different SCCs), DFS cannot return to an unfinished vertex in `A`
from `B`. Therefore all vertices reachable in `B` must finish before DFS can finish the entry vertex in `A`.
Thus `finish(A) > finish(B)`.

So decreasing finish time induces a reverse-topological order of SCCs in the condensation DAG.

---

## Why the second pass finds exactly one SCC

Let `v` be the first unassigned vertex in decreasing finish time order in step 3.
Let `S` be the SCC of `v`.

**Claim 2.** DFS from `v` in `G^T` reaches every vertex of `S`.

- In `G`, for any `u` in `S` there is a path `u ->* v` and `v ->* u`.
- In `G^T`, all edges reverse, so `v ->* u` still holds.

So all vertices in the SCC are reachable.

**Claim 3.** DFS from `v` in `G^T` cannot reach any vertex outside `S` that is still unassigned.

Suppose DFS in `G^T` reaches some unassigned vertex `x` that lies in SCC `T != S`.
Then in `G` there is a path `x ->* v`.
That means in the condensation graph `C(G)` there is a path `T ->* S`.
By Lemma 1, this implies `finish(T) > finish(S)`.
But we picked `v` (in `S`) as the first unassigned vertex in decreasing finish time order, so no unassigned SCC
can have larger finish time than `S`.
Contradiction.

Therefore the reached set is exactly the SCC `S`.

After assigning `S`, we continue with the next unassigned vertex in decreasing finish time order and repeat,
covering all SCCs.

---

## Complexity

- First DFS pass explores each vertex and edge once: `O(V + E)`.
- Building `G^T` is `O(V + E)`.
- Second DFS pass explores each vertex and edge in `G^T` once: `O(V + E)`.

Total time: `O(V + E)`.

Memory:
- `G^T` stores all reversed edges: `O(V + E)`.
- DFS stacks and arrays: `O(V)`.

