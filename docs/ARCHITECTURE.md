# 📐 Primordials — System Architecture

---

## 1. Overview

Primordials is organized into two primary tiers:

```
┌────────────────────────────────────────────────────────┐
│               1. Consumer / Web Edition                │
│       React 18 + TypeScript + Vite + Canvas 2D         │
│  - Live desktop wallpaper & Wallpaper Engine Bridge    │
│  - Neural sensory agent behaviors                      │
│  - Spatial hash grid collision detection               │
└──────────────────────────┬─────────────────────────────┘
                           │
┌──────────────────────────▼─────────────────────────────┐
│             2. Laboratory Research Engine              │
│                 Modern C++23 + SDL3                    │
│  - Custom data-oriented Entity-Component-System (ECS)  │
│  - Continuous differential equation ecology solvers    │
│  - Lockstep deterministic networking and replay        │
│  - High entity throughput (250,000+ entities @ 60 FPS) │
└────────────────────────────────────────────────────────┘
```

---

## 2. Mathematical Ecology Model

Both implementations simulate populations using continuous differential equations based on Lotka-Volterra predator-prey dynamics:

$$\frac{dx}{dt} = \alpha x - \beta x y$$

$$\frac{dy}{dt} = \delta x y - \gamma y$$

Where:
- $x$: Prey population density
- $y$: Predator population density
- $\alpha$: Prey natural growth rate
- $\beta$: Predation rate coefficient
- $\delta$: Predator reproduction efficiency per consumed prey
- $\gamma$: Predator natural mortality rate

---

## 3. Spatial Partitioning

To maintain 60 FPS across tens of thousands of entities, continuous 2D space is partitioned into a uniform spatial grid. Neighbor queries for foraging and threat detection execute in $O(1)$ amortized time per entity rather than naive $O(N^2)$ pairwise checking.
