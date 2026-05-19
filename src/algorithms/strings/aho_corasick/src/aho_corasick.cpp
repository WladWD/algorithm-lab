#include "algorithms/strings/aho_corasick/aho_corasick.h"

#include <algorithm>
#include <queue>

namespace algorithms::strings::aho_corasick {

int AhoCorasick::new_node() {
    trie_.emplace_back();
    return static_cast<int>(trie_.size()) - 1;
}

std::size_t AhoCorasick::add_pattern(std::string_view pattern) {
    if (trie_.empty()) new_node(); // root = 0
    built_ = false;

    int cur = 0;
    for (unsigned char c : pattern) {
        if (trie_[static_cast<std::size_t>(cur)].next[c] == -1) {
            trie_[static_cast<std::size_t>(cur)].next[c] = new_node();
        }
        cur = trie_[static_cast<std::size_t>(cur)].next[c];
    }

    if (trie_[static_cast<std::size_t>(cur)].output == -1) {
        trie_[static_cast<std::size_t>(cur)].output = static_cast<int>(patterns_.size());
    }
    // Note: if two identical patterns are added, the first wins for output;
    // duplicate tracking could be extended but is out of scope here.

    patterns_.emplace_back(pattern);
    return patterns_.size() - 1;
}

void AhoCorasick::build() {
    if (trie_.empty()) { new_node(); }
    built_ = true;

    // BFS to compute failure links (similar to KMP's prefix function).
    std::queue<int> q;

    // Children of root: failure link = root.
    for (int c = 0; c < kAlpha; ++c) {
        int ch = trie_[0].next[c];
        if (ch == -1) {
            trie_[0].next[c] = 0; // self-loop at root for missing chars
        } else {
            trie_[static_cast<std::size_t>(ch)].fail = 0;
            q.push(ch);
        }
    }

    while (!q.empty()) {
        int v = q.front(); q.pop();
        // Dictionary link: nearest proper suffix that is a pattern end.
        int f = trie_[static_cast<std::size_t>(v)].fail;
        if (trie_[static_cast<std::size_t>(f)].output != -1) {
            trie_[static_cast<std::size_t>(v)].dict = f;
        } else {
            trie_[static_cast<std::size_t>(v)].dict = trie_[static_cast<std::size_t>(f)].dict;
        }

        for (int c = 0; c < kAlpha; ++c) {
            int ch = trie_[static_cast<std::size_t>(v)].next[c];
            if (ch == -1) {
                // Shortcut: follow failure link.
                trie_[static_cast<std::size_t>(v)].next[c] =
                    trie_[static_cast<std::size_t>(f)].next[c];
            } else {
                trie_[static_cast<std::size_t>(ch)].fail =
                    trie_[static_cast<std::size_t>(f)].next[c];
                q.push(ch);
            }
        }
    }
}

std::vector<Match> AhoCorasick::search(std::string_view text) const {
    std::vector<Match> result;
    if (trie_.empty()) return result;

    int cur = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        cur = trie_[static_cast<std::size_t>(cur)].next[c];

        // Collect all patterns ending at position i.
        int v = cur;
        while (v > 0 || trie_[static_cast<std::size_t>(v)].output != -1) {
            if (trie_[static_cast<std::size_t>(v)].output != -1) {
                int pid = trie_[static_cast<std::size_t>(v)].output;
                result.push_back({i, static_cast<std::size_t>(pid)});
            }
            int d = trie_[static_cast<std::size_t>(v)].dict;
            if (d == -1) break;
            v = d;
        }
    }

    std::sort(result.begin(), result.end(), [](const Match& a, const Match& b) {
        return a.end_pos != b.end_pos ? a.end_pos < b.end_pos : a.pattern_id < b.pattern_id;
    });
    return result;
}

} // namespace algorithms::strings::aho_corasick

