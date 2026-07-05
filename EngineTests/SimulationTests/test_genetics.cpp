/// @file test_genetics.cpp — GeneticsSystem static utility tests

#include <catch2/catch_test_macros.hpp>
#include "Simulation/Genetics/GeneticsSystem.hpp"
#include "Simulation/DeterministicRng.hpp"
#include <algorithm>

using namespace Shape;

static CreatureGenome make_genome(float fill_a, float fill_b = -1.0f) {
    CreatureGenome g;
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i)
        g.genes[i] = (fill_b < 0.0f) ? fill_a : (i % 2 == 0 ? fill_a : fill_b);
    g.lineage_hash = GeneticsSystem::compute_hash(g);
    return g;
}

TEST_CASE("Genetics — crossover child genes come from parents", "[genetics]") {
    DeterministicRng rng(42);
    const auto a = make_genome(0.0f);
    const auto b = make_genome(1.0f);
    const auto child = GeneticsSystem::crossover(a, b, rng);

    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
        REQUIRE((child.genes[i] == 0.0f || child.genes[i] == 1.0f));
    }
}

TEST_CASE("Genetics — crossover uses both parents", "[genetics]") {
    DeterministicRng rng(999);
    const auto a = make_genome(0.0f);
    const auto b = make_genome(1.0f);
    const auto child = GeneticsSystem::crossover(a, b, rng);

    bool has_a = false, has_b = false;
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
        if (child.genes[i] == 0.0f) has_a = true;
        if (child.genes[i] == 1.0f) has_b = true;
    }
    REQUIRE(has_a);
    REQUIRE(has_b);
}

TEST_CASE("Genetics — mutation stays in [0,1]", "[genetics]") {
    DeterministicRng rng(123);
    auto g = make_genome(0.5f);
    GeneticsSystem::mutate(g, 1.0f, 0.1f, rng); // 100% mutation rate
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
        REQUIRE(g.genes[i] >= 0.0f);
        REQUIRE(g.genes[i] <= 1.0f);
    }
}

TEST_CASE("Genetics — mutation rate 0 does nothing", "[genetics]") {
    DeterministicRng rng(456);
    auto g = make_genome(0.5f);
    const auto before = g;
    GeneticsSystem::mutate(g, 0.0f, 0.5f, rng);
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i)
        REQUIRE(g.genes[i] == before.genes[i]);
}

TEST_CASE("Genetics — genomic distance: identical genomes = 0", "[genetics]") {
    const auto g = make_genome(0.5f);
    REQUIRE(GeneticsSystem::genomic_distance(g, g) < 0.001f);
}

TEST_CASE("Genetics — genomic distance: opposite genomes = 1", "[genetics]") {
    const auto a = make_genome(0.0f);
    const auto b = make_genome(1.0f);
    const float d = GeneticsSystem::genomic_distance(a, b);
    REQUIRE(d > 0.99f);
}

TEST_CASE("Genetics — hash is deterministic", "[genetics]") {
    const auto g = make_genome(0.3f);
    const u64 h1 = GeneticsSystem::compute_hash(g);
    const u64 h2 = GeneticsSystem::compute_hash(g);
    REQUIRE(h1 == h2);
}

TEST_CASE("Genetics — different genomes have different hashes", "[genetics]") {
    const auto a = make_genome(0.3f);
    const auto b = make_genome(0.7f);
    REQUIRE(GeneticsSystem::compute_hash(a) != GeneticsSystem::compute_hash(b));
}

TEST_CASE("Genetics — crossover is deterministic with same seed", "[genetics]") {
    const auto a = make_genome(0.0f);
    const auto b = make_genome(1.0f);

    DeterministicRng rng1(42), rng2(42);
    const auto c1 = GeneticsSystem::crossover(a, b, rng1);
    const auto c2 = GeneticsSystem::crossover(a, b, rng2);
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i)
        REQUIRE(c1.genes[i] == c2.genes[i]);
}
