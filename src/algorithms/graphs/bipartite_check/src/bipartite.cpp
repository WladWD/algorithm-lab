#include <algorithms/graphs/bipartite_check/bipartite.h>
#include <queue>
#include <stack>

namespace algorithms::graphs::bipartite_check {
OptionalColorResult bipartite_bfs(const AdjList& g) {
    int32_t n = (int32_t)g.size();
    ColorResult color(n, -1);
    std::queue<int32_t> q;

    for (int32_t start = 0; start < n; ++start) {
        if (color[start] != -1)
            continue;
        color[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int32_t v = q.front();
            q.pop();
            for (int32_t to : g[v]) {
                if (color[to] == -1) {
                    color[to] = color[v] ^ 1;
                    q.push(to);
                } else if (color[to] == color[v]) {
                    return std::nullopt; // same color on both endpoints -> not bipartite
                }
            }
        }
    }
    return color;
}

OptionalColorResult bipartite_dfs(const AdjList& g) {
    int32_t n = (int32_t)g.size();
    ColorResult color(n, -1);
    std::stack<int> st;

    for (int32_t start = 0; start < n; ++start) {
        if (color[start] != -1)
            continue;
        color[start] = 0;
        st.push(start);

        while (!st.empty()) {
            int32_t v = st.top();
            st.pop();
            for (int32_t to : g[v]) {
                if (color[to] == -1) {
                    color[to] = color[v] ^ 1;
                    st.push(to);
                } else if (color[to] == color[v]) {
                    return std::nullopt;
                }
            }
        }
    }
    return color;
}
} // namespace algorithms::graphs::bipartite_check
