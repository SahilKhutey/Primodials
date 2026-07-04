# 03_Evolution_Framework

This document specifies the Evolution Framework.

---

## 1. Core Specifications

* **Purpose:** Evolve shape populations through selection pressures, crossovers, and mutations.
* **Responsibilities:** Selecting parent partners, combining parent genomes, applying Gaussian mutations, and macro vertex shifts.
* **Interfaces:** `IGeneticOperator`, `ISelectionMechanism`.
* **Managers:** `EvolutionManager`.
* **Systems:** `ReproductionSystem` (spawns child entities), `MutationSystem` (applies genetic drift).
* **Components:** `GeneticsComponent`.
* **Events:** `ReproductionEvent` (published on child birth), `MutationEvent` (published on macro vertex shift).
* **Resources:** Base mutation probabilities, vertex transition configurations.
* **Algorithms:** Uniform crossover, Gaussian point mutation distributions.
* **Threading:** Confined to Phase 3 CommandBuffer flushes on a single thread to guarantee deterministic allocations.
* **Serialization:** Saves lineage tracking hashes (`lineageID`) for lineage tree reconstruction.
* **Testing:** Assert that offspring traits inherit values within parent ranges.
* **Debugging:** Exposes phylogenetic lineage tree branches in diagnostics overlay dashboards.
