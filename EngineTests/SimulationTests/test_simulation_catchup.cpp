#include <catch2/catch_test_macros.hpp>
#include "Simulation/SimulationScheduler.hpp"
#include <cmath>

using namespace Shape;

TEST_CASE("Scheduler Catchup — Single large step runs up to budget", "[simulation][catchup]") {
    SimulationScheduler sched(60);
    sched.set_max_ticks_per_step(32);
    sched.set_max_owed_ticks(1000);

    // 1 second of real time = 60 owed ticks. Budget is 32.
    sched.step(1.0);

    REQUIRE(sched.last_step_ticks() == 32);
    REQUIRE(sched.owed_ticks_unrun() == 28);  // 60 - 32
    REQUIRE(sched.dropped_ticks_total() == 0); // None dropped yet (we'll catch up)
}

TEST_CASE("Scheduler Catchup — Subsequent normal frames pay down the debt", "[simulation][catchup]") {
    SimulationScheduler sched(60);
    sched.set_max_ticks_per_step(5); // Small budget to force gradual catch-up

    // 12 ticks of real time. Budget is 5. Runs 5, owes 7.
    sched.step(12.0 / 60.0);
    REQUIRE(sched.owed_ticks_unrun() == 7);

    // Normal frame: 1 new + 7 debt = 8. Budget is 5. Runs 5, owes 3.
    sched.step(1.0 / 60.0);
    REQUIRE(sched.owed_ticks_unrun() == 3);

    // Next frame: 1 new + 3 debt = 4. Budget is 5. Runs 4, owes 0.
    sched.step(1.0 / 60.0);
    REQUIRE(sched.owed_ticks_unrun() == 0);
    REQUIRE(sched.current_tick() == 14); // 5 + 5 + 4 = 14 total
}

TEST_CASE("Scheduler Catchup — DropDebtImmediately policy drops debt", "[simulation][catchup]") {
    SimulationScheduler sched(60);
    sched.set_max_ticks_per_step(32);
    sched.set_debt_policy(SimulationScheduler::DebtPolicy::DropDebtImmediately);

    sched.step(1.0);
    REQUIRE(sched.last_step_ticks() == 1);  // Only one tick ran
    REQUIRE(sched.owed_ticks_unrun() == 0); // Debt dropped
    REQUIRE(sched.dropped_ticks_total() == 59); // 60 - 1
}

TEST_CASE("Scheduler Catchup — RequireUserAcknowledgment policy freezes", "[simulation][catchup]") {
    SimulationScheduler sched(60);
    sched.set_max_ticks_per_step(32);
    sched.set_max_owed_ticks(100);
    sched.set_debt_policy(SimulationScheduler::DebtPolicy::RequireUserAcknowledgment);

    // 2 seconds of debt = 120 owed ticks > 100 max. Should freeze.
    sched.step(2.0);
    REQUIRE(sched.last_step_ticks() == 0);
    REQUIRE(sched.current_tick() == 0);

    // User acks. Next step runs normally within new budget.
    sched.acknowledge_debt();
    sched.step(1.0 / 60.0);
    REQUIRE(sched.current_tick() == 1);
}

TEST_CASE("Scheduler Catchup — Sub-tick accumulator holds time", "[simulation][catchup]") {
    SimulationScheduler sched(60);
    sched.set_max_ticks_per_step(32);

    // dt = 1/300 (5ms at 60Hz target) = 0.2 ticks owed.
    sched.step(1.0 / 300.0);
    REQUIRE(sched.current_tick() == 0);  // No whole tick yet
    REQUIRE(sched.owed_ticks_unrun() == 0);

    // Accumulator holds 1/300. After 3 more frames = 4/300
    sched.step(1.0 / 300.0);
    sched.step(1.0 / 300.0);
    sched.step(1.0 / 300.0);

    // 4 * 1/300 = 4/300. 5th frame adds 1/300 -> 5/300 = 1/60. Run 1.
    sched.step(1.0 / 300.0);
    REQUIRE(sched.current_tick() == 1);
}

TEST_CASE("Scheduler Catchup — Replay determinism under hitch", "[simulation][catchup]") {
    SimulationScheduler sched_a(60), sched_b(60);

    const double dt = 1.0 / 60.0;
    for (int i = 0; i < 30; ++i) {
        sched_a.step(dt);
        sched_b.step(dt);
    }

    sched_a.step(1.0);
    sched_b.step(1.0);

    REQUIRE(sched_a.current_tick() == sched_b.current_tick());
    REQUIRE(sched_a.owed_ticks_unrun() == sched_b.owed_ticks_unrun());
    REQUIRE(sched_a.dropped_ticks_total() == sched_b.dropped_ticks_total());
}
