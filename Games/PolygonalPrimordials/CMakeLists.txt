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

# Standalone PolygonalPrimordialsWallpaper executable
add_executable(PolygonalPrimordialsWallpaper
    WallpaperMain.cpp
    src/Achievements.cpp
    src/Telemetry.cpp
    src/CrashReporter.cpp
)

target_include_directories(PolygonalPrimordialsWallpaper PRIVATE src)
target_link_libraries(PolygonalPrimordialsWallpaper PRIVATE ShapeEngine SDL3::SDL3)

if(WIN32)
    target_link_libraries(PolygonalPrimordialsWallpaper PRIVATE shell32 gdi32 user32 dwmapi powrprof)
endif()

add_custom_command(TARGET PolygonalPrimordialsWallpaper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Content/Themes
        $<TARGET_FILE_DIR:PolygonalPrimordialsWallpaper>/Content/Themes
)
