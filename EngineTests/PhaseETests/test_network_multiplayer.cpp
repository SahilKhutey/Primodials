#include <catch2/catch_test_macros.hpp>
#include "Network/Transport/LoopbackTransport.hpp"
#include "Network/Protocol/Packet.hpp"
#include "Network/Protocol/InputCommand.hpp"
#include "Network/Lockstep/LockstepManager.hpp"
#include "Network/Lockstep/RollbackSystem.hpp"
#include "Network/NetworkManager.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"

TEST_CASE("Network Multiplayer Subsystem - Loopback Transport, Lockstep, and Rollback", "[Network]") {
    SECTION("LoopbackTransport in-process packet delivery") {
        Shape::Network::LoopbackTransport transport;
        REQUIRE(transport.Initialize(7777, 4));

        u8 payload[] = { 0x01, 0x02, 0x03, 0x04 };
        Shape::Network::Endpoint peer{ "127.0.0.1", 7777, 100 };
        REQUIRE(transport.Send(peer, payload, sizeof(payload)));

        auto pkt = transport.Receive();
        REQUIRE(pkt.has_value());
        REQUIRE(pkt->data.size() == 4);
        REQUIRE(pkt->data[0] == 0x01);
        REQUIRE(pkt->data[3] == 0x04);
        REQUIRE(transport.GetBytesSent() == 4);
        REQUIRE(transport.GetBytesReceived() == 4);
    }

    SECTION("InputBatch serialization roundtrip") {
        Shape::Network::PlayerInput in;
        in.type = Shape::Network::PlayerInputType::SpawnEntity;
        in.tickNumber = 120;
        in.playerId = 0x12345678;
        in.targetEntity = 42;
        in.position = { 150.0f, 250.0f };
        in.floatValue = 3.14f;
        in.stringValue = "SpeciesAlpha";

        Shape::Network::InputBatch batch;
        batch.startTick = 120;
        batch.endTick = 120;
        batch.frameNumber = 1;
        batch.inputs.push_back(in);

        auto bytes = batch.Serialize();
        Shape::Network::PacketReader reader(bytes.data(), bytes.size());
        REQUIRE(reader.GetType() == Shape::Network::PacketType::InputBatch);

        auto restoredBatch = Shape::Network::InputBatch::Deserialize(reader);
        REQUIRE(restoredBatch.startTick == 120);
        REQUIRE(restoredBatch.inputs.size() == 1);
        REQUIRE(restoredBatch.inputs[0].type == Shape::Network::PlayerInputType::SpawnEntity);
        REQUIRE(restoredBatch.inputs[0].tickNumber == 120);
        REQUIRE(restoredBatch.inputs[0].playerId == 0x12345678);
        REQUIRE(restoredBatch.inputs[0].position.x == 150.0f);
        REQUIRE(restoredBatch.inputs[0].stringValue == "SpeciesAlpha");
    }

    SECTION("LockstepManager state transitions and input application") {
        Shape::World world;
        Shape::Network::LockstepManager lockstep;
        lockstep.Initialize(&world);

        lockstep.AddLocalPlayer(1);
        lockstep.AddRemotePlayer(2, "PeerClient");
        REQUIRE(lockstep.GetPlayerCount() == 2);

        // Update to trigger state transition to Running
        lockstep.Update(0.0f);
        REQUIRE(lockstep.GetState() == Shape::Network::LockstepState::Running);

        Shape::Network::PlayerInput in;
        in.type = Shape::Network::PlayerInputType::SpawnEntity;
        in.position = { 50.0f, 50.0f };
        lockstep.SubmitInput(in);

        lockstep.Update(0.1f); // Advance tick
        auto entities = world.Query<Shape::TransformComponent>();
        REQUIRE(entities.size() == 1);
    }

    SECTION("RollbackSystem snapshot capture and state restoration") {
        Shape::World world;
        Shape::Entity e1 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e1, Shape::TransformComponent{ .position = { 10.0f, 20.0f } });

        Shape::Network::RollbackSystem rollback;
        rollback.Initialize(&world, 10);

        rollback.CaptureSnapshot(100, 1.6);
        REQUIRE(rollback.GetSnapshotCount() == 1);

        world.GetComponent<Shape::TransformComponent>(e1).position = { 999.0f, 999.0f };

        REQUIRE(rollback.RestoreToTick(100));
        auto restoredEntities = world.Query<Shape::TransformComponent>();
        REQUIRE(restoredEntities.size() == 1);
        const auto& tf = world.GetComponent<Shape::TransformComponent>(restoredEntities[0]);
        REQUIRE(tf.position.x == 10.0f);
        REQUIRE(tf.position.y == 20.0f);
    }

    SECTION("NetworkManager initialization and update pipeline") {
        Shape::World world;
        Shape::Network::NetworkManager net;
        REQUIRE(net.Initialize(&world, 1));
        REQUIRE(net.IsInitialized());
        REQUIRE(net.GetRole() == Shape::Network::NetworkRole::Host);

        Shape::Network::PlayerInput in;
        in.type = Shape::Network::PlayerInputType::SpawnEntity;
        in.position = { 30.0f, 40.0f };
        net.SubmitInput(in);

        net.Update(0.0f); // Transition to Running
        net.Update(0.1f); // Advance tick
        REQUIRE(net.GetLockstep().GetCurrentTick() > 0);
    }
}
