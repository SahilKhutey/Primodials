# Games/PolygonalPrimordials/CMakeLists.txt
add_executable(PolygonalPrimordials
    src/main.cpp
    src/Game.cpp
    src/SteamIntegration.cpp
    src/Achievements.cpp
    src/CloudSaves.cpp
    src/Workshop.cpp
    src/Telemetry.cpp
    src/CrashReporter.cpp
)

target_include_directories(PolygonalPrimordials PRIVATE src)
target_link_libraries(PolygonalPrimordials PRIVATE ShapeEngine ShapeNetwork)

if(NOT MSVC)
    target_compile_options(PolygonalPrimordials PRIVATE -Wall -Wextra)
else()
    target_compile_options(PolygonalPrimordials PRIVATE /W3 /EHsc)
endif()
