# Debugging — quick guide

The repo has three CMake presets defined in `CMakePresets.json`:
- `dev` → `out/build/dev` (Debug, tests ON, bench ON)
- `sanitizers` → `out/build/sanitizers` (inherits `dev`, enables Address/UB sanitizers on Clang/GCC)
- `release` → `out/build/release` (Release, benchmarks ON, tests OFF, LTO if available)

# CLion
- CLion understands `CMakePresets.json` automatically. Open the project folder.
- In Settings | Build, Execution, Deployment | CMake select the preset you want (`dev`, `sanitizers` or `release`) and apply. CLion will configure into the preset's binary dir (e.g. `out/build/dev`).
- Build the target you want (right-click target or use the Targets tool window). Typical targets: `all`, specific library, test executables under `tests/`, or benchmark binaries under `benchmarks/`.
- Create a Run/Debug configuration: choose "Executable" and point the target executable (from the build output). Set working directory and environment variables as needed.
- For sanitizers use the `sanitizers` preset. Note: sanitizers are added by CMake only on Clang/GCC (see top-level `CMakeLists.txt`); on macOS use CLion's bundled toolchain that supports AppleClang/LLDB for best compatibility.
- Useful: point CLion to `out/build/<preset>/compile_commands.json` for accurate indexing if needed.

# Xcode (macOS)
- Generate an Xcode project with CMake (example using `dev` flags):

```bash
cmake -S . -B out/build/xcode -G Xcode -DCMAKE_BUILD_TYPE=Debug -DALGO_ENABLE_TESTS=ON -DALGO_ENABLE_BENCH=ON
# or explicitly enable sanitizers
cmake -S . -B out/build/xcode -G Xcode -DCMAKE_BUILD_TYPE=Debug -DALGO_ENABLE_SANITIZERS=ON
```

- Open `out/build/xcode/YourProject.xcodeproj` in Xcode.
- Select the executable scheme (tests or a specific binary) and set the scheme's Run arguments / environment as required.
- If you built with sanitizers via `-DALGO_ENABLE_SANITIZERS=ON`, the compile/link flags are already injected by CMake. You can also enable Xcode runtime diagnostics (Address/Thread sanitizers) in the scheme diagnostics for an additional safety net.
- Note: CMake preset `sanitizers` in this repo uses `-fsanitize=address,undefined` only when the compiler is Clang/AppleClang/GNU; MSVC doesn't get these flags.

# Visual Studio (Windows)
- Visual Studio (2019/2022) supports CMakePresets. Open the folder in Visual Studio (File → Open → Folder) and Visual Studio will detect `CMakePresets.json`.
- Choose the preset (Configuration dropdown) that matches `dev` / `release` / `sanitizers` and press Configure. Build the desired target from the CMake Targets View and set it as the startup item.
- Use Visual Studio's debugger as usual. Note: Address/UB sanitizers configured by this repo are only applied for Clang/GNU toolchains; MSVC does not use `-fsanitize` flags — sanitizer preset may be no-op on MSVC.

# General notes & tips
- Binaries and compile commands: built artifacts live under `out/build/<preset>/`. `compile_commands.json` is generated in the build dir — point your IDE to it if it does not pick it up automatically.
- Debugging tests: ensure tests are built (`ALGO_ENABLE_TESTS=ON` in `dev` or `sanitizers`) and run the test executable directly under the IDE for interactive debugging.
- Reproducible perf/debug runs: use the `release` preset to test performance; use `sanitizers` preset to catch memory/UB issues in Debug.
- If you need a CMake generator other than Ninja (e.g., Xcode), pass equivalent cache variables shown above when configuring.

# Troubleshooting
- "Target not found" — verify you opened the right build dir/preset and that the target is enabled in `out/build/<preset>/`.
- "Sanitizers not enabled" — confirm you used the `sanitizers` preset or passed `-DALGO_ENABLE_SANITIZERS=ON` and use Clang/GNU toolchain.
- Debugger crashes with sanitizer hits — reproduce the run in the terminal first to capture sanitizer logs, then attach the debugger to the process if needed.

That's it — pick the preset (`dev` / `sanitizers` / `release`) appropriate for your session and open the project in your IDE. 