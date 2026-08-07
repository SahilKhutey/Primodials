# 07_Ecosystem_System

This document specifies the ecosystem model — the network of biological interactions (food chains, predator-prey dynamics, biomass cycling) that govern species populations at a macro scale.

---

## 1. Trophic Layers

The simulation ecosystem is organized into trophic layers:

```
Layer 3: Apex Predators (Large Polygons, N>8 sides)
         ▲
Layer 2: Consumers (Medium Polygons, 4–8 sides)
         ▲
Layer 1: Herbivores / Gatherers (Triangles, 3 sides)
         ▲
Layer 0: Resource Nodes (Food Patches, Mineral Veins)
```

Each layer feeds on the layer below. Energy transfer efficiency between trophic levels follows the 10% rule:
$$E_{\text{transferred}} = 0.10 \times E_{\text{consumed}}$$
(90% is lost as metabolic heat — see [12_Energy_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/12_Energy_Framework.md)).

---

## 2. Predator-Prey Dynamics (Lotka-Volterra)

Population oscillations between prey ($x$) and predator ($y$) follow Lotka-Volterra equations:

$$\frac{dx}{dt} = \alpha x - \beta xy$$
$$\frac{dy}{dt} = \delta xy - \gamma y$$

| Parameter | Meaning                          |
|----------|----------------------------------|
| $\alpha$ | Prey birth rate                  |
| $\beta$  | Predation rate                   |
| $\delta$ | Predator conversion efficiency   |
| $\gamma$ | Predator death rate              |

These emerge naturally from individual utility decisions — no global script enforces them. When prey is abundant, predator reproduction utility rises; when prey collapses, predators starve. The oscillations appear as a direct consequence of the utility AI system.

---

## 3. Biomass Conservation

All biomass is tracked across trophic transfers:

$$B_{\text{ecosystem}}(t) = \sum_{\text{all living}} M_i + \sum_{\text{resources}} R_j = \text{constant}$$

Mass flows:
```
Resource Node → consumed by → Gatherer
Gatherer      → consumed by → Predator
Predator      → dies        → Decay Resource Node
Decay Node    → consumed by → Gatherer
```

This closed loop prevents biomass inflation or deflation, keeping the ecosystem self-regulating over arbitrarily long simulations.

---

## 4. Species Diversity Index

The ecosystem tracks a Shannon diversity index over shape vertex distributions:

$$H = -\sum_{s} p_s \ln(p_s)$$

where $p_s$ is the proportion of the population with $s$ vertices. High $H$ indicates a healthy, diverse ecosystem; low $H$ signals monoculture dominance and vulnerability to collapse.

---

## 5. Keystone Species Mechanics

Shapes with high career ranks in specific roles act as **keystone species** — their removal causes cascading population effects. A high-rank Sentinel destroyed in a military conflict may leave dozens of shapes without memory coordination, causing them to starve from poor resource navigation.
