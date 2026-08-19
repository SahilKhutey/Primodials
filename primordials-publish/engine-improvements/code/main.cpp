/**
 * Primordials — Native Engine Demonstration Application (C++23)
 *
 * Demonstrates high-capacity entity simulation with profiling.
 *
 * License: MIT
 */

#include "Entity.hpp"
#include "World.hpp"
#include "SpatialHash.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include "LotkaVolterra.hpp"
#include "Renderer.hpp"
#include "Profiler.hpp"

#include <chrono>
#include <cstdio>
#include <thread>

struct Position { float x, y; };
struct Velocity { float dx, dy; };
struct Species {
    int type; // 0=producer, 1=grazer, 2=predator
    float energy;
};

int main() {
    primordials::World world;
    world.register_component<Position>();
    world.register_component<Velocity>();
    world.register_component<Species>();

    primordials::SpatialHash grid(50.0f, 2048);
    primordials::Random rng(1337);
    primordials::render::BatchRenderer renderer;

    constexpr std::size_t ENTITY_COUNT = 50000;
    std::printf("Initializing Primordials native engine with %zu entities...\n", ENTITY_COUNT);

    for (std::size_t i = 0; i < ENTITY_COUNT; ++i) {
        auto e = world.create_entity<Position>(
            rng.range_float(0.0f, 1920.0f),
            rng.range_float(0.0f, 1080.0f)
        );
        world.get<Velocity>(e) = {
            rng.range_float(-15.0f, 15.0f),
            rng.range_float(-15.0f, 15.0f)
        };
        world.get<Species>(e) = {
            rng.range_int(0, 2),
            1.0f
        };
    }

    constexpr float DT = 1.0f / 60.0f;
    std::printf("Running 180 simulation frames...\n");

    for (int frame = 0; frame < 180; ++frame) {
        {
            PROFILE_SCOPE("Movement System");
            auto* pos = world.get<Position>();
            auto* vel = world.get<Velocity>();
            if (pos && vel) {
                for (std::size_t i = 0; i < ENTITY_COUNT; ++i) {
                    pos[i].x += vel[i].dx * DT;
                    pos[i].y += vel[i].dy * DT;
                    if (pos[i].x < 0.0f) pos[i].x += 1920.0f;
                    if (pos[i].x >= 1920.0f) pos[i].x -= 1920.0f;
                    if (pos[i].y < 0.0f) pos[i].y += 1080.0f;
                    if (pos[i].y >= 1080.0f) pos[i].y -= 1080.0f;
                }
            }
        }

        {
            PROFILE_SCOPE("Spatial Hash & Ecology");
            grid.clear();
            auto* pos = world.get<Position>();
            for (std::size_t i = 0; i < ENTITY_COUNT; ++i) {
                grid.insert(static_cast<std::uint32_t>(i), pos[i].x, pos[i].y);
            }
        }

        {
            PROFILE_SCOPE("Render Submission");
            renderer.begin_frame();
            auto* pos = world.get<Position>();
            for (std::size_t i = 0; i < ENTITY_COUNT; ++i) {
                renderer.submit_instance(pos[i].x, pos[i].y, 0.3f, 0.8f, 0.7f, 1.0f);
            }
            renderer.end_frame();
        }
    }

    primordials::profile::Profiler::instance().print_summary();
    return 0;
}
