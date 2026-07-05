# Engine/Compute/CMakeLists.txt
add_library(ShapeCompute STATIC
    "include/Shape/Compute/IComputeContext.h"
    "include/Shape/Compute/ComputeBuffer.h"
    "include/Shape/Compute/ComputeShader.h"
    "include/Shape/Compute/ComputeKernel.h"
    "include/Shape/Compute/BatchSim.h"
    "src/ComputeContext.cpp"
    "src/ComputeBuffer.cpp"
    "src/ComputeShader.cpp"
    "src/BatchSim.cpp"
)

target_include_directories(ShapeCompute PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
)

target_link_libraries(ShapeCompute PUBLIC ShapeEngine)
