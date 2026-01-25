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

### 1. Algebra & Number Theory

This section collects fundamental algebraic and number-theory algorithms commonly used across competitive programming, cryptography, and algorithmic libraries. Each entry includes a short description.

| Algorithm                                      | Description |
|------------------------------------------------|---|
| [Euclidean algorithm (GCD)](src/algebra/gcd/README.md)   | Iterative/recursive gcd(a, b). |
| [Extended Euclid](src/algebra/gcdext/README.md)          | Find x, y such that ax + by = gcd(a,b). Used for modular inverses and Diophantine equations. |
| [Modular inverse](src/algebra/mod_inv/README.md)        | Compute a^{-1} mod m (via extgcd or powmod when m is prime). |
| [Fast modular exponentiation (powmod)](src/algebra/powmod/README.md)  | Binary exponentiation modulo m. |
| [Chinese Remainder Theorem (CRT)](src/algebra/crt/README.md) | Solve x ≡ a_i (mod m_i) for pairwise-coprime moduli. |
| [Garner's algorithm](src/algebra/garner/README.md)      | CRT variant useful for mixed/non-coprime moduli conversion and reconstruction. |
| [Sieve of Eratosthenes](src/algebra/sieve/README.md)    | Generate primes up to N efficiently. |
| Miller–Rabin primality                         | Probabilistic primality test (deterministic bases for 64-bit). |
| Pollard's Rho                                  | Practical integer factorization for 64-bit numbers. |
| Discrete logarithm (BSGS)                      | Baby-step giant-step for solving a^x ≡ b (mod p). |
| Number-Theoretic Transform (NTT)               | FFT analog in finite fields for fast convolution. |

---

### 2. Graph Algorithms

| Subcategory | Algorithms                                                                                                                                                                                                               |
|--------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Shortest Path** | [Dijkstra](src/algorithms/graphs/dijkstra), [Bellman-Ford](src/algorithms/graphs/bellman_ford), [Floyd–Warshall](src/algorithms/graphs/floyd_warshall), A*, Johnson’s Algorithm                                                                                                                        |
| **Minimum Spanning Tree** | [Kruskal](src/algorithms/graphs/kruskal_mst), [Prim](src/algorithms/graphs/prim_mst/README.md)                                                                                                                                                                                                            |
| **Traversal** | [BFS](src/algorithms/graphs/bfs/README.md), [DFS (iterative & recursive)](src/algorithms/graphs/dfs/README.md), [Topological Sort](src/algorithms/graphs/topological_sort/README.md)                                                                                                                                                                       |
| **Connectivity & Cuts** | [Union-Find (DSU)](src/data_structures/dsu/README.md), Tarjan’s Bridge & Articulation Points, [Kosaraju / SCC](src/algorithms/graphs/kosaraju_scc)                                                                                                                                                  |
| **Flow Algorithms** | Edmonds–Karp, Dinic, Push–Relabel                                                                                                                                                                                        |
| **Bipartite & Matching** | [Bipartite Check (BFS/DFS coloring)](src/algorithms/graphs/bipartite_check), Maximum Matching (Kuhn, Hopcroft–Karp), Minimum Vertex Cover / König's theorem, Bipartite Coloring, Weighted Bipartite Matching (Hungarian) |
| **Misc** | Topological Order Validation, DAG Longest Path                                                                                                                                                                           |

---

### 3. Sorting and Searching

| Type | Algorithms |
|------|-------------|
| **Comparison-Based** | QuickSort, MergeSort, HeapSort, ShellSort |
| **Non-Comparison-Based** | Counting Sort, Radix Sort, Bucket Sort |
| **Searching** | Binary Search, Interpolation Search, Ternary Search, Exponential Search |
| **Selection** | QuickSelect, Median of Medians |

---

### 4. Data Structures

| Category | Implementations |
|-----------|----------------|
| **Fundamentals** | Dynamic Array, Linked List, Stack, Queue, Deque |
| **Associative** | Hash Table (open addressing + chaining), Ordered Map (balanced BST) |
| **Trees** | Binary Search Tree, AVL Tree, Red-Black Tree, Segment Tree, [Fenwick tree (BIT)](src/data_structures/range_query/fenwick), Trie |
| **Heaps / Priority Queues** | Binary Heap, Fibonacci Heap, Pairing Heap |
| **Union-Find / DSU** | Path compression + union by rank |
| **Spatial Structures** | KD-Tree, Quadtree, R-Tree (optional, for advanced geometry) |
| **Range Query Techniques** | [Sqrt (block) decomposition](src/data_structures/range_query/sqrt_decomposition), [Mo's algorithm (offline queries)](src/data_structures/range_query/mo), [RMQ (sparse table)](src/data_structures/range_query/sparse_table) |

---

### 5. Dynamic Programming & Optimization

| Type | Examples |
|------|-----------|
| **Classic** | Fibonacci (memoized / tabulated), 0/1 Knapsack, Longest Common Subsequence, Edit Distance |
| **Pathfinding** | DP on grids, DP on DAGs |
| **Optimization** | Convex Hull Trick, Divide & Conquer DP, Bitmask DP |
| **Advanced** | Digit DP, Tree DP, SOS DP (subset convolution) |

---

### 6. Mathematical Algorithms

| Topic | Examples                                                                                                                                                  |
|--------|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Number Theory** | GCD / Extended Euclid, Modular Exponentiation, Sieve of Eratosthenes, Prime Factorization                                                                 |
| **Combinatorics** | nCr (mod prime), Pascal’s Triangle, Permutations / Combinations generator, [Principle of Inclusion–Exclusion (PIE)](src/math/combinatorics/pie/README.md) |
| **Geometry** | Convex Hull (Graham, Andrew), Line Intersection, Orientation, Closest Pair                                                                                |
| **Linear Algebra** | Matrix Multiplication (Strassen), Gaussian Elimination                                                                                                    |
| **Probability / Statistics** | Random sampling, reservoir sampling, Monte Carlo π estimation                                                                                             |

---

### 7. String Algorithms

| Category | Algorithms |
|-----------|-------------|
| **Pattern Matching** | KMP, Rabin–Karp, Z-Algorithm, Prefix Function |
| **Suffix Structures** | Suffix Array, Suffix Tree (Ukkonen), LCP Array |
| **String Hashing** | Polynomial Hash, Rolling Hash |
| **Applications** | Substring Search, Longest Palindromic Substring, Anagram Grouping |

---

### 8. Lock-Free and Concurrent Data Structures 

| Structure | Description / Use                                                                                              |
|------------|----------------------------------------------------------------------------------------------------------------|
| **Lock-Free Stack** | [Based on atomic CAS; Treiber’s stack](src/data_structures/lock_free/stack)                                    |
| **Lock-Free Queue** | [Michael & Scott queue (single-producer/single-consumer or MPMC)](src/data_structures/lock_free/queue)         |
| **Ring Buffer (Circular Queue)** | [Fixed-capacity, cache-friendly, used in trading systems](src/data_structures/lock_free/ring_buffer/README.md) |
| **Lock-Free Hash Map** | [Open addressing / chained lock-free maps](src/data_structures/lock_free/hash_map/README.md)                   |
| **Hazard Pointers / Epoch Reclamation** | [Safe memory reclamation without global locks](src/data_structures/lock_free/hazard_pointers/README.md)        |
| **Atomic Variables** | [Atomic operations / memory ordering](src/data_structures/lock_free/atomic/README.md)                                                                            |
| **Barrier / Latch Implementations** | [Thread coordination primitives](src/data_structures/lock_free/barrier/README.md)                                                                                 |
| **Spinlocks / Backoff Strategies** | [For fallback mechanisms under contention](src/data_structures/lock_free/spinlock/README.md)                                                                       |

---

### 9. Custom Allocators

| Allocator | Description / Suggested path                                                                                                |
|---|-----------------------------------------------------------------------------------------------------------------------------|
| System allocator | Default `malloc`/`new`. Baseline for correctness and simple tests.                                                          |
| Simple mutex-backed pool | Shared pool protected by a mutex.                                            |
| Per-thread freelist / pool | Per-thread caches/freelists for fixed-size objects.                                   |
| Arena / region allocator | Allocate from a contiguous region and free in bulk.                                  |
| Slab / fixed-size allocator | Preallocated slabs for identical-size objects. |

---

### 10. Parallel and System-Level Algorithms

| Category | Examples |
|-----------|-----------|
| **Parallel Sorting** | Parallel MergeSort, Bitonic Sort (SIMD / OpenMP) |
| **Concurrent Data Processing** | Producer-Consumer pipeline |
| **Memory Layout / Cache Optimization** | [SoA vs AoS](benchmarks/memory_layout/aos_soa), [cache line alignment, prefetching](benchmarks/memory_layout/cache/README.md) |
| **Scheduling** | Work-stealing queues, load balancing heuristics |

---

### 11. Machine Learning Basics
| Topic | Example Algorithms |
|--------|-------------------|
| **Supervised** | Linear Regression, Logistic Regression, KNN |
| **Optimization** | Gradient Descent (batch / stochastic / momentum), Adam |
| **Unsupervised** | K-Means, PCA |
| **Probabilistic** | Naive Bayes, Hidden Markov Models |


## ⚙️ Build, Debug & Profiling (shortcuts)

Detailed build, debugging, formatting, and profiling instructions were moved to the `docs/` folder. See:

- [Build instructions (CMake, build presets, formatting)](./docs/build.md)
- [Debug guide (sanitizers, running under a debugger)](./docs/debug.md)
- [Profiling & Google Benchmark (how to run benchmarks and profile)](./docs/profiling.md)

Use those docs for step-by-step commands and examples.
