#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>

namespace ds::ordered_map {

// Ordered key-value map backed by an AVL tree.
//
// Keys are kept in sorted order according to Compare (default: std::less<Key>).
// All structural operations maintain the AVL balance invariant:
//   |height(right) - height(left)| ≤ 1 for every node.
//
// Complexity:
//   insert, erase, find, contains, operator[]: O(log n)
//   min_key, max_key:                          O(log n)
//   size, empty:                               O(1)
//   clear:                                     O(n)
//
// Deep copy is supported (O(n) copy constructor / copy assignment).
// Move is O(1).

template <typename Key,
          typename Value,
          typename Compare = std::less<Key>>
class OrderedMap {
public:
    OrderedMap()  = default;
    ~OrderedMap() { delete_tree(root_); }

    OrderedMap(const OrderedMap& other)
        : root_(copy_tree(other.root_)), size_(other.size_) {}

    OrderedMap& operator=(OrderedMap other) noexcept {
        std::swap(root_,  other.root_);
        std::swap(size_,  other.size_);
        std::swap(cmp_,   other.cmp_);
        return *this;
    }

    OrderedMap(OrderedMap&& other) noexcept
        : root_(other.root_), size_(other.size_), cmp_(std::move(other.cmp_)) {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    /// Insert or overwrite value for key. O(log n).
    void insert(Key key, Value value);

    /// Remove key. Returns true if key was found. O(log n).
    bool erase(const Key& key);

    /// Return pointer to value, nullptr if absent. O(log n).
    Value*       find(const Key& key);
    const Value* find(const Key& key) const;

    /// True if key is present. O(log n).
    bool contains(const Key& key) const;

    /// Insert default-constructed value if absent, return reference.
    /// Requires Value to be default-constructible. O(log n).
    Value& operator[](const Key& key);

    /// Pointer to the minimum key, nullptr if empty. O(log n).
    const Key* min_key() const;
    /// Pointer to the maximum key, nullptr if empty. O(log n).
    const Key* max_key() const;

    std::size_t size()  const noexcept { return size_; }
    bool        empty() const noexcept { return size_ == 0; }
    void        clear();

private:
    struct Node {
        Key   key;
        Value value;
        Node* left   = nullptr;
        Node* right  = nullptr;
        int   height = 1;

        Node(Key k, Value v)
            : key(std::move(k)), value(std::move(v)) {}
    };

    Node*       root_  = nullptr;
    std::size_t size_  = 0;
    Compare     cmp_{};

    // ── tree utilities ──────────────────────────────────────────────────────

    static int height(const Node* n) noexcept { return n ? n->height : 0; }
    static int bf(const Node* n)     noexcept {
        return n ? height(n->right) - height(n->left) : 0;
    }
    static void update_height(Node* n) noexcept {
        if (n) n->height = 1 + std::max(height(n->left), height(n->right));
    }

    static Node* rotate_right(Node* y) noexcept {
        Node* x  = y->left;
        y->left  = x->right;
        x->right = y;
        update_height(y);
        update_height(x);
        return x;
    }

    static Node* rotate_left(Node* x) noexcept {
        Node* y  = x->right;
        x->right = y->left;
        y->left  = x;
        update_height(x);
        update_height(y);
        return y;
    }

    static Node* balance(Node* n) noexcept {
        update_height(n);
        int b = bf(n);
        if (b > 1) {  // right-heavy
            if (bf(n->right) < 0)           // right-left
                n->right = rotate_right(n->right);
            return rotate_left(n);
        }
        if (b < -1) { // left-heavy
            if (bf(n->left) > 0)            // left-right
                n->left = rotate_left(n->left);
            return rotate_right(n);
        }
        return n;
    }

    static Node* min_node(Node* n) noexcept {
        while (n && n->left) n = n->left;
        return n;
    }
    static Node* max_node(Node* n) noexcept {
        while (n && n->right) n = n->right;
        return n;
    }

    static Node* copy_tree(const Node* n) {
        if (!n) return nullptr;
        Node* c   = new Node(n->key, n->value);
        c->height = n->height;
        c->left   = copy_tree(n->left);
        c->right  = copy_tree(n->right);
        return c;
    }

    static void delete_tree(Node* n) noexcept {
        if (!n) return;
        delete_tree(n->left);
        delete_tree(n->right);
        delete n;
    }

    // ── recursive operations ────────────────────────────────────────────────

    Node* insert_impl(Node* n, Key key, Value value, bool& size_changed);
    Node* erase_impl(Node* n, const Key& key, bool& erased);
    Node* find_impl(Node* n, const Key& key) const noexcept;
};

// ──────────────────────────── method definitions ────────────────────────────

template <typename K, typename V, typename C>
typename OrderedMap<K, V, C>::Node*
OrderedMap<K, V, C>::insert_impl(Node* n, K key, V value, bool& size_changed) {
    if (!n) {
        size_changed = true;
        return new Node(std::move(key), std::move(value));
    }
    if (cmp_(key, n->key)) {
        n->left  = insert_impl(n->left,  std::move(key), std::move(value), size_changed);
    } else if (cmp_(n->key, key)) {
        n->right = insert_impl(n->right, std::move(key), std::move(value), size_changed);
    } else {
        n->value = std::move(value); // update
    }
    return balance(n);
}

template <typename K, typename V, typename C>
typename OrderedMap<K, V, C>::Node*
OrderedMap<K, V, C>::erase_impl(Node* n, const K& key, bool& erased) {
    if (!n) return nullptr;

    if (cmp_(key, n->key)) {
        n->left  = erase_impl(n->left,  key, erased);
    } else if (cmp_(n->key, key)) {
        n->right = erase_impl(n->right, key, erased);
    } else {
        erased = true;
        if (!n->left || !n->right) {
            Node* child = n->left ? n->left : n->right;
            delete n;
            return child; // child may be nullptr (leaf case)
        }
        // Two children: replace with inorder successor (min of right subtree).
        Node*       succ     = min_node(n->right);
        K           succ_key = succ->key;   // copy before tree is modified
        n->key   = succ->key;
        n->value = std::move(succ->value);
        bool dummy = false;
        n->right = erase_impl(n->right, succ_key, dummy);
        erased = true;
    }
    return balance(n);
}

template <typename K, typename V, typename C>
typename OrderedMap<K, V, C>::Node*
OrderedMap<K, V, C>::find_impl(Node* n, const K& key) const noexcept {
    while (n) {
        if      (cmp_(key, n->key)) n = n->left;
        else if (cmp_(n->key, key)) n = n->right;
        else                        return n;
    }
    return nullptr;
}

template <typename K, typename V, typename C>
void OrderedMap<K, V, C>::insert(K key, V value) {
    bool changed = false;
    root_ = insert_impl(root_, std::move(key), std::move(value), changed);
    if (changed) ++size_;
}

template <typename K, typename V, typename C>
bool OrderedMap<K, V, C>::erase(const K& key) {
    bool erased = false;
    root_ = erase_impl(root_, key, erased);
    if (erased) --size_;
    return erased;
}

template <typename K, typename V, typename C>
V* OrderedMap<K, V, C>::find(const K& key) {
    Node* n = find_impl(root_, key);
    return n ? &n->value : nullptr;
}

template <typename K, typename V, typename C>
const V* OrderedMap<K, V, C>::find(const K& key) const {
    const Node* n = find_impl(root_, key);
    return n ? &n->value : nullptr;
}

template <typename K, typename V, typename C>
bool OrderedMap<K, V, C>::contains(const K& key) const {
    return find_impl(root_, key) != nullptr;
}

template <typename K, typename V, typename C>
V& OrderedMap<K, V, C>::operator[](const K& key) {
    if (!contains(key)) insert(key, V{});
    return *find(key);
}

template <typename K, typename V, typename C>
const K* OrderedMap<K, V, C>::min_key() const {
    Node* n = min_node(root_);
    return n ? &n->key : nullptr;
}

template <typename K, typename V, typename C>
const K* OrderedMap<K, V, C>::max_key() const {
    Node* n = max_node(root_);
    return n ? &n->key : nullptr;
}

template <typename K, typename V, typename C>
void OrderedMap<K, V, C>::clear() {
    delete_tree(root_);
    root_  = nullptr;
    size_  = 0;
}

} // namespace ds::ordered_map
