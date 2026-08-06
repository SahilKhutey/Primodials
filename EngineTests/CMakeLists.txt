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
    AI/test_neural_network.cpp
    AI/test_qlearning.cpp
    Civilization/CivilizationSystemTests.cpp
    Networking/NetworkingTests.cpp
    Serialization/SerializationTests.cpp
    ContainerTests.cpp
    WallpaperTests/test_wallpaper_foundation.cpp
    WallpaperTests/test_game_view.cpp
    WallpaperTests/test_wallpaper_settings.cpp
    WallpaperTests/test_wallpaper_standalone.cpp
    WallpaperTests/test_wallpaper_sprint2.cpp
    WallpaperTests/test_wallpaper_sprint3.cpp
    WallpaperTests/test_wallpaper_sprint4.cpp
    WallpaperTests/test_wallpaper_sprint5.cpp
    WallpaperTests/test_final_qa.cpp
    WallpaperTests/test_full_suite.cpp

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
    PhaseETests/test_group_behavior.cpp
    PhaseETests/test_weather.cpp
    PhaseETests/test_building.cpp
    PhaseETests/test_biome.cpp
    PhaseETests/test_simulation_controller.cpp
    PhaseETests/test_camera_controller.cpp
    PhaseETests/test_opengl_renderer.cpp
    PhaseETests/test_imgui_panels.cpp
    PhaseETests/test_spatial_hash_system.cpp
    PhaseETests/test_lua_sandbox.cpp
    PhaseETests/test_replay_system.cpp
    PhaseETests/test_final_release_features.cpp
    PhaseETests/test_vulkan_renderer.cpp
    PhaseETests/test_network_multiplayer.cpp
    PhaseETests/test_visual_scripting.cpp
    PhaseETests/test_steam_and_distribution.cpp
    PhaseETests/test_community_polish.cpp
    PhaseETests/FullPipelineTests.cpp
    PhaseETests/SaveLoadRoundTrip.cpp
    PhaseETests/MultiplayerDeterminismTests.cpp
    PhaseETests/LongRunTest.cpp
    PhaseETests/test_end_to_end_user_scenarios.cpp
    PhaseETests/test_environment_adaptation.cpp
    PhaseETests/test_ui_system.cpp
    PhaseETests/test_phase8_ui.cpp
    PhaseETests/test_phase9_hud.cpp

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
