# Algorithm Lab

**Algorithm Lab** — a curated personal collection of **classic and modern algorithms** implemented in **C++20**, featuring:
- Clean, well-documented implementations
- Real-world examples and test coverage (GoogleTest)
- Performance measurement via **Google Benchmark**
- Profiling and correctness validation
- Experiments with **lock-free and concurrent data structures** for and low-latency systems

> **Goal:** Combine algorithmic clarity, modern C++ design, and measurable performance — bridging academic understanding with production-grade efficiency.

---
---

## 📚 Core Algorithm Categories

Below is the roadmap of all algorithms and data structures to include.
Each category can be implemented with:
- ✅ **Implementation (`.cpp/.h`)**
- ✅ **Unit test** (`GoogleTest`)
- ✅ **Benchmark** (`Google Benchmark`)
- ✅ **Proof or analysis** (`proof.md`)

---

### 1. Graph Algorithms

| Subcategory | Algorithms |
|--------------|-------------|
| **Shortest Path** | Dijkstra, Bellman-Ford, Floyd–Warshall, A*, Johnson’s Algorithm |
| **Minimum Spanning Tree** | Kruskal, Prim |
| **Traversal** | BFS, DFS (iterative & recursive), Topological Sort |
| **Connectivity & Cuts** | Union-Find (DSU), Tarjan’s Bridge & Articulation Points, Kosaraju / SCC |
| **Flow Algorithms** | Edmonds–Karp, Dinic, Push–Relabel |
| **Misc** | Bipartite Check, Topological Order Validation, DAG Longest Path |

---

### 2. Sorting and Searching

| Type | Algorithms |
|------|-------------|
| **Comparison-Based** | QuickSort, MergeSort, HeapSort, ShellSort |
| **Non-Comparison-Based** | Counting Sort, Radix Sort, Bucket Sort |
| **Searching** | Binary Search, Interpolation Search, Ternary Search, Exponential Search |
| **Selection** | QuickSelect, Median of Medians |

---

### 3. Data Structures

| Category | Implementations |
|-----------|----------------|
| **Fundamentals** | Dynamic Array, Linked List, Stack, Queue, Deque |
| **Associative** | Hash Table (open addressing + chaining), Ordered Map (balanced BST) |
| **Trees** | Binary Search Tree, AVL Tree, Red-Black Tree, Segment Tree, Fenwick (BIT), Trie |
| **Heaps / Priority Queues** | Binary Heap, Fibonacci Heap, Pairing Heap |
| **Union-Find / DSU** | Path compression + union by rank |
| **Spatial Structures** | KD-Tree, Quadtree, R-Tree (optional, for advanced geometry) |

---

### 4. Dynamic Programming & Optimization

| Type | Examples |
|------|-----------|
| **Classic** | Fibonacci (memoized / tabulated), 0/1 Knapsack, Longest Common Subsequence, Edit Distance |
| **Pathfinding** | DP on grids, DP on DAGs |
| **Optimization** | Convex Hull Trick, Divide & Conquer DP, Bitmask DP |
| **Advanced** | Digit DP, Tree DP, SOS DP (subset convolution) |

---

### 5. Mathematical Algorithms

| Topic | Examples |
|--------|-----------|
| **Number Theory** | GCD / Extended Euclid, Modular Exponentiation, Sieve of Eratosthenes, Prime Factorization |
| **Combinatorics** | nCr (mod prime), Pascal’s Triangle, Permutations / Combinations generator |
| **Geometry** | Convex Hull (Graham, Andrew), Line Intersection, Orientation, Closest Pair |
| **Linear Algebra** | Matrix Multiplication (Strassen), Gaussian Elimination |
| **Probability / Statistics** | Random sampling, reservoir sampling, Monte Carlo π estimation |

---

### 6. String Algorithms

| Category | Algorithms |
|-----------|-------------|
| **Pattern Matching** | KMP, Rabin–Karp, Z-Algorithm, Prefix Function |
| **Suffix Structures** | Suffix Array, Suffix Tree (Ukkonen), LCP Array |
| **String Hashing** | Polynomial Hash, Rolling Hash |
| **Applications** | Substring Search, Longest Palindromic Substring, Anagram Grouping |

---

### 7. Lock-Free and Concurrent Data Structures 

| Structure | Description / Use |
|------------|------------------|
| **Lock-Free Stack** | Based on atomic CAS; Treiber’s stack |
| **Lock-Free Queue** | Michael & Scott queue (single-producer/single-consumer or MPMC) |
| **Ring Buffer (Circular Queue)** | Fixed-capacity, cache-friendly, used in trading systems |
| **Lock-Free Hash Map** | Open addressing + atomic updates |
| **Hazard Pointers / Epoch Reclamation** | Safe memory reclamation without global locks |
| **Wait-Free Counter / Atomic Variables** | Atomic fetch-add / relaxed memory semantics |
| **Barrier / Latch Implementations** | Thread coordination primitives |
| **SPSC / MPSC Queues** | Single- / Multi- producer-consumer queues for data passing |
| **Spinlocks / Backoff Strategies** | For fallback mechanisms under contention |

*Benchmark goals:* microsecond-level latency, millions of ops/sec, lock contention scaling.

---

### 8. Parallel and System-Level Algorithms

| Category | Examples |
|-----------|-----------|
| **Parallel Sorting** | Parallel MergeSort, Bitonic Sort (SIMD / OpenMP) |
| **Concurrent Data Processing** | Producer-Consumer pipeline |
| **Memory Layout / Cache Optimization** | SoA vs AoS, cache line alignment, prefetching |
| **Scheduling** | Work-stealing queues, load balancing heuristics |

---

### 9. Machine Learning Basics (optional / future)
| Topic | Example Algorithms |
|--------|-------------------|
| **Supervised** | Linear Regression, Logistic Regression, KNN |
| **Optimization** | Gradient Descent (batch / stochastic / momentum), Adam |
| **Unsupervised** | K-Means, PCA |
| **Probabilistic** | Naive Bayes, Hidden Markov Models |

---

## ⚙️ Build and Run

```bash
# Configure and build (with tests and benchmarks)
cmake --preset dev
cmake --build --preset dev -j

# Run all tests
ctest --preset dev --output-on-failure

# Run a specific benchmark
./out/build/dev/algorithms/graphs/dijkstra/bench_graphs_dijkstra --benchmark_min_time=2.0