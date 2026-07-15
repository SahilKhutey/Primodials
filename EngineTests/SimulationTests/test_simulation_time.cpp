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

    REQUIRE(sched.current_tick() == 0);
    REQUIRE(sched.sim_time() == 0.0);

    // Feed exactly 1 second of real time in small chunks
    usize ticks = 0;
    const double dt = 1.0 / 60.0;
    for (int i = 0; i < 60; ++i) {
        ticks += sched.step(dt);
    }
    REQUIRE(ticks == 60);
    REQUIRE(counter.count == 60);
    REQUIRE(sched.current_tick() == 60);
    REQUIRE(std::abs(sched.sim_time() - 1.0) < 1e-9);

    // One more frame: still 61
    sched.step(dt);
    REQUIRE(sched.current_tick() == 61);
}

TEST_CASE("Scheduler — spiral of death cap (max 8 ticks/frame)", "[simulation][time]") {
    SimulationScheduler sched(60);
    sched.set_max_ticks_per_step(8);
    TickCounter counter;
    sched.register_system(counter);

    // Feed 10 seconds at once — should cap at 8 ticks per step call
    const usize ticks = sched.step(10.0);
    REQUIRE(ticks <= 8);
    REQUIRE(counter.count <= 8);
}

TEST_CASE("Scheduler — pause halts tick execution", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    const double dt = 1.0 / 60.0;
    for (int i = 0; i < 30; ++i) sched.step(dt);
    const double t_before = sched.sim_time();

    sched.pause();
    const usize ticks = sched.step(1.0);
    REQUIRE(ticks == 0);
    REQUIRE(counter.count == 30);
    REQUIRE(sched.current_tick() == 30);
    REQUIRE(sched.sim_time() == t_before);
}

TEST_CASE("Scheduler — resume after pause continues", "[simulation][time]") {
    SimulationScheduler sched(60);
    TickCounter counter;
    sched.register_system(counter);

    sched.pause();
    sched.step(0.5);
    REQUIRE(counter.count == 0);

    sched.resume();
    for (int i = 0; i < 30; ++i) {
        sched.step(1.0 / 60.0);
    }
    REQUIRE(counter.count == 30);
}

TEST_CASE("Scheduler — speed scaling options", "[simulation][time]") {
    SECTION("2x speed doubles tick count") {
        SimulationScheduler sched(60);
        TickCounter counter;
        sched.register_system(counter);

        sched.set_speed(2.0f);
        usize ticks = 0;
        for (int i = 0; i < 30; ++i) {
            ticks += sched.step(0.5 / 30.0);
        }
        REQUIRE(ticks == 60);
        REQUIRE(counter.count == 60);
    }

    SECTION("0.5x speed multiplier does not corrupt sim_time") {
        SimulationScheduler sched(60);
        sched.set_speed(0.5f);
        const double dt = 1.0 / 60.0;
        for (int i = 0; i < 30; ++i) {
            sched.step(dt);
        }
        const u64 ticks_at_half = sched.current_tick();
        REQUIRE(ticks_at_half > 0);
        REQUIRE(ticks_at_half < 30);  // Should be ~15
        REQUIRE(std::abs(sched.sim_time() - ticks_at_half / 60.0) < 1e-9);
    }
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
        OrderedSystem(std::vector<int>* out_, int id_, int prio_) : out(out_), id(id_), prio(prio_) {}
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
        CheckSystem(u64* last_, bool* ok_) : last(last_), ok(ok_) {}
        void update(const TickContext& ctx) override {
            if (ctx.tick != *last + 1) *ok = false;
            *last = ctx.tick;
        }
        int  priority() const override { return 0; }
        std::string_view name() const override { return "CheckSystem"; }
    } checker{&last_tick, &monotone};

    sched.register_system(checker);
    for (int i = 0; i < 60; ++i) {
        sched.step(1.0 / 60.0);
    }

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
            explicit HashSystem(u64* out_) : out(out_) {}
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

TEST_CASE("Scheduler — determinism same input sequence", "[simulation][time]") {
    SimulationScheduler sched_a(60), sched_b(60);
    const double dt = 1.0 / 60.0;
    for (int i = 0; i < 100; ++i) {
        sched_a.step(dt);
        sched_b.step(dt);
    }
    REQUIRE(sched_a.current_tick() == sched_b.current_tick());
    REQUIRE(sched_a.sim_time() == sched_b.sim_time());
}
