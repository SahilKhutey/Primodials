# 🌿 PRIMODIALS — Biome Authoring Tools Overview

## 🎯 MISSION

**Make biome creation accessible to anyone — zero code required.**

The most impactful way to expand Primordials is through user-generated biomes. The visual authoring suite enables creators, educators, and players to build, simulate, validate, and share living ecosystems directly in the browser.

---

## 🎯 KEY CAPABILITIES

1. **Visual Parameter Control:** Sliders, color wheels, shape pickers, and behavior profiles.
2. **Instant Live Simulation Preview:** Continuous 60 FPS feedback running parallel in Canvas 2D or WebGPU.
3. **Real-time Diagnostic Validation:** 50+ rules detecting starvation loops, schema violations, and unreferenced trophic dependencies.
4. **Automated GitHub PR & JSON Export:** Formatted, production-ready biome schemas downloadable with a single click or pre-filled as GitHub pull requests.
5. **Integrated Community Gallery:** Cloud repository to browse, rate, download, and fork ecosystems.

---

## 🏗️ SYSTEM TOPOLOGY

```
┌────────────────────────────────────────────────────────────┐
│                    Editor UI (React 18)                    │
│ ├─ Left: Species Roster & Hierarchy                        │
│ ├─ Center: Real-Time Live Preview Canvas                   │
│ ├─ Right: Multi-Tab Parameter & Rules Inspector            │
│ └─ Top: Mode Switcher (Visual / Live / Raw JSON)           │
└─────────────────────────────┬──────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│                 Validation Engine (Worker)                 │
│ └─ Schema, bounds, trophic food web loops, & stability     │
└─────────────────────────────┬──────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│              Live Physics & Ecology Simulator              │
│ └─ Dual Canvas/WebGPU runtime emitting telemetry stats     │
└─────────────────────────────┬──────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│                  Export & Gallery Pipeline                 │
│ ├─ Formatted Schema JSON generation                        │
│ ├─ Automated GitHub PR Deep-link generator                 │
│ └─ Supabase Community Gallery synchronization             │
└────────────────────────────────────────────────────────────┘
```
