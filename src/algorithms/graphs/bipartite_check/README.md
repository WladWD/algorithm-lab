# Bipartite Check

This folder contains simple implementations of Bipartite Graph checking (BFS and DFS variants).

What is a bipartite graph?
- An undirected graph is bipartite if its vertex set can be partitioned into two disjoint sets L and R such that every edge connects a vertex in L to a vertex in R. Equivalently, the graph has no odd-length cycles.

Approaches implemented here
- BFS coloring (level-based): Perform BFS from every unvisited node, color nodes alternately (0/1) by level; if an edge connects nodes of the same color, the graph is not bipartite.
  - Complexity: O(V + E) time, O(V) space (colors + queue).

- DFS coloring (iterative stack-based): Perform DFS from every unvisited node, color neighbors opposite to the current node; detect same-color edges as contradiction.
  - Complexity: O(V + E) time, O(V) space (colors + stack or recursion stack).

Notes and usage
- Both implementations handle disconnected graphs by iterating over all vertices and starting a new BFS/DFS at each unvisited vertex.