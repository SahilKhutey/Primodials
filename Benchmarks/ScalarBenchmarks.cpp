#include "ShapeEngine/Math/Scalar.hpp"
#include "Math/MathUtils.hpp"
#include "Core/Platform.hpp"
#include <chrono>
#include <iostream>

using namespace Shape::Math;

// Lightweight manual micro-benchmark implementation (in absence of Google Benchmark package)
void RunScalarBenchmarks() {
    std::cout << "--- Running Scalar Math Benchmarks ---" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    volatile float val = 125.45f;
    u64 iterations = 100'000'000ULL;

    for (u64 i = 0; i < iterations; ++i) {
        val = Sqrt(val);
        val = val * val; // Keep it warm
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "BM_Sqrt iterations: " << iterations << ", Time: " << elapsed.count() << " ms" << std::endl;
}

int main() {
    RunScalarBenchmarks();
    return 0;
}
