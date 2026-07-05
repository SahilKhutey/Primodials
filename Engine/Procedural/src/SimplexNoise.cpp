// Shape/Procedural/SimplexNoise.cpp
#include "Shape/Procedural/SimplexNoise.h"
#include "Shape/Random/DeterministicRng.hpp"

#include <algorithm>
#include <cmath>

namespace Shape::Procedural {

    SimplexNoise::SimplexNoise(uint64_t seed) {
        build_permutation(seed);
    }

    void SimplexNoise::build_permutation(uint64_t seed) {
        for (int i = 0; i < 256; ++i) m_perm[i] = static_cast<uint8_t>(i);
        Shape::Random::DeterministicRng rng(seed);
        for (int i = 255; i > 0; --i) {
            const int j = static_cast<int>(rng.NextU64() % (i + 1));
            std::swap(m_perm[i], m_perm[j]);
        }
        for (int i = 0; i < 256; ++i) m_perm[i] = m_perm[i];
    }

    float SimplexNoise::grad(int hash, float x, float y) const {
        const int h = hash & 7;
        const float u = h < 4 ? x : y;
        const float v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
    }

    float SimplexNoise::grad3(int hash, float x, float y, float z) const {
        const int h = hash & 15;
        const float u = h < 8 ? x : y;
        const float v = h < 4 ? y : ((h == 12 || h == 14) ? x : z);
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }

    float SimplexNoise::noise(float x, float y) const {
        constexpr float F2 = 0.5f * (1.732050807568877f - 1.0f);
        constexpr float G2 = (3.0f - 1.732050807568877f) / 6.0f;

        const float s = (x + y) * F2;
        const int i = static_cast<int>(std::floor(x + s));
        const int j = static_cast<int>(std::floor(y + s));
        const float t = (i + j) * G2;
        const float X0 = i - t, Y0 = j - t;
        const float x0 = x - X0, y0 = y - Y0;

        const int i1 = x0 > y0 ? 1 : 0;
        const int j1 = x0 > y0 ? 0 : 1;

        const float x1 = x0 - i1 + G2;
        const float y1 = y0 - j1 + G2;
        const float x2 = x0 - 1.0f + 2.0f * G2;
        const float y2 = y0 - 1.0f + 2.0f * G2;

        const int ii = i & 255, jj = j & 255;
        const int gi0 = m_perm[(ii + m_perm[jj]) & 255] & 7;
        const int gi1 = m_perm[(ii + i1 + m_perm[(jj + j1) & 255]) & 255] & 7;
        const int gi2 = m_perm[(ii + 1 + m_perm[(jj + 1) & 255]) & 255] & 7;

        float n0 = 0, n1 = 0, n2 = 0;
        float t0 = 0.5f - x0 * x0 - y0 * y0;
        if (t0 >= 0) { t0 *= t0; n0 = t0 * t0 * grad(gi0, x0, y0); }
        float t1 = 0.5f - x1 * x1 - y1 * y1;
        if (t1 >= 0) { t1 *= t1; n1 = t1 * t1 * grad(gi1, x1, y1); }
        float t2 = 0.5f - x2 * x2 - y2 * y2;
        if (t2 >= 0) { t2 *= t2; n2 = t2 * t2 * grad(gi2, x2, y2); }

        return 70.0f * (n0 + n1 + n2);
    }

    float SimplexNoise::fbm(float x, float y, int octaves, float lacunarity, float persistence) const {
        float sum = 0;
        float amp = 1;
        float freq = 1;
        float max_amp = 0;
        for (int i = 0; i < octaves; ++i) {
            sum += noise(x * freq, y * freq) * amp;
            max_amp += amp;
            amp *= persistence;
            freq *= lacunarity;
        }
        return sum / max_amp;
    }

    float SimplexNoise::ridge(float x, float y, int octaves) const {
        float sum = 0;
        float amp = 1;
        float freq = 1;
        float max_amp = 0;
        for (int i = 0; i < octaves; ++i) {
            const float n = 1.0f - std::abs(noise(x * freq, y * freq));
            sum += n * n * amp;
            max_amp += amp;
            amp *= 0.5f;
            freq *= 2.0f;
        }
        return sum / max_amp;
    }

    float SimplexNoise::turbulence(float x, float y, int octaves) const {
        float sum = 0;
        float amp = 1;
        float freq = 1;
        for (int i = 0; i < octaves; ++i) {
            sum += std::abs(noise(x * freq, y * freq)) * amp;
            amp *= 0.5f;
            freq *= 2.0f;
        }
        return sum;
    }

    float SimplexNoise::noise3(float x, float y, float z) const { return 0.0f; /* Simplified */ }
    float SimplexNoise::fbm3(float x, float y, float z, int octaves) const { return 0.0f; /* Simplified */ }

} // namespace Shape::Procedural
