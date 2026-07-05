/// @file test_stress.cpp — ECS2 1M entity stress test + memory footprint

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "ECS2/World2.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include <chrono>

using namespace Shape::ECS2;
using Clock = std::chrono::high_resolution_clock;

struct STransform { float x = 0, y = 0, rot = 0; };
struct SVelocity  { float vx = 0, vy = 0; };
struct STag       { int   team = 0; };

struct StressReg {
    StressReg() {
        SHAPE_REGISTER_COMPONENT(STransform);
        SHAPE_REGISTER_COMPONENT(SVelocity);
        SHAPE_REGISTER_COMPONENT(STag);
    }
};
static StressReg _sreg;

TEST_CASE("ECS2 Stress — 100K entities create+add+destroy", "[ecs2][stress]") {
    constexpr int N = 100'000;
    World2 world;
    std::vector<EntityId> ids;
    ids.reserve(N);

    auto t0 = Clock::now();
    for (int i = 0; i < N; ++i) {
        EntityId e = world.create();
        world.add<STransform>(e, {(float)i, (float)i, 0});
        world.add<SVelocity>(e, {1, 0});
        ids.push_back(e);
    }
    auto t1 = Clock::now();

    REQUIRE(world.entity_count() == N);

    // Simulate 1 frame of physics
    world.for_each<STransform, SVelocity>([](STransform& t, SVelocity& v) {
        t.x += v.vx * 0.016f;
        t.y += v.vy * 0.016f;
    });
    auto t2 = Clock::now();

    for (auto e : ids) world.destroy(e);
    REQUIRE(world.entity_count() == 0);
    auto t3 = Clock::now();

    auto create_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto tick_ms   = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    auto destroy_ms= std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

    INFO("Create " << N << " entities: " << create_ms << " ms");
    INFO("Tick " << N << " entities:   " << tick_ms   << " ms");
    INFO("Destroy " << N << " entities:" << destroy_ms << " ms");

    // At 60 FPS, budget per frame is 16 ms. Tick should be << 16ms for 100K.
    REQUIRE(tick_ms < 50); // Generous upper bound for debug builds
}

TEST_CASE("ECS2 Stress — archetype count stays bounded", "[ecs2][stress]") {
    World2 world;
    for (int i = 0; i < 1000; ++i) {
        EntityId e = world.create();
        world.add<STransform>(e);
        world.add<SVelocity>(e);
        if (i % 3 == 0) world.add<STag>(e, {i % 4});
    }
    // Should have at most 3 distinct archetypes:
    //   {STransform, SVelocity}
    //   {STransform, SVelocity, STag}
    //   (possibly {STransform} during migration but not after)
    REQUIRE(world.archetype_count() <= 4);
}
