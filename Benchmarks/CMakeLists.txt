add_executable(ScalarBenchmarks ScalarBenchmarks.cpp)
target_link_libraries(ScalarBenchmarks PRIVATE ShapeEngine)
if(NOT MSVC)
    target_link_options(ScalarBenchmarks PRIVATE -static)
endif()

add_executable(Vector3Benchmarks Vector3Benchmarks.cpp)
target_link_libraries(Vector3Benchmarks PRIVATE ShapeEngine)
if(NOT MSVC)
    target_link_options(Vector3Benchmarks PRIVATE -static)
endif()

add_executable(Matrix2Benchmarks Matrix2Benchmarks.cpp)
target_link_libraries(Matrix2Benchmarks PRIVATE ShapeEngine)
if(NOT MSVC)
    target_link_options(Matrix2Benchmarks PRIVATE -static)
endif()

add_executable(QuaternionBenchmarks QuaternionBenchmarks.cpp)
target_link_libraries(QuaternionBenchmarks PRIVATE ShapeEngine)
if(NOT MSVC)
    target_link_options(QuaternionBenchmarks PRIVATE -static)
endif()

add_executable(PointBenchmarks PointBenchmarks.cpp)
target_link_libraries(PointBenchmarks PRIVATE ShapeEngine)
if(NOT MSVC)
    target_link_options(PointBenchmarks PRIVATE -static)
endif()

# ─── Phase 3 Benchmarks ───────────────────────────────────────────────
add_executable(ecs_iteration_bench ecs_iteration_bench.cpp)
target_link_libraries(ecs_iteration_bench PRIVATE ShapeEngine)

add_executable(archetype_transition_bench archetype_transition_bench.cpp)
target_link_libraries(archetype_transition_bench PRIVATE ShapeEngine)

add_executable(spatial_query_bench spatial_query_bench.cpp)
target_link_libraries(spatial_query_bench PRIVATE ShapeEngine)

add_executable(full_sim_bench full_sim_bench.cpp)
target_link_libraries(full_sim_bench PRIVATE ShapeEngine)

add_executable(bench_runner bench_runner.cpp)
target_link_libraries(bench_runner PRIVATE ShapeEngine)
