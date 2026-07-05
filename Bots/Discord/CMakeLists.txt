# Bots/Discord/CMakeLists.txt
add_executable(DiscordBot
    src/main.cpp
    src/Bot.cpp
    src/Commands.cpp
    src/Moderation.cpp
    src/SupportQueue.cpp
    src/Announcements.cpp
    src/LeaderboardSync.cpp
    src/EventScheduler.cpp
    src/PollsReactions.cpp
)

target_include_directories(DiscordBot PUBLIC src)
target_link_libraries(DiscordBot PRIVATE LiveOps) # Mocked link against LiveOps
