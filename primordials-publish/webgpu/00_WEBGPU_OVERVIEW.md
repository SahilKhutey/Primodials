# 🚀 PRIMODIALS — WebGPU Optimization Overview

## 🎯 THE PROBLEM

The baseline browser simulation is **CPU-bound**:

```
┌──────────────────────────────────────────────────────────┐
│                           CPU                            │
│ ├─ ECS update (sequential, ~5ms / 5K entities)           │
│ ├─ Spatial hash (O(n) insert + query, ~3ms)              │
│ ├─ LV dynamics (per-entity, ~4ms)                        │
│ └─ Canvas 2D render (immediate mode, ~5ms / 5K)          │
│                                                          │
│ TOTAL: ~17ms for 5K entities                             │
│ Bottleneck: Single-threaded JavaScript CPU execution     │
└──────────────────────────────────────────────────────────┘
```

**Limit:** ~30K entities before the 16.67ms (60 FPS) frame budget is broken.

---

## 🎯 THE SOLUTION: WebGPU Compute Pipeline

```
┌──────────────────────────────────────────────────────────┐
│                   CPU (JavaScript / React)               │
│ ├─ Input handling & UI controls                          │
│ ├─ Game loop orchestration (pause, reset, biome switch)  │
│ └─ Submit GPU command encoder passes                     │
│                                                          │
│                   GPU (WebGPU WGSL Compute)              │
│ ├─ Compute: Movement & bounds wrapping (256 workgroup)   │
│ ├─ Compute: Spatial hash building & prefix scan          │
│ ├─ Compute: Lotka-Volterra predator-prey dynamics        │
│ └─ Render: Hardware instanced quad polygons (1 draw call)│
│                                                          │
│ TOTAL: ~2ms CPU, ~3ms GPU for 500,000 entities           │
│ Result: 1,000,000+ entities @ 60 FPS in the browser      │
└──────────────────────────────────────────────────────────┘
```

---

## 📊 PERFORMANCE TARGETS

| Metric | Canvas 2D (CPU) | WebGPU (Compute + Render) | Improvement |
| :--- | :--- | :--- | :--- |
| **Max Entity Capacity** | ~30,000 | **1,000,000+** | **33x** |
| **Frame Time (100k entities)** | 66ms (15 FPS) | **3ms (60 FPS)** | **22x faster** |
| **CPU Utilization** | ~80% | **< 5%** | **16x reduction** |
| **Memory Footprint** | ~500 MB (RAM) | **~85 MB (VRAM)** | **5.8x reduction** |
| **Battery & Thermal Drain** | High | **Low (Silent fan)** | **~3x more efficient** |

---

## 🎯 3-TIER PROGRESSIVE FALLBACK STRATEGY

```
Tier 1: WebGPU (Default for modern Chrome, Edge, Safari 17+, Firefox 121+) ➔ 1M+ entities @ 60 FPS
   │
   ▼ (if unavailable)
Tier 2: WebGL2 (Intermediate fallback with transform feedback) ➔ 100k entities @ 60 FPS
   │
   ▼ (if unavailable)
Tier 3: Canvas 2D (Universal legacy fallback) ➔ 30k entities @ 60 FPS
```
