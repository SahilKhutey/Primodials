# 11_Editor_Framework

This document specifies the Editor Framework.

---

## 1. Core Specifications

* **Purpose:** Sandbox editing, performance timelines visualization, and entity state inspection.
* **Responsibilities:** Drawing ImGui overlays, rendering diagnostics graphs, translating viewport unprojections, modifying component registries.
* **Interfaces:** `IEditorTool`, `IDiagnosticsPanel`.
* **Managers:** `EditorManager`.
* **Systems:** `EditorSystem` (updates dev UI overlay widgets).
* **Components:** None (inspects all components).
* **Events:** `EditorSelectionChanged` (emitted on selecting entities).
* **Resources:** Layout files, theme color profiles.
* **Algorithms:** Raycast viewport screen-to-world coordinate transformations.
* **Threading:** Runs on the rendering thread, drawing editor panels on top of double-buffered visual caches.
* **Serialization:** Saves custom window layouts and active diagnostics settings.
* **Testing:** Asserts that manual modifications via inspectors are safely committed.
* **Debugging:** Toggle grids, threat boundaries, and AI need curves.
