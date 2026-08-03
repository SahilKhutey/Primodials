#include "ShapeEngine/Math/Quaternion.hpp"
#include "ShapeEngine/Math/Transform.hpp"
#include "Core/Platform.hpp"
#include <chrono>
#include <iostream>

using namespace Shape::Math;
using namespace Shape::Math::Literals;

void RunQuaternionBenchmarks() {
    std::cout << "--- Running Quaternion Benchmarks ---" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    volatile float dummy = 0.0f;
    Quaternionf q = Quaternionf::AxisAngle(Vector3f::UnitY(), 45.0_deg);
    Vector3f v(1.0f, 0.0f, 0.0f);
    u64 iterations = 20'000'000ULL;

    for (u64 i = 0; i < iterations; ++i) {
        q = (q * Quaternionf::AxisAngle(Vector3f::UnitX(), Radians(0.001f))).Normalized();
        Vector3f r = q.Rotate(v);
        dummy += r.x;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "BM_Quaternion iterations: " << iterations << ", Time: " << elapsed.count() << " ms\n";
}

void RunSlerpBenchmarks() {
    std::cout << "--- Running SLERP Benchmarks ---" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    volatile float dummy = 0.0f;
    Quaternionf a = Quaternionf::Identity();
    Quaternionf b = Quaternionf::AxisAngle(Vector3f::UnitZ(), 180.0_deg);
    u64 iterations = 20'000'000ULL;

    for (u64 i = 0; i < iterations; ++i) {
        float t = static_cast<float>(i % 1000) / 1000.0f;
        Quaternionf r = Quaternionf::SLerp(a, b, t);
        dummy += r.w;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "BM_SLERP iterations: " << iterations << ", Time: " << elapsed.count() << " ms\n";
}

int main() {
    RunQuaternionBenchmarks();
    RunSlerpBenchmarks();
    return 0;
}
