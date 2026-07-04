# 14_Procedural_World_Generation

This document specifies the Perlin noise terrain generators, resource placement formulas, and height map constraints.

---

## 1. Terrain Height Maps (Perlin Noise)
Biome elevation and humidity values are generated using stacked fractional Brownian motion (fBm) noise:

$$\text{Height}(x, y) = \sum_{i=1}^L A_i \times \text{Noise}(f_i \cdot x, f_i \cdot y)$$

* **Layers ($L$):** 4 octaves.
* **Frequency ($f_i$):** Scales by $2.0$ per octave (lacunarity).
* **Amplitude ($A_i$):** Scales by $0.5$ per octave (gain/persistence).

---

## 2. Biome Assignment
Cells are assigned biomes by mapping Height and Humidity coordinates to a Whittaker diagram:

```
Humidity
  ▲
 1.0 │ Ocean      Forest       Tundra
     │ Grassland  Grassland    Tundra
 0.0 │ Desert     Desert       Volcanic
     └───────────────────────────────►
      0.0                           1.0  Elevation / Height
```

---

## 3. Seed Determinism
The world generation process depends entirely on the 64-bit seed. Given the same seed, the noise coordinates produce identical biomes, resource coordinates, and elevations, satisfying the [18_Deterministic_Simulation](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/18_Deterministic_Simulation.md) contract.
