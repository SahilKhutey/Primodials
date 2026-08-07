add_executable(ShapeEngineTests
    Main.cpp
    Core/LoggerTests.cpp
    Core/BootstrapTests.cpp
    Core/JobSystemTests.cpp
    Core/PlatformTests.cpp
    Memory/AllocatorTests.cpp
    Math/MathTests.cpp
    Math/ScalarTests.cpp
    Math/Vector3Tests.cpp
    Math/Matrix2Tests.cpp
    Math/QuaternionTests.cpp
    Math/GeometryTests.cpp
    Math/PointTests.cpp
    ECS/EcsTests.cpp
    ECS2/test_entity.cpp
    ECS2/test_archetype.cpp
    ECS2/test_command_buffer.cpp
    ECS2/test_stress.cpp
    Physics/PhysicsTests.cpp
    Biology/BiologyTests.cpp
    AI/CognitiveTests.cpp
    AI/CareerTests.cpp
    Civilization/CivilizationSystemTests.cpp
    Networking/NetworkingTests.cpp
    Serialization/SerializationTests.cpp
    ContainerTests.cpp

    # Phase C — Simulation Tests
    SimulationTests/test_simulation_time.cpp
    SimulationTests/test_simulation_catchup.cpp
    SimulationTests/test_spatial.cpp
    SimulationTests/test_genetics.cpp
    SimulationTests/test_species.cpp

    # Phase D — Life Demo Tests
    SimulationTests/test_reproduction.cpp
    SimulationTests/test_combat.cpp
    SimulationTests/test_genome_decoder.cpp
    SimulationTests/test_snapshot.cpp
    SimulationTests/test_speciation.cpp

    # Phase E — Editor, Replay, Civilization Tests
    PhaseETests/test_replay.cpp
    PhaseETests/test_procedural.cpp
    PhaseETests/test_tech_tree.cpp
    PhaseETests/test_diplomacy.cpp

    # Phase F — Vulkan, Multiplayer, Scripting, Assets
    NetworkTests/test_network.cpp
    ScriptingTests/test_scripting.cpp
    AssetTests/test_asset.cpp
    VulkanTests/test_vulkan.cpp

    # Phase G — Game and Performance Tests
    GameTests/test_steam_stub.cpp
    GameTests/test_achievements_roundtrip.cpp
    ../Games/PolygonalPrimordials/src/Achievements.cpp
    PerformanceTests/test_baseline.cpp
)

target_include_directories(ShapeEngineTests PRIVATE
    ${CMAKE_SOURCE_DIR}/Games/PolygonalPrimordials/src
)

target_link_libraries(ShapeEngineTests PRIVATE
    ShapeEngine
    Catch2::Catch2WithMain
)

if(NOT MSVC)
    target_link_options(ShapeEngineTests PRIVATE -static)
endif()

# Target-specific compiler options
if(MSVC)
    target_compile_options(ShapeEngineTests PRIVATE
        /W4          # Warning level 4
        /WX          # Treat warnings as errors
        /permissive- # Standards conformance
        /volatile:iso
        /EHsc
        /fp:precise
    )
else()
    target_compile_options(ShapeEngineTests PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
        -fno-fast-math
        -Wno-c2y-extensions
    )
endif()

# Include the Catch module to discover tests automatically
include(CTest)
enable_testing()

include(Catch)
catch_discover_tests(ShapeEngineTests)
