// EngineTests/PerformanceTests/test_baseline.cpp
#include <catch2/catch_test_macros.hpp>
#include "ECS2/World2.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include <chrono>
#include <iostream>

using namespace Shape::ECS2;
using Clock = std::chrono::high_resolution_clock;

struct BPos { float x = 0.0f, y = 0.0f; };
struct BVel { float vx = 1.0f, vy = 1.0f; };

struct PerfReg {
    PerfReg() {
        SHAPE_REGISTER_COMPONENT(BPos);
        SHAPE_REGISTER_COMPONENT(BVel);
    }
};
static PerfReg _preg;

TEST_CASE("1M Entities Performance Benchmark", "[perf][stress]") {
    constexpr int N = 1000000;
    World2 world;

    std::cout << "\n[BENCHMARK] Creating " << N << " entities with BPos and BVel...\n";
    auto t0 = Clock::now();
    for (int i = 0; i < N; ++i) {
        EntityId e = world.create();
        world.add<BPos>(e, {(float)i, (float)i});
        world.add<BVel>(e, {1.0f, -1.0f});
    }
    auto t1 = Clock::now();
    auto create_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "[BENCHMARK] Successfully created in " << create_ms << " ms.\n";

    REQUIRE(world.entity_count() == N);

    constexpr int FRAMES = 100;
    std::cout << "[BENCHMARK] Simulating " << FRAMES << " frames of physics iteration...\n";
    auto t2 = Clock::now();
    for (int f = 0; f < FRAMES; ++f) {
        world.for_each<BPos, BVel>([](BPos& p, BVel& v) {
            p.x += v.vx * 0.016f;
            p.y += v.vy * 0.016f;
        });
    }
    auto t3 = Clock::now();
    auto tick_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
    double avg_frame_ms = (double)tick_ms / FRAMES;
    double fps = 1000.0 / avg_frame_ms;

    std::cout << "[BENCHMARK] Completed " << FRAMES << " frames in " << tick_ms << " ms.\n";
    std::cout << "[BENCHMARK] Average frame time: " << avg_frame_ms << " ms (" << fps << " FPS)\n";

    // Clean up
    auto t4 = Clock::now();
    world.clear_all();
    auto t5 = Clock::now();
    auto destroy_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count();
    std::cout << "[BENCHMARK] Destroyed all entities in " << destroy_ms << " ms.\n";

    REQUIRE(world.entity_count() == 0);
}
