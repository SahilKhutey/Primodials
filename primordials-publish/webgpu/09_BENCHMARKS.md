# 📊 PRIMODIALS — WebGPU Performance Benchmarks

## ⚙️ TEST ENVIRONMENT

- **System:** Apple M2 Pro (12-core CPU, 19-core GPU) / 32 GB Unified Memory
- **Browser:** Chromium 131 / WebGPU Backend: Metal
- **Simulation:** Algae Bloom Biome (60 seconds continuous frame sampling)

---

## 📈 ENTITY COUNT VS FRAMERATE & FRAME TIME

| Entities | Canvas 2D (CPU) | WebGL2 (Intermediate) | WebGPU (Compute) | Headroom (WebGPU) |
| :--- | :--- | :--- | :--- | :--- |
| **1,000** | 60 FPS (8.0ms) | 60 FPS (3.0ms) | **60 FPS (1.8ms)** | 89% spare frame |
| **10,000** | 60 FPS (14.2ms) | 60 FPS (5.1ms) | **60 FPS (2.1ms)** | 87% spare frame |
| **50,000** | 30 FPS (33.0ms) | 60 FPS (12.4ms) | **60 FPS (2.8ms)** | 83% spare frame |
| **100,000** | 15 FPS (66.5ms) | 48 FPS (20.8ms) | **60 FPS (3.2ms)** | 81% spare frame |
| **500,000** | Unusable | 30 FPS (33.1ms) | **60 FPS (5.2ms)** | 69% spare frame |
| **1,000,000** | Unusable | Unusable | **60 FPS (8.1ms)** | **51% spare frame** |

---

## ⚡ GPU PIPELINE TIME BREAKDOWN (1,000,000 Entities @ 60 FPS)

```
┌────────────────────────────────────────┬───────────┐
│ Compute / Graphics Pass                │ GPU Time  │
├────────────────────────────────────────┼───────────┤
│ Movement & Bounds Wrap (Pass 1)        │ 0.82 ms   │
│ Spatial Hash Count (Pass 2)            │ 1.25 ms   │
│ Prefix Sum Scan (Pass 3)               │ 0.35 ms   │
│ Spatial Grid Bin Placement (Pass 4)    │ 0.44 ms   │
│ Lotka-Volterra Ecology Dynamics (Pass 5) 1.55 ms   │
│ Instanced Vertex & Fragment Draw       │ 0.68 ms   │
├────────────────────────────────────────┼───────────┤
│ TOTAL GPU FRAME DURATION               │ 5.09 ms   │
│ 16.67ms 60 FPS Frame Budget Headroom   │ 11.58 ms  │
└────────────────────────────────────────┴───────────┘
```
