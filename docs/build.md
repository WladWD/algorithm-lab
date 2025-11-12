# Build (CMake) — Algorithm Lab

Prerequisites
- CMake 3.19+ (presets support)
- Ninja (recommended generator)
- A C++20/C++23-capable toolchain (clang or gcc)
- clang-format (for automatic formatting)

CMake presets included (authoritative)
- `dev`
  - Generator: Ninja
  - Binary dir: `out/build/dev`
  - Config: `CMAKE_BUILD_TYPE=Debug`, `ALGO_ENABLE_TESTS=ON`, `ALGO_ENABLE_BENCH=ON`
- `sanitizers`
  - Inherits `dev`
  - Binary dir: `out/build/sanitizers`
  - Enables sanitizers via `ALGO_ENABLE_SANITIZERS=ON`
- `release`
  - Generator: Ninja
  - Binary dir: `out/build/release`
  - Config: `CMAKE_BUILD_TYPE=Release`, `ALGO_ENABLE_TESTS=OFF`, `ALGO_ENABLE_BENCH=ON`, `ALGO_ENABLE_LTO=ON`

# Configure and build (dev preset — Debug, tests + benchmarks)
```bash
cmake --preset dev
cmake --build --preset dev -j
```

# Build release (Release, LTO enabled, tests disabled)
```bash
cmake --preset release
cmake --build --preset release -j
```

# Build sanitizers (Debug + Address/UB sanitizers)
```bash
cmake --preset sanitizers
cmake --build --preset sanitizers -j
```

# Run clang-format
Format in-place:
```bash
# configure (ensures the format target is available)
cmake --preset dev

# run the format target to apply formatting to all tracked source files
cmake --build --preset dev --target format
```

Check formatting (dry-run, fails if anything is unformatted):
```bash
cmake --preset dev
cmake --build --preset dev --target format-check
```

The build artifacts are placed under `out/build/<preset>/`, e.g., `out/build/dev/algorithms/` for algorithm binaries.