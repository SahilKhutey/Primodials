# ShapeEngine & Polygonal Primordials Benchmarks

Reproducible performance figures collected from the automated `bench_runner` benchmark suite.

## 1. Archetype ECS Iteration Performance

| Entities | Iteration Time (ms) | Memory (MB) | Est. Max FPS | 16.6ms Budget Status |
|----------|--------------------|-------------|--------------|----------------------|
|     1000 |              0.022 |        0.06 |      45747.7 | ✅ Pass (60+ FPS) |
|    10000 |              0.189 |        0.61 |       5281.0 | ✅ Pass (60+ FPS) |
|   100000 |              1.835 |        6.10 |        544.9 | ✅ Pass (60+ FPS) |
|   250000 |              4.787 |       15.26 |        208.9 | ✅ Pass (60+ FPS) |

## 2. Key Findings & Guidelines

- **250K Entities @ 60+ FPS**: Archetype SoA memory packing delivers cache-coherent entity iteration under the 16.6ms frame budget.
- **Memory Consumption**: Memory scales linearly at ~64 bytes/entity for typical Transform + Geometry component sets.
- **Reproducibility**: Run `./build/bin/bench_runner.exe` to re-execute performance verification locally.
