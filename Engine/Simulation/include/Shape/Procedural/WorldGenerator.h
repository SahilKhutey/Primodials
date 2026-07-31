#pragma once

#include <cstdint>

namespace Shape::Procedural {

class WorldGenerator {
public:
    WorldGenerator() = default;
    ~WorldGenerator() = default;

    void generate(uint64_t seed) { m_seed = seed; }
    uint64_t seed() const { return m_seed; }

private:
    uint64_t m_seed = 0;
};

} // namespace Shape::Procedural
