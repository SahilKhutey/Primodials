# 🎯 PRIMORDIALS — Native Engine Performance Targets & KPI Metrics

---

## 🏆 BENCHMARK TARGETS (Native C++23)

| Benchmark Metric | Current Baseline | Optimized Target | Stretch Goal |
| :--- | :--- | :--- | :--- |
| **Active Entities @ 60 FPS** | 250,000 | **500,000** | **1,000,000+** |
| **Engine Cold-Start Time** | 1.10 s | **< 450 ms** | **< 200 ms** |
| **Memory Footprint (per 1k entities)** | 3.20 KB | **1.80 KB** | **1.20 KB** |
| **Draw Calls per Frame (100k scene)** | ~10,000 | **< 25** | **1 (Instanced)** |
| **Frame Time (P99 @ 250k entities)** | ~15.8 ms | **< 7.5 ms** | **< 4.2 ms** |
| **Clean Build Duration (Ninja)** | ~4.5 min | **< 1.5 min** | **< 45 sec** |
| **Stripped Binary Size** | ~7.8 MB | **< 3.5 MB** | **< 1.8 MB** |

---

## 📊 PROFILING PROTOCOL & TOOLING

- **Sampling Profiler:** Tracy Profiler (v0.10+) via scoped zone telemetry markers.
- **Micro-benchmarks:** Catch2 v3 Benchmark harness with nanosecond-level clock precision.
- **Hardware Counter Monitoring:** AMD uProf / Intel VTune for cache-miss rates ($L1/L2/L3$) and branch misprediction rates.
- **Memory Profiling:** Heaptrack & LLVM AddressSanitizer/LeakSanitizer (`-fsanitize=address,undefined`).
