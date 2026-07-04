# 05_Climate_System

This document specifies the global and regional climate model — temperature gradients, seasonal cycles, and long-term climate drift.

---

## 1. Climate Architecture

Climate is modeled at two scales:

| Scale     | Resolution        | Update Rate | Scope                          |
|----------|-------------------|-------------|--------------------------------|
| Global   | Single world value | 300 ticks   | Baseline temperature, seasons  |
| Regional | Per biome cell    | 60 ticks    | Local heat pockets, cold zones |

Climate provides the abiotic forcing function that shapes biome character, resource availability, and metabolic costs.

---

## 2. Seasonal Cycles

The world cycles through four seasons driven by a sinusoidal temperature oscillation:

$$T_{\text{global}}(t) = T_{\text{base}} + A_{\text{season}} \sin\left(\frac{2\pi t}{P_{\text{season}}}\right)$$

| Parameter        | Default Value   | Description                              |
|-----------------|-----------------|------------------------------------------|
| $T_{\text{base}}$ | 0.5           | Neutral baseline temperature             |
| $A_{\text{season}}$ | 0.2         | Seasonal amplitude                       |
| $P_{\text{season}}$ | 14,400 ticks | One full seasonal year (≈ 4 sim-hours at 60Hz) |

Seasons:
| Season | $T$ Range   | Effect                                             |
|--------|-------------|---------------------------------------------------|
| Spring | 0.4 – 0.6  | Resource regeneration bonus (+25%)                 |
| Summer | 0.6 – 0.8  | Metabolic drain (+15%); deserts expand             |
| Autumn | 0.4 – 0.6  | Food storage urgency rises; migration begins       |
| Winter | 0.1 – 0.3  | Severe metabolic drain (+40%); reproduction halted |

---

## 3. Regional Heat Propagation

Heat spreads between adjacent environment cells using a discrete diffusion equation:

$$T_{i,j}^{t+1} = T_{i,j}^t + \kappa \nabla^2 T_{i,j}^t$$

where $\kappa$ is the thermal conductivity of the biome (forests insulate more than deserts).

**Heat Sources:** Volcanic biomes, large populations of active entities, and fire events raise local cell temperatures. These feed back into entity metabolic costs and resource availability.

---

## 4. Climate Change Pressure

Over thousands of ticks, persistent behavioral patterns can alter the regional climate:

| Entity Behavior           | Long-Term Climate Effect                            |
|--------------------------|-----------------------------------------------------|
| Massive deforestation     | Reduced humidity → increased desertification        |
| Large-scale construction  | Increased local heat (urban heat islands)           |
| High CO₂ from metabolism  | Gradual global temperature rise                     |

This creates a subtle but powerful emergent feedback loop between civilization development and environmental conditions.
