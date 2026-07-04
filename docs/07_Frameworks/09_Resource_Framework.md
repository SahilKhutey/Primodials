# 09_Resource_Framework

This document specifies the Resource Framework.

---

## 1. Core Specifications

* **Purpose:** Handles offline asset compilation, runtime database lookup, and caching.
* **Responsibilities:** Reading TTF fonts, loading sound clips, caching texture handles, managing file dependency graphs.
* **Interfaces:** `IAssetImporter`, `IAssetDatabase`.
* **Managers:** `ResourceManager`.
* **Systems:** None (executes offline or during load/initialize sequences).
* **Components:** None (interacts with global caches).
* **Events:** `AssetLoadedEvent` (published on memory load completions).
* **Resources:** Raw file directories (images, fonts, scripts).
* **Algorithms:** LRU (Least Recently Used) cache eviction pipelines.
* **Threading:** Asset loading runs on secondary threads during startup to prevent main thread blocks.
* **Serialization:** Compiles and packs text settings into binary formats.
* **Testing:** Verifies cache hit ratios and verifies that missing assets fall back to default assets.
* **Debugging:** Exposes an Asset Database Panel displaying active textures memory footprints.
