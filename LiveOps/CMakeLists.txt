# LiveOps/CMakeLists.txt
add_library(LiveOps STATIC
    src/Dashboard.cpp
    src/MetricsAggregator.cpp
    src/AnomalyDetector.cpp
    src/AlertingSystem.cpp
    src/LiveOpsClient.cpp
    src/RemoteConfig.cpp
    src/ABTest.cpp
    src/PlayerSegmentation.cpp
    src/CohortAnalysis.cpp
    src/HealthMonitor.cpp
    src/ServerMonitor.cpp
    src/RollbackManager.cpp
    src/IncidentReport.cpp
    src/SupportWorkflow.cpp
    src/TournamentSystem.cpp
    src/SeasonalEvent.cpp
    src/WorkshopCurator.cpp
    src/RoadmapTracker.cpp
)

target_include_directories(LiveOps PUBLIC src)
target_link_libraries(LiveOps PRIVATE ShapeEngine)

if(NOT MSVC)
    target_compile_options(LiveOps PRIVATE -Wall -Wextra)
endif()
