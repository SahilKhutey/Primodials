#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/StorageComponent.hpp"
#include "ECS/CivilizationComponent.hpp"
#include "Civilization/CivilizationSystem.hpp"

using namespace Shape;

TEST_CASE("CivilizationSystem: Logistics transfer balances storage", "[civilization][logistics]") {
    World world;
    CivilizationSystem system;

    Entity e1 = world.CreateEntity();
    world.AddComponent(e1, TransformComponent{{0.0f, 0.0f}, 0.0f, {1.0f, 1.0f}});
    world.AddComponent(e1, CivilizationComponent{FactionAlignment::Allied, 1, {}});
    StorageComponent s1;
    s1.minerals = 100.0f;
    world.AddComponent(e1, s1);

    Entity e2 = world.CreateEntity();
    world.AddComponent(e2, TransformComponent{{50.0f, 0.0f}, 0.0f, {1.0f, 1.0f}}); // Within 200 unit radius
    world.AddComponent(e2, CivilizationComponent{FactionAlignment::Allied, 1, {}});
    StorageComponent s2;
    s2.minerals = 0.0f;
    world.AddComponent(e2, s2);

    // Update with dt = 1.0f (10 units should transfer from e1 to e2)
    system.Update(world, 1.0f);

    auto& newS1 = world.GetComponent<StorageComponent>(e1);
    auto& newS2 = world.GetComponent<StorageComponent>(e2);

    REQUIRE(newS1.minerals == Catch::Approx(90.0f));
    REQUIRE(newS2.minerals == Catch::Approx(10.0f));
}

TEST_CASE("CivilizationSystem: Technology unlocks when science points reached", "[civilization][technology]") {
    World world;
    CivilizationSystem system;

    Entity e = world.CreateEntity();
    world.AddComponent(e, TransformComponent{});
    world.AddComponent(e, CivilizationComponent{FactionAlignment::Allied, 1, {}});
    StorageComponent s;
    s.sciencePoints = 150.0f; // Costs 100 to unlock Tech 1
    world.AddComponent(e, s);

    system.Update(world, 1.0f);

    auto& civ = world.GetComponent<CivilizationComponent>(e);
    auto& store = world.GetComponent<StorageComponent>(e);

    REQUIRE(civ.HasTechnology(1) == true);
    REQUIRE(store.sciencePoints == Catch::Approx(50.0f)); // 100 deducted
}
