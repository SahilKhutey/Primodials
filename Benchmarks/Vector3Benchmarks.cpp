#include "ShapeEngine/Math/Vector3.hpp"
#include "Core/Platform.hpp"
#include <chrono>
#include <iostream>

using namespace Shape::Math;

void RunVector3Benchmarks() {
    std::cout << "--- Running Vector3 Math Benchmarks ---" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    volatile float dummy = 0.0f;
    Vector3f a(1.0f, 2.0f, 3.0f);
    Vector3f b(4.0f, 5.0f, 6.0f);
    u64 iterations = 50'000'000ULL;

    for (u64 i = 0; i < iterations; ++i) {
        dummy += Dot(a, b);
        Vector3f c = Cross(a, b);
        a = Normalize(c);
        b = Reflect(a, Vector3f::UnitY());
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "BM_Vector3 iterations: " << iterations << ", Time: " << elapsed.count() << " ms" << std::endl;
}

int main() {
    RunVector3Benchmarks();
    return 0;
}
