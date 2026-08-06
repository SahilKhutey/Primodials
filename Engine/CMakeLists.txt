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

    # Platform input & wallpaper layer
    Platform/Events.cpp
    Platform/Input.cpp
    Platform/SystemTray.cpp
    Platform/WallpaperWindow.cpp
    Platform/PowerManager.cpp
    Wallpaper/SystemTray.cpp
    Wallpaper/WallpaperWindow.cpp
    Wallpaper/PowerManager.cpp

    Themes/ThemeManager.cpp
    Themes/ThemeApplier.cpp
    Particles/AmbientParticleSystem.cpp
    Wallpaper/CameraDirector.cpp
    Wallpaper/GameViewWindow.cpp
    Audio/AmbientPlayer.cpp
    Wallpaper/WallpaperSettings.cpp

    UI/SettingsState.cpp
    UI/Loading/LoadingScreen.cpp
    UI/Menus/MainMenu.cpp
    UI/Menus/PauseMenu.cpp
    UI/HUD/GameHUD.cpp
    UI/ThemeSystem/ThemeSelector.cpp
    UI/Settings/SettingsWindow.cpp
    UI/HUD/EntityInspector.cpp
    UI/Widgets/NotificationToast.cpp
    UI/Widgets/ColorPicker.cpp
    UI/Profiles/ProfileManager.cpp
    GameView/GameViewWindow.cpp
    GameView/SharedStateSync.cpp
    Wallpaper/WallpaperTrayMenu.cpp
    Renderer/BackgroundRenderer.cpp
    Renderer/Renderers/ParticleRenderer.cpp

    GameView/SharedSimulationState.cpp
    GameView/GameViewWindow.cpp
    Diary/EcosystemDiary.cpp
    UI/Panels/DiaryPanel.cpp
    Steam/Workshop/WorkshopThemeManager.cpp
    Steam/Workshop/ThemeValidator.cpp
    Steam/Workshop/ThemePublisher.cpp
    Authoring/ThemePreviewRenderer.cpp
    Steam/Workshop/WorkshopDiscovery.cpp
    Steam/Workshop/WorkshopRatings.cpp
    Steam/Workshop/WorkshopComments.cpp
    UI/Panels/WorkshopThemePanel.cpp
    UI/Panels/WorkshopCreatorPanel.cpp
    AutoStart/AutoStartManager.cpp
    Localization/LocaleManager.cpp

    Input/InputSystem.cpp
    Platform/Window.cpp
    Platform/Source/Platform.cpp
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
    # Phase E — Expanded Simulation Systems
    Simulation/src/Systems/DiplomacySystem.cpp
    Simulation/src/Systems/GroupBehaviorSystem.cpp
    Simulation/src/Systems/ProceduralWorldSystem.cpp
    Simulation/src/Systems/TechnologySystem.cpp
    Simulation/src/Systems/WeatherSystem.cpp
    Simulation/src/Systems/BuildingSystem.cpp
    Simulation/src/Systems/NeuralControlSystem.cpp
    Simulation/src/Systems/QLearningSystem.cpp
    Simulation/src/Systems/BiomeSystem.cpp
    Simulation/src/Systems/EnvironmentAdaptationSystem.cpp
    Simulation/src/SimulationController.cpp

    Rendering/SDL3Renderer.cpp
    Rendering/OpenGLRenderer.cpp
    Rendering/RenderingSystem.cpp
    Rendering/CameraController.cpp
    Rendering/GameInputController.cpp

    UI/ImGuiLayer.cpp
    UI/Panels/StatsPanel.cpp
    UI/Panels/PopulationGraphPanel.cpp
    UI/Panels/HierarchyPanel.cpp
    UI/Panels/InspectorPanel.cpp
    UI/Panels/ParameterPanel.cpp
    UI/Panels/ReplayPanel.cpp
    UI/Panels/SpeciesEditorPanel.cpp
    UI/Panels/NetworkPanel.cpp
    UI/Panels/VisualScriptPanel.cpp

    Spatial/BroadPhaseSystem.cpp

    Scripting/LuaSandbox.cpp
    Scripting/LuaModLoader.cpp

    Serialization/SimSerializer.cpp
    Simulation/ReplaySystem.cpp

    Network/Lockstep/LockstepManager.cpp

    VisualScripting/Core/Node.cpp
    VisualScripting/Core/NodeGraph.cpp
    VisualScripting/Core/NodeRegistry.cpp
    VisualScripting/Execution/GraphExecutor.cpp
    VisualScripting/Editor/NodeGraphEditor.cpp

    Steam/SteamContext.cpp
    Steam/Achievements/AchievementManager.cpp
    Steam/Workshop/WorkshopManager.cpp
    Steam/Cloud/CloudSaveManager.cpp
    Steam/Presence/RichPresence.cpp
    Steam/Stats/StatTracker.cpp

    Distribution/BuildCooker/AssetCooker.cpp

    UI/Components/Toast.cpp
    Onboarding/TutorialSequence.cpp
    Community/LeaderboardClient.cpp
    Community/FriendsListManager.cpp
    Community/ScreenshotUploader.cpp

    UI/Panels/AchievementsPanel.cpp
    UI/Panels/WorkshopBrowserPanel.cpp
    UI/Panels/CommunityHubPanel.cpp
)

# Win32Window.cpp uses <windows.h> and only makes sense on Windows; the cross-platform
# Platform/Window.cpp (SDL3-based) is what's actually used on all platforms including Windows.
if(WIN32)
    target_sources(ShapeEngine PRIVATE Platform/Source/Windows/Win32Window.cpp)
endif()

target_include_directories(ShapeEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Platform/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Math/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Simulation/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Containers>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ECS2>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Reflection>
    $<INSTALL_INTERFACE:include>
)

# Enable thread safety for Logger
find_package(Threads REQUIRED)
target_link_libraries(ShapeEngine PUBLIC Threads::Threads SDL3::SDL3)

if(WIN32)
    target_link_libraries(ShapeEngine PUBLIC shell32 gdi32 user32 dwmapi powrprof)
endif()

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

add_subdirectory(Replay)
add_subdirectory(Network)
add_subdirectory(Scripting)
add_subdirectory(Compute)
add_subdirectory(AssetPipeline)
add_subdirectory(Optimization)

