#include "algorithms/graphs/dinic/dinic.h"

#include <gtest/gtest.h>
#include <random>
#include <set>

using namespace algorithms::graphs::dinic;

namespace {

// Helper to create a simple flow network
FlowNetwork make_simple_network() {
    // Network:
    //     10
    // 0 -----> 1
    // |        |
    // 10       10
    // v   10   v
    // 2 -----> 3
    FlowNetwork network(4);
    network.add_edge(0, 1, 10);
    network.add_edge(0, 2, 10);
    network.add_edge(1, 3, 10);
    network.add_edge(2, 3, 10);
    return network;
}

// Classic max flow example from textbooks
FlowNetwork make_clrs_example() {
    // CLRS Figure 26.1 style network
    //       16         12
    //   s -----> 1 --------> t
    //   | \      |^         ^|
    // 13|  \4    |9\10     / |
    //   v   v    v  \     /  |
    //   2 ----> 3 ----> 4    |
    //      14       7   \20  |14
    //                    \   |
    //                     \->5
    FlowNetwork network(6);
    // s = 0, t = 5
    network.add_edge(0, 1, 16);
    network.add_edge(0, 2, 13);
    network.add_edge(1, 2, 10);
    network.add_edge(2, 1, 4);
    network.add_edge(1, 3, 12);
    network.add_edge(2, 4, 14);
    network.add_edge(3, 2, 9);
    network.add_edge(3, 5, 20);
    network.add_edge(4, 3, 7);
    network.add_edge(4, 5, 4);
    return network;
}

} // namespace

// Basic functionality tests
TEST(Dinic, SimpleNetwork) {
    auto network = make_simple_network();
    auto flow = network.max_flow(0, 3);
    EXPECT_EQ(flow, 20);
}

TEST(Dinic, CLRSExample) {
    auto network = make_clrs_example();
    auto flow = network.max_flow(0, 5);
    EXPECT_EQ(flow, 23);
}

// Edge case: single edge
TEST(Dinic, SingleEdge) {
    FlowNetwork network(2);
    network.add_edge(0, 1, 42);
    EXPECT_EQ(network.max_flow(0, 1), 42);
}

// Edge case: no path exists
TEST(Dinic, NoPath) {
    FlowNetwork network(3);
    network.add_edge(0, 1, 10);
    // No edge to vertex 2
    EXPECT_EQ(network.max_flow(0, 2), 0);
}

// Edge case: source equals sink
TEST(Dinic, SourceEqualsSink) {
    FlowNetwork network(3);
    network.add_edge(0, 1, 10);
    network.add_edge(1, 2, 10);
    EXPECT_EQ(network.max_flow(0, 0), 0);
}

// Multiple paths with different capacities
TEST(Dinic, MultiplePathsDifferentCapacities) {
    // Three parallel paths from 0 to 3
    //   /--5--1--5--\
    // 0 ---10-2-10--- 3
    //   \--3--4--3--/
    FlowNetwork network(5);
    network.add_edge(0, 1, 5);
    network.add_edge(1, 3, 5);
    network.add_edge(0, 2, 10);
    network.add_edge(2, 3, 10);
    network.add_edge(0, 4, 3);
    network.add_edge(4, 3, 3);
    EXPECT_EQ(network.max_flow(0, 3), 18);
}

// Test bottleneck at source
TEST(Dinic, BottleneckAtSource) {
    FlowNetwork network(3);
    network.add_edge(0, 1, 5);   // Bottleneck
    network.add_edge(1, 2, 100);
    EXPECT_EQ(network.max_flow(0, 2), 5);
}

// Test bottleneck at sink
TEST(Dinic, BottleneckAtSink) {
    FlowNetwork network(3);
    network.add_edge(0, 1, 100);
    network.add_edge(1, 2, 5);   // Bottleneck
    EXPECT_EQ(network.max_flow(0, 2), 5);
}

// Diamond graph - tests flow splitting
TEST(Dinic, DiamondGraph) {
    //       a
    //      / \
    //     5   5
    //    /     \
    //   s       t
    //    \     /
    //     5   5
    //      \ /
    //       b
    FlowNetwork network(4);
    // s=0, a=1, b=2, t=3
    network.add_edge(0, 1, 5);
    network.add_edge(0, 2, 5);
    network.add_edge(1, 3, 5);
    network.add_edge(2, 3, 5);
    EXPECT_EQ(network.max_flow(0, 3), 10);
}

