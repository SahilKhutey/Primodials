// Shape/Procedural/SimplexNoise.h
// Deterministic 2D simplex noise
#pragma once
#include <cstdint>
#include <cmath>
#include <array>

namespace Shape::Procedural {

    class SimplexNoise {
    public:
        explicit SimplexNoise(uint64_t seed = 0);

        float noise(float x, float y) const;
        float fbm(float x, float y, int octaves, float lacunarity = 2.0f, float persistence = 0.5f) const;
        float ridge(float x, float y, int octaves) const;
        float turbulence(float x, float y, int octaves) const;

        // 3D variants
        float noise3(float x, float y, float z) const;
        float fbm3(float x, float y, float z, int octaves) const;

    private:
        std::array<uint8_t, 256> m_perm;
        float grad(int hash, float x, float y) const;
        float grad3(int hash, float x, float y, float z) const;
        void build_permutation(uint64_t seed);
    };

} // namespace Shape::Procedural
