# Engine/Network/CMakeLists.txt
add_library(ShapeNetwork STATIC
    "include/Shape/Network/INetworkBackend.h"
    "include/Shape/Network/NetPacket.h"
    "include/Shape/Network/NetProtocol.h"
    "include/Shape/Network/NetSerializer.h"
    "include/Shape/Network/LockstepSession.h"
    "include/Shape/Network/RollbackSession.h"
    "include/Shape/Network/Matchmaker.h"
    "include/Shape/Network/Lobby.h"
    "include/Shape/Network/ENetBackend.h"
    "src/LockstepSession.cpp"
    "src/RollbackSession.cpp"
    "src/NetSerializer.cpp"
    "src/Matchmaker.cpp"
    "src/Lobby.cpp"
    "src/ENetBackend.cpp"
)

target_include_directories(ShapeNetwork PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
)

target_link_libraries(ShapeNetwork PUBLIC ShapeEngine)
