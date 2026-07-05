// EngineTests/GameTests/test_steam_stub.cpp
#include <catch2/catch_test_macros.hpp>
#include "../../Games/PolygonalPrimordials/src/SteamStub.h"

TEST_CASE("SteamStub Initialization", "[steam]") {
    REQUIRE(SteamAPI_Init() == true);
    SteamAPI_Shutdown();
}
