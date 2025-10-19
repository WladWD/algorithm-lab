# Build (CMake) — Algorithm Lab

This document contains step-by-step build instructions, CMake presets, and formatting tips for the repository.

Prerequisites
- CMake 3.19+ with presets support
- Ninja (recommended by the provided presets)
- A C++20-capable toolchain (clang or GCC)
- clang-format (for formatting)

CMake presets (provided)
- `dev` — RelWithDebInfo, tests and benchmarks enabled
  - binary dir: `out/build/dev`
- `sanitizers` — inherits `dev`, enables sanitizers and Debug build
  - binary dir: `out/build/sanitizers`
- `release` — Release build, LTO enabled, tests disabled
  - binary dir: `out/build/release`

Configure & build (common workflows)

# Configure and build (default dev preset)
```bash
cmake --preset dev
cmake --build --preset dev -j
```

# Build release
```bash
cmake --preset release
cmake --build --preset release -j
```

# Build sanitizers (Address/UBSan/Leak)
```bash
cmake --preset sanitizers
cmake --build --preset sanitizers -j
```

Running tests

The presets include a `testPresets` entry. To run all tests (dev preset):

```bash
ctest --preset dev --output-on-failure
```

You can also run individual test binaries under `out/build/<preset>/...` directly.

Formatting (clang-format)

We recommend using a repository-level `.clang-format` file to keep a consistent style. Use the following commands:

# Install (macOS)
```bash
brew install clang-format
```

# Format the entire repository (macOS / zsh)
```bash
find . -type f \( -iname "*.cpp" -o -iname "*.cc" -o -iname "*.cxx" -o -iname "*.c" -o -iname "*.h" -o -iname "*.hpp" -o -iname "*.hxx" \) \
  -not -path "./build/*" -not -path "./out/*" -not -path "./.git/*" -print0 \
  | xargs -0 clang-format -i -style=file
```

# Format only changes vs a base branch (fast)
```bash
git-clang-format origin/main..HEAD
```

# Format staged files (before commit)
```bash
git-clang-format --staged
```

Optional: pre-commit hook

Create `.git/hooks/pre-commit` with the provided snippet in `README.md` to format staged C/C++ files automatically (make it executable).

Notes
- Binary output directories are under `out/build/<preset>` per `CMakePresets.json`.
- If you need compile commands for tools like clangd, enable/export `compile_commands.json` by configuring with a preset and copying `out/build/<preset>/compile_commands.json` to the repo root or pointing your editor to the build dir.
- If you modify presets, update the `CMakePresets.json` accordingly.

