#include <catch2/catch_test_macros.hpp>
#include "Serialization/SimSerializer.hpp"
#include "Simulation/ReplaySystem.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"

TEST_CASE("Replay Subsystem - Binary .sim Serialization & Timeline Scrubbing", "[Serialization][Replay]") {
    SECTION("SimSerializer memory roundtrip preservation") {
        Shape::World world;
        Shape::Entity e1 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e1, Shape::TransformComponent{ .position = { 100.0f, 200.0f } });

        Shape::GeometryComponent geo(5, 10.0f);
        world.AddComponent<Shape::GeometryComponent>(e1, geo);

        std::vector<u8> memory;
        REQUIRE(Shape::Serialization::SimSerializer::SaveToMemory(world, memory, 12.5));

        Shape::World restoredWorld;
        f64 restoredSimTime = 0.0;
        REQUIRE(Shape::Serialization::SimSerializer::LoadFromMemory(restoredWorld, memory, &restoredSimTime));
        REQUIRE(restoredSimTime == 12.5);

        auto restoredEntities = restoredWorld.Query<Shape::TransformComponent>();
        REQUIRE(restoredEntities.size() == 1);
        const auto& tf = restoredWorld.GetComponent<Shape::TransformComponent>(restoredEntities[0]);
        REQUIRE(tf.position.x == 100.0f);
        REQUIRE(tf.position.y == 200.0f);
    }

    SECTION("ReplaySystem recording and timeline frame restoration") {
        Shape::World world;
        Shape::Simulation::ReplaySystem replay;
        replay.SetRecording(true);

        Shape::Entity e1 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e1, Shape::TransformComponent{ .position = { 10.0f, 10.0f } });
        replay.RecordFrame(world, 1, 0.016);

        world.GetComponent<Shape::TransformComponent>(e1).position = { 50.0f, 50.0f };
        replay.RecordFrame(world, 2, 0.032);

        REQUIRE(replay.GetFrameCount() == 2);

        // Restore frame 0 (position 10,10)
        REQUIRE(replay.RestoreFrame(world, 0));
        auto entitiesFrame0 = world.Query<Shape::TransformComponent>();
        REQUIRE(world.GetComponent<Shape::TransformComponent>(entitiesFrame0[0]).position.x == 10.0f);

        // Restore frame 1 (position 50,50)
        REQUIRE(replay.RestoreFrame(world, 1));
        auto entitiesFrame1 = world.Query<Shape::TransformComponent>();
        REQUIRE(world.GetComponent<Shape::TransformComponent>(entitiesFrame1[0]).position.x == 50.0f);
    }
}
