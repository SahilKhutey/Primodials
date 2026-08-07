# 14_Information_Framework

This document defines the information model — how knowledge is perceived, stored, decayed, shared, and acted upon at individual and civilization scales.

---

## 1. Information as a Physical Resource
Information in this simulation is treated as a measurable, conserved quantity subject to:
* **Entropy:** Information degrades over time without reinforcement.
* **Bandwidth Limits:** Communication channels have maximum transmission capacities.
* **Propagation Delays:** Information spreads at finite speed through spatial fields and contact networks.

---

## 2. Perception Layer (Sensory Queries)
Every entity perceives the world by querying the Spatial Hash Grid for nearby entities within a **Sensory Radius** ($R_{\text{sense}}$):
$$\mathcal{P} = \{ e \in \text{Entities} \mid \|p_e - p_{\text{self}}\| \le R_{\text{sense}} \}$$

Perceived entities are filtered by type (resource, threat, ally) and written into the entity's short-term observation buffer.

---

## 3. Memory System
Each entity maintains a **Memory Log** — a bounded ring buffer of past observations:

```
MemoryEntry {
    position  : Vector2   // World coordinate of observed node
    strength  : f32       // Importance/richness (0.0 – 100.0)
    age       : f32       // Elapsed time since observation (seconds)
    type      : MemType   // Food, Threat, Ally, Unknown
}
```

**Decay Rule:** Memory entries weaken over time:
$$\text{Strength}_{t+1} = \text{Strength}_t - R_{\text{decay}} \times \Delta t \quad \text{if age} > T_{\text{threshold}}$$
Entries with Strength $\le 0.0$ are evicted from the buffer.

**Reinforcement:** Re-observing the same node resets strength to its full value, resetting the decay timer.

---

## 4. Distributed Knowledge (Sentinel Broadcasts)
Information spreads collectively through the civilization's Sentinel network:
* **Sentinel Emission:** Sentinel entities project spatial influence fields that carry coordinate data of discovered resources and threats.
* **Field Reception:** Entities entering a Sentinel's command radius receive a memory sync, inserting new entries directly into their local Memory Log.
* **Network Propagation:** Adjacent Sentinels exchange memory tables via direct contact, allowing knowledge to propagate across the entire civilization even without a global broadcast.

$$\text{Knowledge Propagation Delay} \approx \frac{d(\text{source}, \text{target})}{R_{\text{sentinel}}}$$

---

## 5. Belief vs. Ground Truth
Entities act on *beliefs* derived from their Memory Log, not omniscient ground truth. This introduces realistic misinformation dynamics:
* An entity may navigate to a food coordinate that has since been consumed — reinforcing the need for fresh observations.
* Civilizations operating on stale intelligence allocate resources inefficiently, creating a **natural selection pressure for better information systems**.
