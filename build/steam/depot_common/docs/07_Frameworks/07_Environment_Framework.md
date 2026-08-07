# 07_Environment_Framework

This document specifies the Environment Framework.

---

## 1. Core Specifications

* **Purpose:** Manages the abiotic ecosystem: biomes, terrain heightmaps, climate temperature fields, and weather events.
* **Responsibilities:** Generating fBm terrain structures, updating regional climates, simulating storm vectors, spawning resource nodes.
* **Interfaces:** `ITerrainGenerator`, `IClimateModel`.
* **Managers:** `EnvironmentManager`.
* **Systems:** `ClimateSystem` (seasonal calculations), `WeatherSystem` (precipitations and wind vectors).
* **Components:** `ResourceComponent`.
* **Events:** `WeatherEventChanged` (emitted on storms trigger), `ResourceDepletedEvent` (emitted when resource richness hits zero).
* **Resources:** Base fertility map tables, height map coordinates.
* **Algorithms:** Stacked Perlin fractional Brownian motion, Whittaker biome mapping, thermal heat diffusion equations.
* **Threading:** Climate maps update at L3 (every 60 ticks) on background workers.
* **Serialization:** Saves current environment cell temperature grids and resource node counts.
* **Testing:** Verifies that rain fires are suppressed when water cells overlap them.
* **Debugging:** Renders a color-coded temperature grid and biome boundaries in the editor overlays.
