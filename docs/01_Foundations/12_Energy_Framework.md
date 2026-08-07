# 12_Energy_Framework

This document defines the universal energy model governing generation, storage, transfer, dissipation, and efficiency tracking across all layers of the simulation.

---

## 1. Forms of Energy
Every entity in the simulation interacts with one or more of the following energy types:

| Energy Type      | Symbol | Description                                          |
|-----------------|--------|------------------------------------------------------|
| Kinetic          | $E_k$  | Energy of motion: $\frac{1}{2}Mv^2$                 |
| Metabolic        | $E_m$  | Biological fuel stored inside entity lifecycle       |
| Chemical         | $E_c$  | Stored in resource nodes (food, minerals)            |
| Thermal          | $E_T$  | Heat dissipated from kinetic friction/drag           |
| Cognitive        | $E_\text{cog}$ | Energy cost of running decisions and memory |

---

## 2. Metabolic Energy Model
Every living entity stores metabolic energy in the range $[0.0, 100.0]$.

**Consumption from Movement:**
$$\Delta E_m = -C_{\text{metabolism}} \times |v| \times \Delta t$$

**Consumption from Cognition:**
$$\Delta E_m = -C_{\text{think}} \times N_{\text{candidates}} \times \Delta t$$
where $N_{\text{candidates}}$ is the number of action utility candidates evaluated per tick.

**Gain from Eating:**
$$\Delta E_m = +C_{\text{absorb}} \times \text{Resource.Richness}$$

---

## 3. Energy Networks (Civilization-Scale)
Civilizations aggregate energy flows across their population:

$$E_{\text{civ}} = \sum_{i}^{N} E_{m,i}$$

**Production Rate:**
$$\dot{E}_{\text{prod}} = N_{\text{gatherers}} \times R_{\text{gather}} - N_{\text{total}} \times C_{\text{metabolism}} \times \bar{v}$$

**Energy Deficit → Starvation Cascade:** If $\dot{E}_{\text{prod}} < 0$, the civilization enters an energy deficit state, triggering emergency resource reallocation policies in the Sentinel system.

---

## 4. Dissipation (Thermal Heat)
Energy converted to heat through friction is tracked globally:
$$\Delta E_T = C_{\text{drag}} \times M \times v^2 \times \Delta t$$

This contributes to local **temperature fields**, which affect chemical reaction rates (see [07_Chemistry_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/07_Chemistry_Framework.md)).

---

## 5. Efficiency Metric
Every energy subsystem exposes a measurable efficiency score:
$$\eta = \frac{E_{\text{output}}}{E_{\text{input}}} \in [0.0, 1.0]$$

High-efficiency shapes are selected during evolution; low-efficiency shapes are out-competed.
