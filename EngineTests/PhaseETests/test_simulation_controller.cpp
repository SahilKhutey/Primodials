#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/SimulationController.h"
#include "Simulation/SimulationClock.hpp"
#include "Simulation/SimulationScheduler.hpp"

TEST_CASE("SimulationController - State Machine & Time Scaling", "[Simulation][Controller]") {
    Shape::SimulationClock clock(60);
    Shape::SimulationScheduler scheduler;
    Shape::Simulation::SimulationController controller(&clock, &scheduler);

    SECTION("Initial state is Stopped") {
        REQUIRE(controller.GetState() == Shape::Simulation::SimulationState::Stopped);
        REQUIRE_FALSE(controller.IsRunning());
    }

    SECTION("Play / Pause Transitions") {
        controller.Play();
        REQUIRE(controller.GetState() == Shape::Simulation::SimulationState::Running);
        REQUIRE(controller.IsRunning());
        REQUIRE_FALSE(controller.IsPaused());

        controller.Pause();
        REQUIRE(controller.GetState() == Shape::Simulation::SimulationState::Paused);
        REQUIRE(controller.IsPaused());
    }

    SECTION("Speed Presets and Time Scaling") {
        controller.SetSpeedPreset(1);
        REQUIRE(controller.GetTimeScale() == 1.0f);

        controller.SetSpeedPreset(2);
        REQUIRE(controller.GetTimeScale() == 2.0f);

        controller.SetSpeedPreset(4);
        REQUIRE(controller.GetTimeScale() == 10.0f);
    }

    SECTION("Single-Tick Stepping") {
        uint64_t initial_tick = controller.GetCurrentTick();
        controller.StepSingleTick();
        REQUIRE(controller.GetCurrentTick() == initial_tick + 1);
        REQUIRE(controller.IsPaused());
    }
}
