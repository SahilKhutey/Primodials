#include <catch2/catch_all.hpp>
#include "Jobs/JobSystem.hpp"
#include <atomic>

TEST_CASE("JobSystem Execution", "[JobSystem]") {
    shape::JobSystem& jobSystem = shape::JobSystem::Get();
    jobSystem.Initialize();

    std::atomic<u32> counter{0};

    SECTION("Scheduling tasks increments counter across threads") {
        for (u32 i = 0; i < 10; ++i) {
            jobSystem.Schedule([&counter]() {
                counter++;
            });
        }

        jobSystem.WaitAll();
        REQUIRE(counter == 10);
    }

    jobSystem.Shutdown();
}
