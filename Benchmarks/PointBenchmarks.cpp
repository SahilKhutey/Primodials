#include "ShapeEngine/Math/Geometry/Point/PointAlgorithms.hpp"
#include "Core/Platform.hpp"
#include <chrono>
#include <iostream>
#include <array>

using namespace Shape::Math;
using namespace Shape::Math::Geometry;

template<typename Fn>
void RunBenchmark(const char* name, u64 iterations, Fn&& fn) {
    auto start = std::chrono::high_resolution_clock::now();
    fn(iterations);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = end - start;
    std::cout << name << ": " << iterations << " iters, "
              << ms.count() << " ms, "
              << (ms.count() / static_cast<double>(iterations) * 1e6) << " ns/iter\n";
}

int main() {
    std::cout << "=== Point Benchmarks ===\n\n";

    RunBenchmark("BM_Point3_Add_Vector", 50'000'000ULL, [](u64 n) {
        volatile float sink = 0.0f;
        Point3f p(1.0f, 2.0f, 3.0f);
        Vector3f v(0.001f, 0.001f, 0.001f);
        for (u64 i = 0; i < n; ++i) {
            p += v;
            sink += p.x;
        }
    });

    RunBenchmark("BM_Point3_Distance", 20'000'000ULL, [](u64 n) {
        volatile float sink = 0.0f;
        Point3f a(0, 0, 0);
        Point3f b(3.0f, 4.0f, 0.0f);
        for (u64 i = 0; i < n; ++i) {
            sink += Distance(a, b);
        }
    });

    RunBenchmark("BM_Point3_Lerp", 20'000'000ULL, [](u64 n) {
        volatile float sink = 0.0f;
        Point3f a(0, 0, 0), b(100.0f, 200.0f, 300.0f);
        for (u64 i = 0; i < n; ++i) {
            float t = static_cast<float>(i % 1000) / 1000.0f;
            Point3f m = LerpPoint(a, b, t);
            sink += m.x;
        }
    });

    // Algorithm benchmarks
    static const u32 PTS = 1024;
    static Point3f cloud[PTS];
    for (u32 i = 0; i < PTS; ++i) {
        cloud[i] = { static_cast<float>(i % 100), static_cast<float>(i * 3 % 100), 0.0f };
    }

    RunBenchmark("BM_BoundingBox_1024pts", 100'000ULL, [](u64 n) {
        volatile float sink = 0.0f;
        for (u64 i = 0; i < n; ++i) {
            auto bb = BoundingBox(cloud, PTS);
            sink += bb.minPt.x;
        }
    });

    RunBenchmark("BM_RitterSphere_1024pts", 100'000ULL, [](u64 n) {
        volatile float sink = 0.0f;
        for (u64 i = 0; i < n; ++i) {
            auto s = RitterSphere(cloud, PTS);
            sink += s.radius;
        }
    });

    return 0;
}
