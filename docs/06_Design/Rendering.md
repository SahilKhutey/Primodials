# Rendering

This document specifies the abstract rendering architecture, render graphs, and graphics API encapsulation.

---

## 1. Render Graph Pipeline
The client abstracts graphics drawing behind a virtual base pipeline:

```
[Simulation World] ──→ [Render Extraction] ──→ [Render World] ──→ [GPU Draw Commands]
```

* **Render Extraction:** Reads required entity transforms from component stores and copies them to a double-buffered visual cache.
* **Render World:** Renders particles, paths, and grids using instanced draw commands.

---

## 2. API Wrappers
* **Graphics API Interface:** Exposes methods to compile shaders and draw primitives (lines, circles, convex polygons).
* **Backends:** Mapped to OpenGL 4.6 (Phase 1) and Vulkan (Phase 2). Core simulation files contain zero graphics library includes.

---

## 3. Frame Rate Interpolation
To smooth visual rendering when frame rates exceed simulation tick rates:
$$\mathbf{p}_{\text{render}} = \mathbf{p}_{t-1} \times (1.0 - \alpha) + \mathbf{p}_t \times \alpha$$
where $\alpha$ is the fractional remainder of the tick clock.
