add_library(ShapeEngine STATIC
    Core/Logger.cpp
    Core/Bootstrap.cpp
    Core/Time.cpp
    Memory/ArenaAllocator.cpp
    Memory/PoolAllocator.cpp
    Math/MathUtils.cpp
    Math/Matrix4.cpp
    Math/Source/Scalar.cpp
    Math/Source/Matrix2.cpp
    Jobs/JobSystem.cpp
    
    ECS/World.cpp
    ECS/SpeciesSchema.cpp
    ECS/CommandBuffer.cpp
    Input/InputSystem.cpp
    Platform/Window.cpp
    Platform/Source/Platform.cpp
    Platform/Source/Windows/Win32Window.cpp
    Simulation/SimulationClock.cpp
    Simulation/SimulationScheduler.cpp
    Physics/SpatialHashGrid.cpp
    Physics/PhysicsSystem.cpp
    Math/PolygonGeometry.cpp
    AI/OptimizationSystem.cpp
    Biology/BiologySystem.cpp
    AI/CognitiveSystem.cpp
    Influence/InfluenceSystem.cpp
    Sentinel/SentinelSystem.cpp
    ECS/CareerSystem.cpp
    Civilization/CivilizationSystem.cpp
    Utility/CRC32.cpp
    Networking/LockstepSynchronizer.cpp
    Networking/DesyncVerifier.cpp
    Serialization/Serializer.cpp
    Simulation/ReplaySystem.cpp
    Rendering/SDL3Renderer.cpp
    Rendering/RenderingSystem.cpp
)

target_include_directories(ShapeEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Platform/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Math/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Containers>
    $<INSTALL_INTERFACE:include>
)

# Enable thread safety for Logger
find_package(Threads REQUIRED)
target_link_libraries(ShapeEngine PUBLIC Threads::Threads SDL3::SDL3)

# Target-specific compiler options
if(MSVC)
    target_compile_options(ShapeEngine PRIVATE
        /W4          # Warning level 4
        /WX          # Treat warnings as errors
        /permissive- # Standards conformance
        /volatile:iso
        /EHsc
        /fp:precise
    )
else()
    target_compile_options(ShapeEngine PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
        -fno-fast-math
    )
endif()
