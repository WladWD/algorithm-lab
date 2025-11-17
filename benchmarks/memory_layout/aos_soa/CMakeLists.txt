add_executable(bench_data_memory_layout_soa_aos main.cpp)
target_link_libraries(bench_data_memory_layout_soa_aos PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
)

