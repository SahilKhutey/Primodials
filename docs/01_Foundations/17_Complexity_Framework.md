# 17_Complexity_Framework

This document defines how the engine manages complexity growth, prevents runaway feedback loops, and maintains computational stability at scale.

---

## 1. Complexity Classes in the Simulation
The simulation contains interactions across multiple complexity classes:

| Class     | Example                                          | Algorithm Complexity |
|-----------|--------------------------------------------------|----------------------|
| $O(1)$    | Component read/write                             | Hash set indexing    |
| $O(N)$    | System iteration over all entities of a type     | Contiguous arrays    |
| $O(N \log N)$ | Sorted event queue processing               | Priority heaps       |
| $O(N^2)$  | Naive pair-wise interaction checks (forbidden)   | Must be partitioned  |

> **Rule:** No system may perform unpartitioned $O(N^2)$ pair checks. All spatial queries must go through the Spatial Hash Grid ($O(N)$ amortized per tick).

---

## 2. Feedback Loop Classification
All feedback loops in the simulation must be classified as either **stabilizing** (negative) or **runaway** (positive), and managed accordingly:

| Loop Type    | Description                                      | Management Strategy                      |
|-------------|--------------------------------------------------|------------------------------------------|
| **Stabilizing (-)** | High population → resource scarcity → population decline | Natural; no intervention needed |
| **Runaway (+)**     | Large colony → faster resource gather → even larger colony | Cap via carrying capacity or tech cost curves |

**Carrying Capacity Formula:**
$$\frac{dN}{dt} = r N \left(1 - \frac{N}{K}\right)$$
where $N$ is population, $r$ is intrinsic growth rate, and $K$ is the environmental carrying capacity.

---

## 3. Simulation Stability Invariants
These invariants must hold at all times or trigger assertion failures:

* **Energy Conservation:** $\sum E_{\text{all entities}} + \sum E_{\text{resources}} = E_{\text{initial}} \pm \varepsilon_{\text{tolerance}}$
* **No Orphaned Entities:** Every active entity ID must be registered in at least one component store.
* **No Negative Energy:** Metabolic energy $E_m \ge 0.0$ at all times. Starvation reduces health, not energy below zero.

---

## 4. Scalability Tiers
The engine must maintain target tick rates at increasing population scales:

| Population | Target Tick Rate | Primary Bottleneck       |
|-----------|-----------------|--------------------------|
| 0–1,000    | 60 Hz           | None                     |
| 1,000–10,000 | 60 Hz        | Spatial queries          |
| 10,000–100,000 | 30 Hz      | Cognitive + Physics      |
| 100,000–1,000,000 | 10 Hz  | Rendering + Memory       |

Scalability is achieved through **Level-of-Detail (LOD) simulation**: distant or inactive entities are simulated at a coarser tick frequency while local clusters run at full fidelity.
