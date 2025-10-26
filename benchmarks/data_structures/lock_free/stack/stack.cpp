#include "data_structures/lock_free/stack/stack.h"

#include <atomic>
#include <benchmark/benchmark.h>
#include <numeric>
#include <thread>
#include <vector>

using namespace data_structures::lock_free::stack;

// Single-threaded push/pop benchmark for a Stack type
template <typename StackT> static void BM_SingleThread_PushPop(benchmark::State& state) {
    const int N = static_cast<int>(state.range(0));
    for (auto _ : state) {
        StackT s;
        // push N items
        for (int i = 0; i < N; ++i)
            s.push(i);
        // pop N items
        int cnt = 0;
        while (true) {
            auto v = s.pop();
            if (!v)
                break;
            ++cnt;
        }
        benchmark::DoNotOptimize(cnt);
    }
}

// Multi-producer / multi-consumer stress workload
// state.range(0) = producers
// state.range(1) = per_producer
// state.range(2) = consumers
template <typename StackT> static void BM_MPMC_Workload(benchmark::State& state) {
    const int producers = static_cast<int>(state.range(0));
    const int per_producer = static_cast<int>(state.range(1));
    int consumers = static_cast<int>(state.range(2));
    if (consumers <= 0)
        consumers = producers;

    const int total = producers * per_producer;

    for (auto _ : state) {
        // Setup
        std::atomic<int> produced{0};
        std::atomic<int> consumed{0};
        std::atomic<long long> sum{0};
        StackT st;

        // Prepare producer threads
        std::vector<std::thread> producers_t;
        producers_t.reserve(producers);
        // Prepare consumer threads
        std::vector<std::thread> consumers_t;
        consumers_t.reserve(consumers);

        state.PauseTiming();
        // Nothing heavy to setup besides vectors; start timing for workload
        state.ResumeTiming();

        // Launch producer threads
        for (int p = 0; p < producers; ++p) {
            producers_t.emplace_back([p, per_producer, &st, &produced]() {
                int base = p * per_producer;
                for (int i = 0; i < per_producer; ++i) {
                    st.push(base + i);
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }

        // Launch consumer threads
        for (int c = 0; c < consumers; ++c) {
            consumers_t.emplace_back([&st, &consumed, &sum, total]() {
                while (consumed.load(std::memory_order_relaxed) < total) {
                    auto v = st.pop();
                    if (v) {
                        sum.fetch_add(static_cast<long long>(*v), std::memory_order_relaxed);
                        consumed.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        std::this_thread::yield();
                    }
                }
            });
        }

        // Wait for producers
        for (auto& t : producers_t)
            t.join();
        // Wait for consumers
        for (auto& t : consumers_t)
            t.join();

        state.PauseTiming();
        // Sanity-checks in teardown (not measured)
        benchmark::DoNotOptimize(produced.load());
        benchmark::DoNotOptimize(consumed.load());
        benchmark::DoNotOptimize(sum.load());
        state.ResumeTiming();

        // Nothing else to do for this iteration
    }
}

// Register benchmarks
// Single-threaded cases
BENCHMARK_TEMPLATE(BM_SingleThread_PushPop, LockFreeStack<int>)->Arg(100000)->Arg(500000);
BENCHMARK_TEMPLATE(BM_SingleThread_PushPop, LockBasedStack<int>)->Arg(100000)->Arg(500000);

// MPMC cases: 4 producers x 4 consumers x 2500 items each => 10000 total
BENCHMARK_TEMPLATE(BM_MPMC_Workload, LockFreeStack<int>)->Args({4, 2500, 4})->Iterations(10);
BENCHMARK_TEMPLATE(BM_MPMC_Workload, LockBasedStack<int>)->Args({4, 2500, 4})->Iterations(10);

// Expose a dense/large MPMC scenario if desired
BENCHMARK_TEMPLATE(BM_MPMC_Workload, LockFreeStack<int>)
    ->Args({std::thread::hardware_concurrency(), 100000, std::thread::hardware_concurrency()})
    ->Iterations(10);
BENCHMARK_TEMPLATE(BM_MPMC_Workload, LockBasedStack<int>)
    ->Args({std::thread::hardware_concurrency(), 100000, std::thread::hardware_concurrency()})
    ->Iterations(10);

BENCHMARK_MAIN();
