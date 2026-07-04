# 16_Diagnostics_Dev_Overlay

This document specifies the dev metrics HUD, performance timeline graphs, and entity allocation gauges.

---

## 1. Frame Time Graphs
The overlay plots rendering and physics loop tick times over the last 120 frames:

```
[Frame Time Timeline (ms)]
 16.6ms ───────────────────────────────────  ← Target (60Hz)
  8.3ms ─────▲───▲─────▲───────────────────  ← Active Physics Update Time
        ─────┴───┴─────┴───────────────────
        0                               120  Frame Index
```

---

## 2. Memory Diagnostics Gauges
* **Arena Gauge:** Displays current heap bump offset usage versus maximum capacity.
* **Pool Registry:** Displays allocation rates and recycled slot counts per component type.

---

## 3. Entity Density heatmaps
Renders a color-coded grid overlay on the Spatial Hash Grid:
* Low density cells are rendered in light green.
* High density cells (e.g. colony centers) are rendered in warning orange/red, alerting developers to potential collision bottleneck zones.
