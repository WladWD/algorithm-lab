#include "data_structures/lock_free/queue/queue.h"

#include <atomic>
#include <benchmark/benchmark.h>
#include <thread>
#include <vector>

using namespace data_structures::lock_free::queue;

// Single-thread push/pop benchmark
template <typename QueueT> static void BM_SingleThread_PushPop(benchmark::State& state) {
    const int N = static_cast<int>(state.range(0));
    for (auto _ : state) {
        QueueT q;
        for (int i = 0; i < N; ++i)
            q.push(i);
        int cnt = 0;
        while (true) {
            auto v = q.pop();
            if (!v)
                break;
            ++cnt;
        }
        benchmark::DoNotOptimize(cnt);
    }
}

// MPMC benchmark: producers, per_producer items, consumers
template <typename QueueT> static void BM_MPMC(benchmark::State& state) {
    const int producers = static_cast<int>(state.range(0));
    const int per_producer = static_cast<int>(state.range(1));
    const int consumers = static_cast<int>(state.range(2));
    const int total = producers * per_producer;

    for (auto _ : state) {
        state.PauseTiming();
        QueueT q;
        std::atomic<int> produced{0};
        std::atomic<int> consumed{0};
        std::atomic<long long> sum{0};
        std::vector<std::thread> prod_threads;
        std::vector<std::thread> cons_threads;
        prod_threads.reserve(producers);
        cons_threads.reserve(consumers);
        state.ResumeTiming();

        for (int p = 0; p < producers; ++p) {
            prod_threads.emplace_back([p, per_producer, &q, &produced]() {
                int base = p * per_producer;
                for (int i = 0; i < per_producer; ++i) {
                    q.push(base + i);
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }

        for (int c = 0; c < consumers; ++c) {
            cons_threads.emplace_back([&q, &consumed, &sum, total]() {
                while (consumed.load(std::memory_order_relaxed) < total) {
                    auto v = q.pop();
                    if (v) {
                        sum.fetch_add(*v, std::memory_order_relaxed);
                        consumed.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        std::this_thread::yield();
                    }
                }
            });
        }

        for (auto& t : prod_threads)
            t.join();
        for (auto& t : cons_threads)
            t.join();
        state.PauseTiming();
        benchmark::DoNotOptimize(produced.load());
        benchmark::DoNotOptimize(consumed.load());
        benchmark::DoNotOptimize(sum.load());
        state.ResumeTiming();
    }
}

// Register benchmarks
BENCHMARK_TEMPLATE(BM_SingleThread_PushPop, LockFreeQueue<int>)->Arg(100000)->Arg(500000);
BENCHMARK_TEMPLATE(BM_SingleThread_PushPop, LockBasedQueue<int>)->Arg(100000)->Arg(500000);

BENCHMARK_TEMPLATE(BM_MPMC, LockFreeQueue<int>)->Args({4, 2500, 4})->Iterations(5);
BENCHMARK_TEMPLATE(BM_MPMC, LockBasedQueue<int>)->Args({4, 2500, 4})->Iterations(5);

BENCHMARK_TEMPLATE(BM_MPMC, LockFreeQueue<int>)->Args({8, 10000, 8})->Iterations(3);
BENCHMARK_TEMPLATE(BM_MPMC, LockBasedQueue<int>)->Args({8, 10000, 8})->Iterations(3);

BENCHMARK_MAIN();
