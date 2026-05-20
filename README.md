# Algorithm Lab

**Algorithm Lab** — a curated personal collection of **classic and modern algorithms** implemented in **C++20**, featuring:
- Clean, well-documented implementations
- Real-world examples and test coverage (GoogleTest)
- Performance measurement via **Google Benchmark**
- Profiling and correctness validation
- Experiments with **lock-free and concurrent data structures** for low-latency systems

> **Goal:** Combine algorithmic clarity, modern C++ design, and measurable performance — bridging academic understanding with production-grade efficiency.

---

## 📖 Table of Contents

1. [Algebra, Number Theory & Combinatorics](#1-algebra-number-theory--combinatorics)
2. [Graph Algorithms](#2-graph-algorithms)
3. [Sorting and Searching](#3-sorting-and-searching)
4. [Data Structures](#4-data-structures)
5. [Dynamic Programming & Optimization](#5-dynamic-programming--optimization)
6. [String Algorithms](#6-string-algorithms)
7. [Geometry & Linear Algebra](#7-geometry--linear-algebra)
8. [Bit Manipulation](#8-bit-manipulation)
9. [Lock-Free and Concurrent Data Structures](#9-lock-free-and-concurrent-data-structures)
10. [Custom Allocators](#10-custom-allocators)
11. [Parallel and System-Level Algorithms](#11-parallel-and-system-level-algorithms)
12. [Machine Learning & Numerical Methods](#12-machine-learning--numerical-methods)

---

## 📚 Core Algorithm Categories

Below is the roadmap of all algorithms and data structures to include.
Each category can be implemented with:
- ✅ **Implementation (`.cpp/.h`)**
- ✅ **Unit test** (`GoogleTest`)
- ✅ **Benchmark** (`Google Benchmark`)
- ✅ **Proof or analysis** (`proof.md`)

> **Legend:** Items with links (✅) are implemented; plain text items are planned.

---

### 1. Algebra, Number Theory & Combinatorics

Fundamental algebraic, number-theory, and combinatorial algorithms commonly used across competitive programming, cryptography, and algorithmic libraries.

#### Number Theory

| Algorithm | Description |
|---|---|
| ✅ [Euclidean algorithm (GCD)](src/algebra/gcd/README.md) | Iterative/recursive gcd(a, b). |
| ✅ [Extended Euclid](src/algebra/gcdext/README.md) | Find x, y such that ax + by = gcd(a,b). Used for modular inverses and Diophantine equations. |
| ✅ [Modular inverse](src/algebra/mod_inv/README.md) | Compute a⁻¹ mod m (via extgcd or powmod when m is prime). |
| ✅ [Fast modular exponentiation (powmod)](src/algebra/powmod/README.md) | Binary exponentiation modulo m. |
| ✅ [Chinese Remainder Theorem (CRT)](src/algebra/crt/README.md) | Solve x ≡ aᵢ (mod mᵢ) for pairwise-coprime moduli. |
| ✅ [Garner's algorithm](src/algebra/garner/README.md) | CRT variant useful for mixed/non-coprime moduli conversion and reconstruction. |
| ✅ [Sieve of Eratosthenes](src/algebra/sieve/README.md) | Generate primes up to N efficiently. |
| Prime Factorization | Trial division, Wheel factorization. |
| Miller–Rabin primality | Probabilistic primality test (deterministic bases for 64-bit). |
| Pollard's Rho | Practical integer factorization for 64-bit numbers. |
| Euler's totient (φ) | Count integers coprime to n; multiplicative function. |
| Möbius function / inversion | Used for inclusion–exclusion over divisors. |
| Discrete logarithm (BSGS) | Baby-step giant-step for solving aˣ ≡ b (mod p). |
| Number-Theoretic Transform (NTT) | FFT analog in finite fields for fast convolution. |

#### Combinatorics

| Algorithm | Description |
|---|---|
| ✅ [Principle of Inclusion–Exclusion (PIE)](src/math/combinatorics/pie/README.md) | Count via alternating inclusion and exclusion of overlapping sets. |
| nCr (mod prime) | Binomial coefficients modulo a prime (Lucas / precomputed factorials). |
| Pascal's Triangle | Tabulated binomial coefficients. |
| Permutations / Combinations generator | Enumerate or count arrangements. |
| Catalan numbers | Count balanced parentheses, BST shapes, etc. |
| Stirling numbers | Partitions of sets / cycles of permutations. |

---

### 2. Graph Algorithms

| Subcategory | Algorithms |
|---|---|
| **Shortest Path** | ✅ [Dijkstra](src/algorithms/graphs/dijkstra), ✅ [Bellman-Ford](src/algorithms/graphs/bellman_ford), ✅ [Floyd–Warshall](src/algorithms/graphs/floyd_warshall), A*, Johnson's Algorithm |
| **Minimum Spanning Tree** | ✅ [Kruskal](src/algorithms/graphs/kruskal_mst), ✅ [Prim](src/algorithms/graphs/prim_mst/README.md) |
| **Traversal & Ordering** | ✅ [BFS](src/algorithms/graphs/bfs/README.md), ✅ [DFS (iterative & recursive)](src/algorithms/graphs/dfs/README.md), ✅ [Topological Sort](src/algorithms/graphs/topological_sort/README.md) |
| **Connectivity & Cuts** | ✅ [Union-Find (DSU)](src/data_structures/dsu/README.md), Tarjan's Bridge & Articulation Points, ✅ [Kosaraju / SCC](src/algorithms/graphs/kosaraju_scc), 2-SAT |
| **Flow Algorithms** | ✅ [Edmonds–Karp](src/algorithms/graphs/edmonds_karp), ✅ [Dinic](src/algorithms/graphs/dinic), Push–Relabel, Min-Cost Max-Flow |
| **Bipartite & Matching** | ✅ [Bipartite Check (BFS/DFS coloring)](src/algorithms/graphs/bipartite_check), Maximum Matching (Kuhn, Hopcroft–Karp), Minimum Vertex Cover / König's theorem, Weighted Bipartite Matching (Hungarian) |
| **Tree Algorithms** | LCA (binary lifting / HLD), Centroid Decomposition, Euler Tour, Heavy-Light Decomposition |
| **Euler & Hamilton** | Euler Path / Circuit, Hamiltonian Path (backtracking / DP) |
| **Miscellaneous** | DAG Longest Path |

---

### 3. Sorting and Searching

| Type | Algorithms |
|---|---|
| **Comparison-Based** | ✅ [QuickSort](src/algorithms/sorting/quicksort), ✅ [MergeSort](src/algorithms/sorting/mergesort), ✅ [HeapSort](src/algorithms/sorting/heapsort), ✅ [ShellSort](src/algorithms/sorting/shellsort), ✅ [TimSort](src/algorithms/sorting/timsort), ✅ [IntroSort](src/algorithms/sorting/introsort) |
| **Non-Comparison-Based** | ✅ [Counting Sort](src/algorithms/sorting/counting_sort), ✅ [Radix Sort](src/algorithms/sorting/radix_sort), ✅ [Bucket Sort](src/algorithms/sorting/bucket_sort) |
| **Searching** | ✅ [Binary Search](src/algorithms/sorting/binary_search), ✅ [Interpolation Search](src/algorithms/sorting/interpolation_search), ✅ [Ternary Search](src/algorithms/sorting/ternary_search), ✅ [Exponential Search](src/algorithms/sorting/exponential_search) |
| **Selection** | ✅ [QuickSelect](src/algorithms/sorting/quickselect), ✅ [Median of Medians](src/algorithms/sorting/median_of_medians) |

---

### 4. Data Structures

| Category | Implementations |
|---|---|
| **Fundamentals** | Dynamic Array, Linked List, Stack, Queue, Deque |
| **Associative** | ✅ [Hash Table (open addressing)](src/data_structures/associative/hash_table_oa), ✅ [Hash Table (chaining)](src/data_structures/associative/hash_table_chaining), ✅ [Ordered Map (AVL)](src/data_structures/associative/ordered_map) |
| **Trees** | Binary Search Tree, AVL Tree, Red-Black Tree, Segment Tree (+ lazy propagation), ✅ [Fenwick tree (BIT)](src/data_structures/range_query/fenwick), ✅ [Trie](src/data_structures/trie), Treap / Implicit Treap |
| **Heaps / Priority Queues** | Binary Heap, Fibonacci Heap, Pairing Heap |
| **Union-Find / DSU** | ✅ [Path compression + union by rank](src/data_structures/dsu/README.md) |
| **Spatial Structures** | KD-Tree, Quadtree, R-Tree (optional, for advanced geometry) |
| **Range Query Techniques** | ✅ [Sqrt (block) decomposition](src/data_structures/range_query/sqrt_decomposition), ✅ [Mo's algorithm (offline queries)](src/data_structures/range_query/mo), ✅ [RMQ (sparse table)](src/data_structures/range_query/sparse_table) |
| **Advanced** | Persistent Segment Tree, Link-Cut Tree, Wavelet Tree |

---

### 5. Dynamic Programming & Optimization

| Type | Examples |
|---|---|
| **Classic** | Fibonacci (memoized / tabulated), 0/1 Knapsack, Longest Common Subsequence, Edit Distance |
| **Pathfinding** | DP on grids, DP on DAGs |
| **Optimization** | Convex Hull Trick, Divide & Conquer DP, Bitmask DP |
| **Advanced** | Digit DP, Tree DP, SOS DP (subset convolution), Profile DP |

---

### 6. String Algorithms

| Category | Algorithms |
|---|---|
| **Pattern Matching** | ✅ [KMP](src/algorithms/strings/kmp), ✅ [Rabin–Karp](src/algorithms/strings/rabin_karp), ✅ [Z-Algorithm](src/algorithms/strings/z_function), ✅ [Aho–Corasick](src/algorithms/strings/aho_corasick) |
| **Suffix Structures** | ✅ [Suffix Array + LCP (Kasai)](src/algorithms/strings/suffix_array), ✅ [Suffix Tree (Ukkonen)](src/algorithms/strings/suffix_tree), ✅ [Suffix Automaton](src/algorithms/strings/suffix_automaton) |
| **String Hashing** | ✅ [Polynomial Hash](src/algorithms/strings/polynomial_hash), ✅ [Rolling Hash](src/algorithms/strings/rolling_hash) |
| **Palindromes** | Manacher's algorithm, Palindromic Tree (Eertree) |
| **Applications** | Substring Search, Longest Palindromic Substring, Anagram Grouping |

---

### 7. Geometry & Linear Algebra

| Topic | Examples |
|---|---|
| **Computational Geometry** | Convex Hull (Graham, Andrew), Line Intersection, Orientation, Closest Pair, Half-Plane Intersection |
| **Linear Algebra** | Matrix Multiplication (Strassen), Gaussian Elimination, Matrix Exponentiation |
| **Probability / Statistics** | Random sampling, Reservoir sampling, Monte Carlo π estimation |

---

### 8. Bit Manipulation

| Topic | Examples |
|---|---|
| **Basics** | ✅ [Popcount, Power-of-2 checks, Lowest/Highest set bit](src/bit_manipulation/bit_tricks/README.md) |
| **Enumeration** | ✅ [Subset enumeration (Gosper's hack), Submask iteration, Gray code](src/bit_manipulation/bit_enum/README.md) |
| **Applications** | ✅ [XOR tricks, Bitwise sieve](src/bit_manipulation/bit_apps/README.md) |

---

### 9. Lock-Free and Concurrent Data Structures

| Structure | Description / Use |
|---|---|
| **Lock-Free Stack** | ✅ [Based on atomic CAS; Treiber's stack](src/data_structures/lock_free/stack) |
| **Lock-Free Queue** | ✅ [Michael & Scott queue (single-producer/single-consumer or MPMC)](src/data_structures/lock_free/queue) |
| **Ring Buffer (Circular Queue)** | ✅ [Fixed-capacity, cache-friendly, used in trading systems](src/data_structures/lock_free/ring_buffer/README.md) |
| **Lock-Free Hash Map** | ✅ [Open addressing / chained lock-free maps](src/data_structures/lock_free/hash_map/README.md) |
| **Hazard Pointers / Epoch Reclamation** | ✅ [Safe memory reclamation without global locks](src/data_structures/lock_free/hazard_pointers/README.md) |
| **Atomic Variables** | ✅ [Atomic operations / memory ordering](src/data_structures/lock_free/atomic/README.md) |
| **Barrier / Latch Implementations** | ✅ [Thread coordination primitives](src/data_structures/lock_free/barrier/README.md) |
| **Spinlocks / Backoff Strategies** | ✅ [For fallback mechanisms under contention](src/data_structures/lock_free/spinlock/README.md) |
| **Read-Copy-Update (RCU)** | Scalable read-mostly synchronization pattern. |
| **Seqlock** | Lock-free readers with writer sequencing; used for timestamps/counters. |

---

### 10. Custom Allocators

| Allocator | Description |
|---|---|
| System allocator | Default `malloc`/`new`. Baseline for correctness and simple tests. |
| Simple mutex-backed pool | Shared pool protected by a mutex. |
| Per-thread freelist / pool | Per-thread caches/freelists for fixed-size objects. |
| Arena / region allocator | Allocate from a contiguous region and free in bulk. |
| Slab / fixed-size allocator | Preallocated slabs for identical-size objects. |

---

### 11. Parallel and System-Level Algorithms

| Category | Examples |
|---|---|
| **Parallel Sorting** | Parallel MergeSort, Bitonic Sort (SIMD / OpenMP) |
| **Concurrent Data Processing** | Producer-Consumer pipeline, Thread pool |
| **Memory Layout / Cache Optimization** | ✅ [SoA vs AoS](benchmarks/memory_layout/aos_soa), ✅ [cache line alignment, prefetching](benchmarks/memory_layout/cache/README.md) |
| **Scheduling** | Work-stealing queues, load balancing heuristics |
| **SIMD / Vectorization** | Manual intrinsics, auto-vectorization patterns |

---

### 12. Machine Learning & Numerical Methods

| Topic | Example Algorithms |
|---|---|
| **Supervised** | Linear Regression, Logistic Regression, KNN |
| **Optimization** | Gradient Descent (batch / stochastic / momentum), Adam |
| **Unsupervised** | K-Means, PCA |
| **Probabilistic** | Naive Bayes, Hidden Markov Models |
| **Numerical** | Newton's method, Numerical integration (Simpson, trapezoidal) |

---

## ⚙️ Build, Debug & Profiling (shortcuts)

Detailed build, debugging, formatting, and profiling instructions were moved to the `docs/` folder. See:

- [Build instructions (CMake, build presets, formatting)](./docs/build.md)
- [Debug guide (sanitizers, running under a debugger)](./docs/debug.md)
- [Profiling & Google Benchmark (how to run benchmarks and profile)](./docs/profiling.md)

Use those docs for step-by-step commands and examples.
