#include "ShapeEngine/Math/Matrix2.hpp"
#include "Core/Platform.hpp"
#include <chrono>
#include <iostream>

using namespace Shape::Math;
using namespace Shape::Math::Literals;

void RunMatrix2Benchmarks() {
    std::cout << "--- Running Matrix2 Math Benchmarks ---" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    volatile float dummy = 0.0f;
    Matrix2f m1(4.0f, 7.0f, 2.0f, 6.0f);
    Matrix2f m2 = Matrix2f::Identity();
    Vector2 v(5.0f, 2.0f);
    u64 iterations = 50'000'000ULL;

    for (u64 i = 0; i < iterations; ++i) {
        Matrix2f m3 = m1 * m2;
        dummy += Determinant(m3);
        Vector2 v2 = m3 * v;
        dummy += v2.x;
        auto inv = SafeInverse(m3);
        if (inv) {
            m2 = *inv;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "BM_Matrix2 iterations: " << iterations << ", Time: " << elapsed.count() << " ms" << std::endl;
}

int main() {
    RunMatrix2Benchmarks();
    return 0;
}
