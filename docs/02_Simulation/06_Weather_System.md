# 06_Weather_System

This document specifies discrete weather events — wind, rain, storms, and drought — and their mechanical effects on the simulation.

---

## 1. Weather as Stochastic Events

Weather events are generated stochastically based on current climate state, using the `SimRNG` for determinism (see [18_Deterministic_Simulation](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/18_Deterministic_Simulation.md)):

$$P(\text{storm}) = f(T_{\text{regional}}, H_{\text{regional}}, \Delta T_{\text{gradient}})$$

High temperature gradients adjacent to high humidity zones dramatically increase storm probability.

---

## 2. Weather Event Types

| Event Type | Trigger Conditions                      | Duration      | Mechanical Effects                                        |
|-----------|------------------------------------------|---------------|-----------------------------------------------------------|
| **Rain**      | High humidity (> 0.7)                | 30–120 ticks  | +Fertility; slows fire; refills water pools               |
| **Drought**   | Low humidity (< 0.2) + high temp     | 300–1200 ticks| −Fertility; depletes water; increases starvation rates     |
| **Wind Storm**| High temp gradient between cells     | 10–60 ticks   | Adds force vector to all entities in affected cells        |
| **Blizzard**  | Low temperature (< 0.15) + high wind | 60–300 ticks  | Severe movement penalty; freezes water resources          |
| **Heat Wave** | T > 0.85 for sustained period        | 120–600 ticks | Accelerated metabolic drain; high starvation risk          |

---

## 3. Wind Force Integration

During a wind storm, an environmental wind force vector is added to every dynamic entity within the affected region each tick:

$$F_{\text{wind}} = \rho_{\text{air}} \times v_{\text{wind}}^2 \times A_{\text{profile}} \times \hat{d}_{\text{wind}}$$

where:
- $\rho_{\text{air}}$ = air density constant
- $v_{\text{wind}}$ = wind speed (scales with storm intensity)
- $A_{\text{profile}}$ = entity frontal area (proportional to bounding radius)
- $\hat{d}_{\text{wind}}$ = normalized wind direction vector

This force is injected into the physics accumulation before `PhysicsSystem::Update()` runs.

---

## 4. Cognitive Impact on Shapes

Weather events generate **threat influence fields** in affected cells. These are picked up by nearby entities' sensory queries:
* **Storm** → raises Fear; increases Escape utility.
* **Rain** → lowers Fear; increases Gather utility (food regrowth expected).
* **Blizzard** → raises Fatigue; forces Rest or Shelter behaviors (future).

---

## 5. Weather Forecasting (Knowledge)

Sentinels with a high knowledge accumulation can observe temperature gradient data and issue **weather predictions** to nearby shapes — an emergent form of early warning that increases colony survival rates. This reflects the [14_Information_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/14_Information_Framework.md) propagation model.
