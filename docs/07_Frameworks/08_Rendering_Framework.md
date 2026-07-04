# 08_Rendering_Framework

This document specifies the Rendering Framework.

---

## 1. Core Specifications

* **Purpose:** High-performance, decoupled rendering of simulation states.
* **Responsibilities:** Extracting entity visual transforms, building graphics render graphs, updating GPU pipeline states, and drawing visual shapes.
* **Interfaces:** `IRenderPipeline`, `IGraphicsDevice`.
* **Managers:** `RenderManager`, `ShaderManager`.
* **Systems:** `RenderExtractionSystem`.
* **Components:** `GeometryComponent`, `TransformComponent` (visual offset buffers).
* **Events:** `FrameDrawnEvent` (emitted at screen swap).
* **Resources:** Font files, shader programs, texture maps.
* **Algorithms:** Render graph compilation, CPU viewport culling, visual lerp position interpolation.
* **Threading:** Runs on a dedicated rendering thread, decoupled from the simulation loop thread.
* **Serialization:** Core rendering configurations are saved.
* **Testing:** Asserts render extraction returns identical datasets under high entity counts.
* **Debugging:** Toggle diagnostics performance timeline bars and allocations widgets.
