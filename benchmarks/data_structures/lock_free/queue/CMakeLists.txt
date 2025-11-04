    add_executable(bench_data_structures_lock_free_queue queue.cpp)
    target_link_libraries(bench_data_structures_lock_free_queue PRIVATE
        data_structures::lock_free::queue
        benchmark::benchmark
        benchmark::benchmark_main
    )