// Test with anti-parallel edges
TEST(Dinic, AntiParallelEdges) {
    FlowNetwork network(2);
    network.add_edge(0, 1, 10);
    network.add_edge(1, 0, 5);  // Anti-parallel
    EXPECT_EQ(network.max_flow(0, 1), 10);
}

// Linear chain
TEST(Dinic, LinearChain) {
    FlowNetwork network(5);
    network.add_edge(0, 1, 10);
    network.add_edge(1, 2, 8);
    network.add_edge(2, 3, 12);
    network.add_edge(3, 4, 6);  // Bottleneck
    EXPECT_EQ(network.max_flow(0, 4), 6);
}

// Test the convenience function
TEST(Dinic, ConvenienceFunction) {
    std::vector<std::tuple<int32_t, int32_t, int32_t>> edges = {
        {0, 1, 10},
        {0, 2, 10},
        {1, 3, 10},
        {2, 3, 10},
    };
    EXPECT_EQ(compute_max_flow(4, edges, 0, 3), 20);
}

// Complex network - bipartite matching reducible
TEST(Dinic, BipartiteMatchingNetwork) {
    // Model bipartite matching as max flow
    // Left: {1, 2}, Right: {3, 4}, Source: 0, Sink: 5
    // Edges: 0->left (cap 1), left->right (cap 1), right->5 (cap 1)
    FlowNetwork network(6);
    // Source to left partition
    network.add_edge(0, 1, 1);
    network.add_edge(0, 2, 1);
    // Matching edges (left to right)
    network.add_edge(1, 3, 1);
    network.add_edge(1, 4, 1);
    network.add_edge(2, 3, 1);
    // Right partition to sink
    network.add_edge(3, 5, 1);
    network.add_edge(4, 5, 1);
    // Max matching size = 2
    EXPECT_EQ(network.max_flow(0, 5), 2);
}

// Test flow values on individual edges
TEST(Dinic, GetFlowOnEdges) {
    FlowNetwork network(3);
    network.add_edge(0, 1, 10);
    network.add_edge(1, 2, 5);
    (void)network.max_flow(0, 2);

    EXPECT_EQ(network.get_flow(0, 1), 5);  // 5 units flow through
    EXPECT_EQ(network.get_flow(1, 2), 5);  // 5 units flow through
}

// Stress test with larger network
TEST(Dinic, LargerNetwork) {
    const int32_t n = 100;
    FlowNetwork network(n);

    // Create a layered network
    // Layer 0: source (0)
    // Layer 1: vertices 1..10
    // Layer 2: vertices 11..20
    // Layer 3: sink (99)

    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<int32_t> cap_dist(1, 100);

    // Source to layer 1
    for (int32_t i = 1; i <= 10; ++i) {
        network.add_edge(0, i, cap_dist(gen));
    }

    // Layer 1 to layer 2
    for (int32_t i = 1; i <= 10; ++i) {
        for (int32_t j = 11; j <= 20; ++j) {
            if (gen() % 3 == 0) {  // Sparse connections
                network.add_edge(i, j, cap_dist(gen));
            }
        }
    }

    // Layer 2 to sink
    for (int32_t i = 11; i <= 20; ++i) {
        network.add_edge(i, 99, cap_dist(gen));
    }

    auto flow = network.max_flow(0, 99);
    EXPECT_GT(flow, 0);  // Should find some flow
}

// Test network with zero capacity edges (should not contribute to flow)
TEST(Dinic, ZeroCapacityEdges) {
    FlowNetwork network(3);
    network.add_edge(0, 1, 10);
    network.add_edge(1, 2, 0);  // Zero capacity
    EXPECT_EQ(network.max_flow(0, 2), 0);
}

// Test multiple runs on same network (should return same result)
TEST(Dinic, MultipleRuns) {
    auto network1 = make_simple_network();
    auto network2 = make_simple_network();

    auto flow1 = network1.max_flow(0, 3);
    auto flow2 = network2.max_flow(0, 3);

    EXPECT_EQ(flow1, flow2);
    EXPECT_EQ(flow1, 20);
}

