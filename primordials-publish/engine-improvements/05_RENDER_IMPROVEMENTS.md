# 🎨 PRIMORDIALS — SDL3 GPU Batch Rendering Architecture

---

## 1. Instanced Quad Rendering Pipeline

- Replaces CPU-side individual `SDL_RenderDrawPoint` / `SDL_RenderFillRect` calls with **GPU instancing via SDL3 GPU API**.
- Single vertex buffer storing base quad shape.
- Single dynamic instance buffer updated once per frame storing `[x, y, color_r, color_g, color_b, color_a, scale, rotation]`.
- Reduces draw calls from $250,000$ per frame down to **1 draw call per species archetype**.

---

## 2. GPU Frustum Culling

Rejects off-screen organisms prior to instance buffer copy, preserving memory bus bandwidth on high-resolution ($4\text{K}/8\text{K}$) displays.
