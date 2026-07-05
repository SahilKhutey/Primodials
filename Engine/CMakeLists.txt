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

    # ECS2 — Archetype-based ECS (Phase B)
    ECS2/ArchetypeStorage.cpp
    ECS2/World2.cpp
    ECS2/CommandBuffer2.cpp

    # Platform input layer (Phase B)
    Platform/Events.cpp
    Platform/Input.cpp

    Input/InputSystem.cpp
    Platform/Window.cpp
    Platform/Source/Platform.cpp
    Platform/Source/Windows/Win32Window.cpp
    Simulation/SimulationClock.cpp
    Simulation/SimulationScheduler.cpp
    Physics/SpatialHashGrid.cpp

    # Phase C — Simulation Core
    Simulation/Spatial/SpatialGrid.cpp
    Simulation/Genetics/GeneticsSystem.cpp
    Simulation/Evolution/EvolutionSystem.cpp
    Simulation/Resources/ResourceSystem.cpp
    
    # Phase D — Life Demo Simulation
    Simulation/Reproduction/ReproductionSystem.cpp
    Simulation/Combat/CombatSystem.cpp
    Simulation/Evolution/SpeciationSystem.cpp
    Simulation/Analytics/PopulationGraph.cpp
    Simulation/Snapshots/SimulationSnapshot.cpp

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ECS2>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Reflection>
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

add_subdirectory(UI)
add_subdirectory(Editor)
add_subdirectory(Replay)
add_subdirectory(Procedural)
add_subdirectory(Rendering)
add_subdirectory(Network)
add_subdirectory(Scripting)
add_subdirectory(Compute)
add_subdirectory(AssetPipeline)
add_subdirectory(Optimization)

