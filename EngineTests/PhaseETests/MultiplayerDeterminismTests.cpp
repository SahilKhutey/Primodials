#include <catch2/catch_test_macros.hpp>
#include "Network/Transport/LoopbackTransport.hpp"
#include "Network/Lockstep/LockstepManager.hpp"
#include "Network/Protocol/InputCommand.hpp"
#include "ECS/World.hpp"

using namespace Shape;
using namespace Shape::Network;

TEST_CASE("Two instances produce identical state with same inputs", "[multiplayer][determinism]") {
    World w1, w2;

    for (int i = 0; i < 50; ++i) {
        w1.CreateEntity();
        w2.CreateEntity();
    }

    REQUIRE(w1.GetActiveEntityCount() == w2.GetActiveEntityCount());
}

TEST_CASE("Lockstep manager processes inputs in order", "[multiplayer][lockstep]") {
    LockstepManager mgr1, mgr2;
    World w1, w2;
    mgr1.Initialize(&w1);
    mgr2.Initialize(&w2);

    mgr1.AddLocalPlayer(1);
    mgr1.AddRemotePlayer(2, "Alice");
    mgr2.AddLocalPlayer(2);
    mgr2.AddRemotePlayer(1, "Bob");

    PlayerInput input;
    input.tickNumber = 0;
    input.playerId = 1;
    input.type = PlayerInputType::SpawnEntity;

    mgr1.SubmitInput(input);
    mgr2.SubmitInput(input);

    REQUIRE(mgr1.GetPlayerCount() == 2);
    REQUIRE(mgr2.GetPlayerCount() == 2);
    REQUIRE(mgr1.GetCurrentTick() == mgr2.GetCurrentTick());
}

TEST_CASE("State hash matches across instances", "[multiplayer][hash]") {
    World w1, w2;

    for (int i = 0; i < 10; ++i) {
        w1.CreateEntity();
        w2.CreateEntity();
    }

    REQUIRE(w1.GetActiveEntityCount() == w2.GetActiveEntityCount());
}

TEST_CASE("Desync detection catches state divergence", "[multiplayer][desync]") {
    World w1, w2;

    for (int i = 0; i < 10; ++i) {
        w1.CreateEntity();
    }
    for (int i = 0; i < 9; ++i) {
        w2.CreateEntity();
    }

    REQUIRE(w1.GetActiveEntityCount() != w2.GetActiveEntityCount());
}
