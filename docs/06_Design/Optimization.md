# Optimization

This document specifies cache optimization guidelines, level-of-detail (LOD) updates, and performance targets.

---

## 1. Cache Locality Guidelines
To optimize CPU cache utilization:
* Components must be stored in contiguous memory blocks.
* Iterations run directly over dense arrays in Sparse Sets.
* Large components are split into separate structs to prevent cache line waste during system iterations.

---

## 2. Level-of-Detail (LOD) Tick Decoupling
To scale simulations to millions of entities:
* **High Priority (FOV):** Entities visible in the viewport update their cognitive and physics states every tick (60 Hz).
* **Medium Priority (Off-screen):** Off-screen entities update their states every 5 ticks (12 Hz).
* **Dormant:** Distant entities suspend updates, freezing active actions until re-entered by other shapes.

---

## 3. Performance Targets
* **L1 Cache Miss Rate:** Target under 2%.
* **Tick Budget:** Total simulation update tick time must remain under $12.0\text{ms}$ at 10,000 active entities.
