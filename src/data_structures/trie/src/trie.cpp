#include <algorithm>
#include <data_structures/trie/trie.h>

namespace ds::trie {

// --- Node -------------------------------------------------------------------

Trie::Node::Node() : prefix_count(0), is_end(false) {
    std::fill(std::begin(children), std::end(children), -1);
}

// --- Trie public API --------------------------------------------------------

Trie::Trie() : word_count_(0) {
    nodes_.reserve(256);
    new_node(); // root is always index 0
}

void Trie::insert(std::string_view word) {
    // Check for invalid characters before modifying the trie.
    for (char c : word) {
        if (char_to_idx(c) < 0) {
            return; // invalid character — reject silently
        }
    }

    int cur = 0; // root
    nodes_[cur].prefix_count++;

    for (char c : word) {
        int idx = char_to_idx(c);
        if (nodes_[cur].children[idx] == -1) {
            nodes_[cur].children[idx] = new_node();
        }
        cur = nodes_[cur].children[idx];
        nodes_[cur].prefix_count++;
    }

    if (!nodes_[cur].is_end) {
        nodes_[cur].is_end = true;
        ++word_count_;
    } else {
        // Word already exists — undo the prefix_count increments.
        cur = 0;
        nodes_[cur].prefix_count--;
        for (char c : word) {
            int idx = char_to_idx(c);
            cur = nodes_[cur].children[idx];
            nodes_[cur].prefix_count--;
        }
    }
}

bool Trie::search(std::string_view word) const {
    int node = traverse(word);
    return node != -1 && nodes_[node].is_end;
}

bool Trie::starts_with(std::string_view prefix) const {
    int node = traverse(prefix);
    return node != -1 && nodes_[node].prefix_count > 0;
}

bool Trie::erase(std::string_view word) {
    // First verify the word exists.
    if (!search(word)) {
        return false;
    }

    int cur = 0;
    nodes_[cur].prefix_count--;
    for (char c : word) {
        int idx = char_to_idx(c);
        cur = nodes_[cur].children[idx];
        nodes_[cur].prefix_count--;
    }
    nodes_[cur].is_end = false;
    --word_count_;
    return true;
}

int Trie::count_with_prefix(std::string_view prefix) const {
    int node = traverse(prefix);
    if (node == -1) {
        return 0;
    }
    return nodes_[node].prefix_count;
}

std::vector<std::string> Trie::words_with_prefix(std::string_view prefix) const {
    std::vector<std::string> result;
    int node = traverse(prefix);
    if (node == -1) {
        return result;
    }
    std::string current(prefix);
    collect(node, current, result);
    std::sort(result.begin(), result.end());
    return result;
}

std::string Trie::longest_prefix_of(std::string_view word) const {
    int cur = 0;
    int last_end = -1; // length of the longest prefix that is a word

    for (int i = 0; i < static_cast<int>(word.size()); ++i) {
        int idx = char_to_idx(word[i]);
        if (idx < 0 || nodes_[cur].children[idx] == -1) {
            break;
        }
        cur = nodes_[cur].children[idx];
        if (nodes_[cur].is_end) {
            last_end = i + 1;
        }
    }

    if (last_end == -1) {
        return {};
    }
    return std::string(word.substr(0, last_end));
}

int Trie::size() const {
    return word_count_;
}

bool Trie::empty() const {
    return word_count_ == 0;
}

void Trie::clear() {
    nodes_.clear();
    word_count_ = 0;
    nodes_.reserve(256);
    new_node(); // recreate root
}

// --- Trie private helpers ---------------------------------------------------

int Trie::new_node() {
    nodes_.emplace_back();
    return static_cast<int>(nodes_.size()) - 1;
}

int Trie::char_to_idx(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    return -1;
}

int Trie::traverse(std::string_view key) const {
    int cur = 0;
    for (char c : key) {
        int idx = char_to_idx(c);
        if (idx < 0 || nodes_[cur].children[idx] == -1) {
            return -1;
        }
        cur = nodes_[cur].children[idx];
    }
    return cur;
}

void Trie::collect(int node_idx, std::string& current, std::vector<std::string>& result) const {
    if (nodes_[node_idx].is_end) {
        result.push_back(current);
    }
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        int child = nodes_[node_idx].children[i];
        if (child != -1 && nodes_[child].prefix_count > 0) {
            current.push_back(static_cast<char>('a' + i));
            collect(child, current, result);
            current.pop_back();
        }
    }
}

} // namespace ds::trie
