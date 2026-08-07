# Engine/Optimization/CMakeLists.txt
add_library(ShapeOptimization STATIC
    "include/Shape/Optimization/Profiler.h"
    "include/Shape/Optimization/TracyIntegration.h"
    "include/Shape/Optimization/MemoryTracker.h"
    "include/Shape/Optimization/FrameAnalyzer.h"
    "include/Shape/Optimization/BatchedUpdates.h"
    "src/Profiler.cpp"
    "src/FrameAnalyzer.cpp"
)

target_include_directories(ShapeOptimization PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
)

target_link_libraries(ShapeOptimization PUBLIC ShapeEngine)
