# Proofs: Bipartite Check (BFS/DFS coloring)

## Definitions
- An undirected graph G = (V, E) is *bipartite* if V can be partitioned into two disjoint sets L and R such that every edge has one endpoint in L and the other in R.
- Equivalently, G is bipartite iff it contains no odd-length cycle.

## Claim 1: G is bipartite iff G has no odd cycle
- (⇒) If G is bipartite with partition (L, R), any cycle alternates between L and R on successive vertices. Therefore any cycle must have even length because it must return to the starting side after an even number of steps.
- (⇐) If G has no odd cycle, we can construct a 2-coloring by picking an arbitrary root in each connected component and performing BFS layering: assign color 0 to the root, color 1 to neighbors at distance 1, color 0 to distance 2, etc. If any edge connects two vertices at the same BFS level, then there's an odd cycle (path from one to the other through the BFS tree plus the edge), contradicting the hypothesis. Thus BFS layering yields a valid 2-coloring and the graph is bipartite.

## Correctness of BFS/DFS coloring algorithm
- Both BFS and DFS do the same conceptual work: they traverse the connected component and assign alternating colors to neighbors.

Soundness (if algorithm returns a coloring, graph is bipartite):
- The algorithm only assigns colors 0/1 and ensures when assigning a color to a vertex it differs from any already-colored neighbor. If it completes without detecting a conflict (an edge whose endpoints share a color), then every edge connects opposite colors, which is exactly the definition of bipartiteness, so the graph is bipartite.

Completeness (if graph is bipartite, algorithm finds a coloring):
- If the graph is bipartite then there exists a 2-coloring. Starting from any root and assigning alternating colors along edges will never encounter a same-color neighbor because the underlying graph admits such a coloring. Both BFS layering and DFS coloring respect parity of path length from the chosen root, matching the bipartition parity, so they succeed in coloring the whole component.

Detection of non-bipartiteness:
- If the graph contains an odd cycle, any BFS/DFS layered coloring will eventually reach an edge that connects two vertices of the same parity (same color). The algorithm detects this case and reports failure.

## Complexity
- Time complexity for both BFS and DFS implementations: O(V + E)
  - Each vertex is discovered and colored once; each edge is scanned at most twice (once from each endpoint) during the traversal across all components.
- Space complexity: O(V) for the color array plus the traversal auxiliary structure (queue for BFS, stack for DFS) and recursion stack if a recursive DFS were used.

## Remarks and edge cases
- Disconnected graph: both algorithms handle disconnected graphs by iterating over all vertices and starting new traversals from unvisited roots.
- Self-loops: a self-loop (edge v-v) implies an odd cycle of length 1 and the algorithm will detect it as a conflict (neighbor equals the same vertex and same color) and report non-bipartite.
- Parallel edges: no effect on correctness; if two vertices are connected by multiple edges the parity reasoning is unchanged; the implementation treats parallel edges as multiple adjacency entries but the conflict check remains valid.
