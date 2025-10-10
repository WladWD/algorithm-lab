#include "algorithms/graphs/dijkstra/generator.h"

namespace algorithms::graphs::dijkstra {
Graph make_worstcase_graph(int V, int E, unsigned seed) {
    Graph g(V);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> d(1, 9);

    for (int i=0;i+1<V;++i) g[i].push_back({i+1, d(rng)});
    for (int i=0;i<E-(V-1);++i) {
        int u = rng()%V, v = rng()%V; if (u==v) v = (v+1)%V;
        g[u].push_back({v, d(rng)});
    }
    return g;
}

Graph make_demo_graph() {
    Graph g(4);
    g[0]={{1,1},{2,5}};
    g[1]={{2,1},{3,2}};
    g[2]={{3,1}};
    return g;
}
} // namespace algorithms::graphs::dijkstra