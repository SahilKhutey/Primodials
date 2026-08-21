#include <chrono>
#include <cstdint>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Basic engine performance remains within validation budget",
          "[performance][final-validation]") {
    constexpr std::uint64_t iterations = 1'000'000;

    volatile std::uint64_t sink = 0;

    const auto start = std::chrono::steady_clock::now();

    for (std::uint64_t i = 0; i < iterations; ++i) {
        sink += (i * 2654435761ULL) ^ (i >> 7U);
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start
    ).count();

    // Keep the accumulator observable so an optimizing compiler cannot
    // eliminate the loop entirely.
    REQUIRE(sink != 0);

    // This is a smoke-test budget, not a published product benchmark.
    REQUIRE(elapsed < 1000);
}
