# Bots/SteamModeration/CMakeLists.txt
add_executable(SteamModerationBot
    src/main.cpp
)

target_include_directories(SteamModerationBot PUBLIC src)
target_link_libraries(SteamModerationBot PRIVATE LiveOps)
