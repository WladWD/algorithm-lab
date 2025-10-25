    add_executable(bench_data_structures_lock_free_stack stack.cpp)
    target_link_libraries(bench_data_structures_lock_free_stack PRIVATE
            data_structures::lock_free::stack
            benchmark::benchmark
            benchmark::benchmark_main
    )