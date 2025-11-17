#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>

struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
};

struct ParticlesSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    ParticlesSoA(size_t n = 0) { resize(n); }
    void resize(size_t n) {
        x.assign(n, 0.0f);
        y.assign(n, 0.0f);
        z.assign(n, 0.0f);
        vx.assign(n, 0.0f);
        vy.assign(n, 0.0f);
        vz.assign(n, 0.0f);
    }
    size_t size() const { return x.size(); }
};

// initialize both layouts with the same pseudo-random data
void init_data(std::vector<ParticleAoS> &aos, ParticlesSoA &soa, size_t n, uint64_t seed) {
    aos.resize(n);
    soa.resize(n);
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<float> dist(-100.0f, 100.0f);
    for (size_t i = 0; i < n; ++i) {
        float x = dist(rng);
        float y = dist(rng);
        float z = dist(rng);
        float vx = dist(rng) * 0.01f;
        float vy = dist(rng) * 0.01f;
        float vz = dist(rng) * 0.01f;
        aos[i].x = x; aos[i].y = y; aos[i].z = z;
        aos[i].vx = vx; aos[i].vy = vy; aos[i].vz = vz;
        soa.x[i] = x; soa.y[i] = y; soa.z[i] = z;
        soa.vx[i] = vx; soa.vy[i] = vy; soa.vz[i] = vz;
    }
}

// kernels
inline float sum_x_aos(const std::vector<ParticleAoS> &aos) {
    float s = 0.0f;
    size_t n = aos.size();
    for (size_t i = 0; i < n; ++i) s += aos[i].x;
    return s;
}

inline float sum_x_soa(const ParticlesSoA &soa) {
    float s = 0.0f;
    size_t n = soa.x.size();
    for (size_t i = 0; i < n; ++i) s += soa.x[i];
    return s;
}

inline void update_x_aos(std::vector<ParticleAoS> &aos, float dt) {
    size_t n = aos.size();
    for (size_t i = 0; i < n; ++i) {
        aos[i].x += aos[i].vx * dt;
    }
}

inline void update_x_soa(ParticlesSoA &soa, float dt) {
    size_t n = soa.x.size();
    for (size_t i = 0; i < n; ++i) {
        soa.x[i] += soa.vx[i] * dt;
    }
}

inline void update_all_aos(std::vector<ParticleAoS> &aos, float dt) {
    size_t n = aos.size();
    for (size_t i = 0; i < n; ++i) {
        aos[i].x += aos[i].vx * dt;
        aos[i].y += aos[i].vy * dt;
        aos[i].z += aos[i].vz * dt;
    }
}

inline void update_all_soa(ParticlesSoA &soa, float dt) {
    size_t n = soa.x.size();
    for (size_t i = 0; i < n; ++i) {
        soa.x[i] += soa.vx[i] * dt;
        soa.y[i] += soa.vy[i] * dt;
        soa.z[i] += soa.vz[i] * dt;
    }
}

// Google Benchmark wrappers
static void BM_SumX_AoS(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    std::vector<ParticleAoS> aos;
    ParticlesSoA soa; // only to keep signature of init_data
    for (auto _ : state) {
        state.PauseTiming();
        init_data(aos, soa, n, 12345);
        state.ResumeTiming();
        float s = sum_x_aos(aos);
        benchmark::DoNotOptimize(s);
    }
    state.SetItemsProcessed(state.iterations() * int64_t(n));
}

static void BM_SumX_SoA(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    std::vector<ParticleAoS> aos;
    ParticlesSoA soa;
    for (auto _ : state) {
        state.PauseTiming();
        init_data(aos, soa, n, 12345);
        state.ResumeTiming();
        float s = sum_x_soa(soa);
        benchmark::DoNotOptimize(s);
    }
    state.SetItemsProcessed(state.iterations() * int64_t(n));
}

static void BM_UpdateX_AoS(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    std::vector<ParticleAoS> aos;
    ParticlesSoA soa;
    const float dt = 0.016f;
    for (auto _ : state) {
        state.PauseTiming();
        init_data(aos, soa, n, 12345);
        state.ResumeTiming();
        update_x_aos(aos, dt);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * int64_t(n));
}

static void BM_UpdateX_SoA(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    std::vector<ParticleAoS> aos;
    ParticlesSoA soa;
    const float dt = 0.016f;
    for (auto _ : state) {
        state.PauseTiming();
        init_data(aos, soa, n, 12345);
        state.ResumeTiming();
        update_x_soa(soa, dt);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * int64_t(n));
}

static void BM_UpdateAll_AoS(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    std::vector<ParticleAoS> aos;
    ParticlesSoA soa;
    const float dt = 0.016f;
    for (auto _ : state) {
        state.PauseTiming();
        init_data(aos, soa, n, 12345);
        state.ResumeTiming();
        update_all_aos(aos, dt);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * int64_t(n) * 3); // 3 updates per item
}

static void BM_UpdateAll_SoA(benchmark::State& state) {
    const size_t n = static_cast<size_t>(state.range(0));
    std::vector<ParticleAoS> aos;
    ParticlesSoA soa;
    const float dt = 0.016f;
    for (auto _ : state) {
        state.PauseTiming();
        init_data(aos, soa, n, 12345);
        state.ResumeTiming();
        update_all_soa(soa, dt);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * int64_t(n) * 3);
}

// Register benchmarks with several sizes
BENCHMARK(BM_SumX_AoS)->Arg(100000)->Arg(1000000)->Arg(10000000);
BENCHMARK(BM_SumX_SoA)->Arg(100000)->Arg(1000000)->Arg(10000000);
BENCHMARK(BM_UpdateX_AoS)->Arg(100000)->Arg(1000000)->Arg(10000000);
BENCHMARK(BM_UpdateX_SoA)->Arg(100000)->Arg(1000000)->Arg(10000000);
BENCHMARK(BM_UpdateAll_AoS)->Arg(100000)->Arg(1000000)->Arg(10000000);
BENCHMARK(BM_UpdateAll_SoA)->Arg(100000)->Arg(1000000)->Arg(10000000);

BENCHMARK_MAIN();

