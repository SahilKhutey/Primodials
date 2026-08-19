# 🧪 PRIMORDIALS — Native Engine Testing & Verification Strategy

---

## 🔬 TEST SUITE TOPOLOGY

- **Unit Tests (`tests/unit/`):** Pure mathematical determinism and component lifetime tests via Catch2.
- **Micro-benchmarks (`tests/benchmarks/`):** Nanosecond-level latency verification of SIMD distance calculations and spatial hash binning.
- **Fuzz Testing (`tests/fuzz/`):** LLVM libFuzzer suites for robust validation of custom user biome JSON parsing.
