cmake_minimum_required(VERSION 3.30)
project(algo-benchmarks LANGUAGES CXX)

# ---- Options ----
option(ALGO_ENABLE_TESTS "Build tests" ON)
option(ALGO_ENABLE_BENCH "Build benchmarks" ON)
option(ALGO_ENABLE_SANITIZERS "Enable Address/Undefined sanitizers" OFF)
option(ALGO_ENABLE_LTO   "Enable link-time optimization if available" ON)

# ---- C++ & warnings ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Handy warning setup
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- Dependencies (FetchContent) ----
include(FetchContent)

# GoogleTest
if (ALGO_ENABLE_TESTS)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.14.0
    )
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
endif()

# Google Benchmark
if (ALGO_ENABLE_BENCH)
    FetchContent_Declare(
            googlebenchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG        v1.8.4
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googlebenchmark)
endif()

# ---- (Optional) LTO ----
include(CheckIPOSupported)
if (ALGO_ENABLE_LTO)
    check_ipo_supported(RESULT lto_supported OUTPUT lto_msg)
    if (lto_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    endif()
endif()

# Sanitizer (for GCC/Clang)
if (ALGO_ENABLE_SANITIZERS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        message(STATUS "Sanitizers enabled: address, undefined")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# ---- Modules ----
add_subdirectory(algorithms/graphs/dijkstra)

if (ALGO_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

if (ALGO_ENABLE_BENCH)
    add_subdirectory(benchmarks)
endif()