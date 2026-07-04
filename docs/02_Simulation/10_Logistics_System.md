# 10_Logistics_System

This document specifies the logistics model — how resources, goods, and information move between entities, buildings, and territories through transport networks.

---

## 1. Logistics as a Graph Problem

Every civilization automatically builds a **logistics graph** — a network of transport paths connecting:
* Resource extraction sites (mines, farms, water sources)
* Production facilities (workshops, refineries)
* Storage depots (shared inventories)
* Distribution points (Sentinel command areas)

```
Resource Node ──→ Gatherer Agent ──→ Storage Depot
                                          │
                                     ┌───┴────────┐
                                     ▼            ▼
                               Workshop      Distribution
                              (Refining)      (to agents)
```

---

## 2. Carrier Agents

Dedicated carrier agents (Squares with high Engineering affinity) handle inter-node transport:

```cpp
struct CarrierTask {
    Entity   source;       // Pickup location
    Entity   destination;  // Delivery location
    ResourceType cargo;
    f32      cargoQty;
    f32      progressPct;  // 0.0 → 1.0 (linear path completion)
};
```

Carriers navigate using the **Delaunay-triangulated route graph** computed by the Sentinel system (see [09_Geometry_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/09_Geometry_Framework.md)).

---

## 3. Transport Priority Queue

The logistics scheduler maintains a priority queue of pending transport tasks ranked by urgency:

$$\text{Priority} = \frac{\text{Deficit}}{\text{Current Stock}} \times \frac{1}{\text{Transport Distance}}$$

High-deficit, low-distance tasks are dispatched first. This creates emergent **just-in-time logistics** behavior — carriers automatically prioritize the most critical shortages.

---

## 4. Route Efficiency & Optimization

Over time, frequently used routes are upgraded through technology and infrastructure:
* **Dirt Path** → reduces travel time by 10%.
* **Stone Road** → reduces travel time by 25%; requires Metallurgy tech.
* **Trade Highway** → reduces travel time by 50%; requires Advanced Engineering tech.

Route upgrades emerge from high-traffic logistics patterns detected by Sentinels — no explicit road-building command is needed.

---

## 5. Logistics Failure Modes

| Failure Mode          | Trigger                                | Consequence                                     |
|----------------------|----------------------------------------|-------------------------------------------------|
| Route blockage        | Military conflict on trade path        | Supply cutoff; production stalls                |
| Carrier attrition     | Predator attacks on carrier agents     | Reduced throughput; depot shortfalls            |
| Storage overflow      | Production > consumption over time     | Wasted resources; workers idle                  |
| Depot under-capacity  | Too many consumers for one depot       | Queue congestion; starvation in outlying areas  |
