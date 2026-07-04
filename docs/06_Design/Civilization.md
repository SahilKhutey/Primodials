# Civilization

This document specifies colony storage logistics, supply-demand pricing curves, and Sentinel policies.

---

## 1. Storage Depots & Carrier Pipelines
Colonies coordinate resource transportation using carrier tasks:
* Depot inventories store food, minerals, and tools.
* Carrier shapes pan along paths calculated from the Delaunay triangulation route graph.

---

## 2. Supply-Demand Pricing
Depot interfaces calculate pricing variables based on stock levels:

$$P_{\text{resource}} = P_{\text{base}} \times \left( \frac{\text{Capacity}}{\text{Stock} + 1.0} \right)^k$$

If stock is low, price rises, prompting carrier shapes to target and import resources.

---

## 3. Sentinel Coordination Policies
Sentinels project a `Morale` influence field that broadcasts policy overrides to nearby shapes:
* **Harvest Policy:** Increases the base priority of resource gathering during starvation crises.
* **Defense Policy:** Directs shapes to withdraw to Sentinel coordinates when threat fields rise.
