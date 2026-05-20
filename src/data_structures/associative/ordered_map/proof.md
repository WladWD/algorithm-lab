# Ordered Map (AVL Tree) — Proof of Correctness

## Definitions

**Height:** `height(null) = 0`, `height(node) = 1 + max(height(left), height(right))`.

**Balance factor (bf):** `bf(node) = height(right) − height(left)`.

**AVL invariant:** `|bf(node)| ≤ 1` for every node.

---

## Height bound

**Theorem:** An AVL tree with `n` nodes has height `h ≤ 1.44 log₂(n + 2) − 0.328`.

**Proof sketch:** Let `N(h)` be the minimum number of nodes in an AVL tree of height `h`. Then `N(0) = 0`, `N(1) = 1`, and `N(h) = 1 + N(h−1) + N(h−2)` (worst case: one subtree has height `h−1`, the other `h−2`). This recurrence is bounded below by the Fibonacci sequence: `N(h) ≥ F(h+2) − 1`. Since `F(k) ≈ φ^k / √5`, we get `h = O(log n)`. ∎

---

## Rotations preserve BST order and restore AVL invariant

### Right rotation (rotate_right(y))

```
    y              x
   / \            / \
  x   C    →    A   y
 / \                / \
A   B              B   C
```

- BST order: `A ≤ x ≤ B ≤ y ≤ C`. Unchanged after rotation. ✓
- Heights updated bottom-up (y first, then x). ✓

### Left rotation (rotate_left(x))

Mirror image of right rotation. ✓

### Four rebalance cases in `balance(n)`

| Condition | Action |
|---|---|
| `bf(n) > 1` and `bf(n->right) ≥ 0` | Left-Left on right child — single left rotate |
| `bf(n) > 1` and `bf(n->right) < 0` | Right-Left on right child — right rotate child, then left rotate n |
| `bf(n) < -1` and `bf(n->left) ≤ 0` | Right-Right on left child — single right rotate |
| `bf(n) < -1` and `bf(n->left) > 0` | Left-Right on left child — left rotate child, then right rotate n |

Each case restores `|bf| ≤ 1` at `n` and does not violate `|bf| ≤ 1` at the child moved up. ✓

---

## Correctness of `insert_impl`

**Claim:** After `insert_impl(root, k, v)`, the tree is a valid AVL BST containing all previously present keys plus `(k, v)`.

**Proof by induction on tree height:**
- **Base (null):** A new leaf is created. Height = 1, bf = 0. ✓
- **Inductive step:** `k` is compared to `n->key`. It recurses into the appropriate subtree (or updates in place). The returned subtree satisfies the invariant by the inductive hypothesis. `balance(n)` is called, which updates `n->height` and applies at most two rotations if `|bf(n)| > 1`. After rotations `|bf| ≤ 1` and BST order is preserved. ✓

---

## Correctness of `erase_impl`

**Claim:** After `erase_impl(root, k)`, the tree is a valid AVL BST with `k` removed.

**Proof:**
- If `k` is in a leaf or a node with one child: the node is deleted, the child is returned. ✓
- **Two-children case:** Replace `n` with its inorder successor `succ = min_node(n->right)`. Copy `succ->key` and `succ->value` into `n`, then recursively erase `succ` from `n->right`. The inorder successor's key satisfies `n->key < succ->key ≤ all right-subtree keys`, so BST order is maintained. After the recursive call `balance(n)` restores the AVL invariant. ✓

The `balance` call at each node on the return path propagates height updates upward, keeping the invariant throughout the tree. ✓

---

## Correctness of `find_impl`

`find_impl` is a standard iterative BST search. At each node it goes left if `k < n->key`, right if `k > n->key`, and returns `n` on equality. Termination is guaranteed by finite tree depth. Correctness follows directly from the BST invariant. ✓

---

## O(log n) complexity of all structural operations

`insert_impl` and `erase_impl` make one recursive call per level plus O(1) rotation work at each level on the return path. Total: O(h) = O(log n). ✓

---

## References

- G. Adelson-Velsky and E. Landis, "An algorithm for the organisation of information," Doklady AN SSSR, 1962.
- T. H. Cormen et al., *Introduction to Algorithms* 4th ed., §13 (red-black trees, AVL analysis in exercises).
- D. Knuth, *The Art of Computer Programming Vol. 3*, §6.2.3.
