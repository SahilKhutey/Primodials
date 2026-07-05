/// @file test_simulation_time.cpp — Fixed-step scheduler tests

#include <catch2/catch_test_macros.hpp>
#include "Simulation/SimulationScheduler.hpp"
#include "Simulation/ISimulationSystem.hpp"

using namespace Shape;

// ─── Counter system ─────────────────────────────────────────────────────────

struct TickCounter final : ISimulationSystem {
    int count = 0;
    void update(const TickContext&) override { ++count; }
    int  priority() const override { return 0; }
    std::string_view name() const override { return "TickCounter"; }
};

TEST_CASE("Scheduler — fixed delta accumulates correctly", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    // Feed exactly 1 second of real time in one chunk
    const usize ticks = sched.step(1.0);
    REQUIRE(ticks == 60);
    REQUIRE(counter.count == 60);
}

TEST_CASE("Scheduler — spiral of death cap (max 8 ticks/frame)", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    // Feed 10 seconds at once — should cap at 8 ticks per step call
    const usize ticks = sched.step(10.0);
    REQUIRE(ticks <= SimulationScheduler::MAX_TICKS_PER_FRAME);
    REQUIRE(counter.count <= static_cast<int>(SimulationScheduler::MAX_TICKS_PER_FRAME));
}

TEST_CASE("Scheduler — pause halts tick execution", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    sched.pause();
    const usize ticks = sched.step(1.0);
    REQUIRE(ticks == 0);
    REQUIRE(counter.count == 0);
}

TEST_CASE("Scheduler — resume after pause continues", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    sched.pause();
    sched.step(0.5);
    REQUIRE(counter.count == 0);

    sched.resume();
    sched.step(0.5);
    REQUIRE(counter.count == 30);
}

TEST_CASE("Scheduler — 2x speed doubles tick count", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    sched.set_speed(2.0f);
    const usize ticks = sched.step(0.5); // 0.5 * 2.0 = 1.0 sim-seconds = 60 ticks
    REQUIRE(ticks == 60);
    REQUIRE(counter.count == 60);
}

TEST_CASE("Scheduler — step_once fires exactly one tick", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    sched.step_once();
    sched.step_once();
    REQUIRE(counter.count == 2);
}

TEST_CASE("Scheduler — priority ordering: lower = first", "[simulation][time]") {
    SimulationScheduler sched(60);

    std::vector<int> order;

    struct OrderedSystem final : ISimulationSystem {
        std::vector<int>* out;
        int id;
        int prio;
        void update(const TickContext&) override { out->push_back(id); }
        int  priority() const override { return prio; }
        std::string_view name() const override { return "OrderedSystem"; }
    };

    OrderedSystem s1{&order, 1, 10};
    OrderedSystem s2{&order, 2, -5};
    OrderedSystem s3{&order, 3, 0};

    sched.register_system(s1);
    sched.register_system(s2);
    sched.register_system(s3);

    sched.step_once();

    REQUIRE(order.size() == 3);
    REQUIRE(order[0] == 2); // priority -5
    REQUIRE(order[1] == 3); // priority 0
    REQUIRE(order[2] == 1); // priority 10
}

TEST_CASE("Scheduler — TickContext.tick is monotone 1-indexed", "[simulation][time]") {
    SimulationScheduler sched(60);

    u64 last_tick = 0;
    bool monotone = true;

    struct CheckSystem final : ISimulationSystem {
        u64* last; bool* ok;
        void update(const TickContext& ctx) override {
            if (ctx.tick != *last + 1) *ok = false;
            *last = ctx.tick;
        }
        int  priority() const override { return 0; }
        std::string_view name() const override { return "CheckSystem"; }
    } checker{&last_tick, &monotone};

    sched.register_system(checker);
    sched.step(1.0); // 60 ticks

    REQUIRE(monotone);
    REQUIRE(last_tick == 60);
}

TEST_CASE("Scheduler — RNG is deterministic for same seed", "[simulation][time]") {
    auto run = [](u64 seed) -> u64 {
        SimulationScheduler sched(60);
        sched.set_seed(seed);

        u64 result = 0;
        struct HashSystem final : ISimulationSystem {
            u64* out;
            void update(const TickContext& ctx) override {
                *out ^= ctx.rng.NextU64() + ctx.tick;
            }
            int  priority() const override { return 0; }
            std::string_view name() const override { return "HashSystem"; }
        } h{&result};

        sched.register_system(h);
        sched.step(1.0);
        return result;
    };

    const u64 a = run(42);
    const u64 b = run(42);
    REQUIRE(a == b);
    REQUIRE(a != run(99)); // Different seed = different output
}