// Network with cycles
TEST(Dinic, NetworkWithCycles) {
    FlowNetwork network(4);
    network.add_edge(0, 1, 10);
    network.add_edge(1, 2, 10);
    network.add_edge(2, 1, 5);  // Creates cycle
    network.add_edge(2, 3, 10);
    EXPECT_EQ(network.max_flow(0, 3), 10);
}

// Empty network (single vertex)
TEST(Dinic, EmptyNetwork) {
    FlowNetwork network(1);
    EXPECT_EQ(network.max_flow(0, 0), 0);
}

// Two vertices, no edges
TEST(Dinic, TwoVerticesNoEdges) {
    FlowNetwork network(2);
    EXPECT_EQ(network.max_flow(0, 1), 0);
}

// Test network size getter
TEST(Dinic, NetworkSize) {
    FlowNetwork network(10);
    EXPECT_EQ(network.size(), 10);
}

// Disconnected components (should find flow only if path exists)
TEST(Dinic, DisconnectedComponents) {
    FlowNetwork network(6);
    // Component 1: 0 -> 1 -> 2
    network.add_edge(0, 1, 10);
    network.add_edge(1, 2, 10);
    // Component 2: 3 -> 4 -> 5 (disconnected from 0)
    network.add_edge(3, 4, 10);
    network.add_edge(4, 5, 10);

    EXPECT_EQ(network.max_flow(0, 2), 10);  // Within component 1
    EXPECT_EQ(network.max_flow(3, 5), 10);  // Within component 2 (new network needed)
}

// Classic example: maximum flow equals minimum cut
TEST(Dinic, MaxFlowMinCut) {
    // Network where min-cut is obvious
    //        5
    //    s ------ a
    //    |        |
    //    5        5
    //    |        |
    //    b ------ t
    //        5
    FlowNetwork network(4);
    network.add_edge(0, 1, 5);   // s -> a
    network.add_edge(0, 2, 5);   // s -> b
    network.add_edge(1, 3, 5);   // a -> t
    network.add_edge(2, 3, 5);   // b -> t

    // Min cut = {(s,a), (s,b)} or {(a,t), (b,t)} = 10
    EXPECT_EQ(network.max_flow(0, 3), 10);
}

// Dinic-specific: test that the algorithm handles unit-capacity networks efficiently
TEST(Dinic, UnitCapacityBipartite) {
    // Larger bipartite matching: 5 left nodes, 5 right nodes
    // Source: 0, Left: 1-5, Right: 6-10, Sink: 11
    FlowNetwork network(12);

    // Source to left partition (unit capacity)
    for (int32_t i = 1; i <= 5; ++i) {
        network.add_edge(0, i, 1);
    }

    // Matching edges (unit capacity): perfect matching exists
    network.add_edge(1, 6, 1);
    network.add_edge(2, 7, 1);
    network.add_edge(3, 8, 1);
    network.add_edge(4, 9, 1);
    network.add_edge(5, 10, 1);
    // Extra cross-edges
    network.add_edge(1, 7, 1);
    network.add_edge(2, 8, 1);
    network.add_edge(3, 6, 1);

    // Right partition to sink (unit capacity)
    for (int32_t i = 6; i <= 10; ++i) {
        network.add_edge(i, 11, 1);
    }

    EXPECT_EQ(network.max_flow(0, 11), 5);
}

// Test with a complex layered DAG
TEST(Dinic, LayeredDAG) {
    // 4-layer network: source -> layer1(3 nodes) -> layer2(3 nodes) -> sink
    FlowNetwork network(8);
    // s=0, layer1={1,2,3}, layer2={4,5,6}, t=7
    network.add_edge(0, 1, 10);
    network.add_edge(0, 2, 10);
    network.add_edge(0, 3, 10);
    network.add_edge(1, 4, 4);
    network.add_edge(1, 5, 6);
    network.add_edge(2, 4, 5);
    network.add_edge(2, 6, 5);
    network.add_edge(3, 5, 3);
    network.add_edge(3, 6, 7);
    network.add_edge(4, 7, 10);
    network.add_edge(5, 7, 10);
    network.add_edge(6, 7, 10);

    // Max flow is limited by internal edge capacities
    auto flow = network.max_flow(0, 7);
    EXPECT_EQ(flow, 28);
}

