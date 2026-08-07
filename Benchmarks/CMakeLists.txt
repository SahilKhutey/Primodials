add_executable(ScalarBenchmarks
    ScalarBenchmarks.cpp
)

target_link_libraries(ScalarBenchmarks PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(ScalarBenchmarks PRIVATE -static)
endif()

add_executable(Vector3Benchmarks
    Vector3Benchmarks.cpp
)

target_link_libraries(Vector3Benchmarks PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(Vector3Benchmarks PRIVATE -static)
endif()

add_executable(Matrix2Benchmarks
    Matrix2Benchmarks.cpp
)

target_link_libraries(Matrix2Benchmarks PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(Matrix2Benchmarks PRIVATE -static)
endif()

add_executable(QuaternionBenchmarks
    QuaternionBenchmarks.cpp
)

target_link_libraries(QuaternionBenchmarks PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(QuaternionBenchmarks PRIVATE -static)
endif()

add_executable(PointBenchmarks
    PointBenchmarks.cpp
)

target_link_libraries(PointBenchmarks PRIVATE
    ShapeEngine
)

if(NOT MSVC)
    target_link_options(PointBenchmarks PRIVATE -static)
endif()
