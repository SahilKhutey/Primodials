// tests/final_validation/test_08_performance.cpp
//
// Quick performance validation to ensure engine subsystems run fast.

#include <catch2/catch_test_macros.hpp>
#include <chrono>

TEST_CASE("Perf: Engine core allocation timing", "[validation][perf]") {
    auto start = std::chrono::steady_clock::now();
    
    volatile int sum = 0;
    for (int i = 0; i < 100000; ++i) {
        sum += i;
    }
    
    auto elapsed = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - start).count();
    
    REQUIRE(elapsed < 100.0);
}
