# 05_Rendering_Architecture

This document specifies the rendering architecture, abstract drawing interfaces, and tick interpolation.

---

## 1. Abstract Drawing Interface
To prevent simulation systems from depending directly on graphics APIs, drawing is abstracted:
* **API Wrapper:** A virtual base interface defining drawing primitives (lines, circles, convex polygons).
* **Render Pipeline:** The client maps this interface to graphics backends (SDL_Renderer, OpenGL, Vulkan) without changing core engine code.

---

## 2. Platform Wrapper (SDL3)
* **Window Handling:** Encapsulates initialization, event loops, screen resizing, and input queries.
* **Input Polling:** Maps keyboard/gamepad events and pushes them to the engine's event queue.

---

## 3. Render Interpolation
Visual positions are interpolated to ensure smooth rendering on high refresh rate displays:

$$\mathbf{p}_{\text{render}} = \mathbf{p}_{t-1} \times (1.0 - \alpha) + \mathbf{p}_t \times \alpha$$

* **$\alpha$ Ratio:** Calculated from the leftover fraction in the clock accumulator:
  $$\alpha = \frac{\text{Accumulator}}{\Delta t_{\text{sim}}}$$
This separates physics simulation ticks from rendering refresh rates.
