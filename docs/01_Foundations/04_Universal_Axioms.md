# 04_Universal_Axioms

This document details the universal axioms governing every entity and state inside the simulation.

---

## Axiom 1 — Everything is an Entity
* **Definition:** Every object, boundary, agent, resource, building, or collective is represented uniformly as a distinct `Entity` handle.
* **Architecture:** Entities are 32-bit unique handles split into:
  - **Index (20 bits):** Points to component storage offsets.
  - **Generation (12 bits):** Generational check to verify recycled IDs.
* **C++ Symbol:** [Entity](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/Entity.hpp).

---

## Axiom 2 — Everything has State
* **Definition:** There is no hidden simulation state. Every physical, cognitive, or biological property is fully queryable in component stores.
* **C++ Symbol:** [World](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/World.hpp).

---

## Axiom 3 — Everything Evolves
* **Definition:** Nothing is static. Every entity changes through time (birth, growth, interaction, transformation, death, recycling).

---

## Axiom 4 — Every Change Has a Cause
* **Definition:** Modifications of state are strictly causal. State changes are driven by physics calculations, metabolic rates, cognitive utility choices, or spatial fields. Randomness must be modeled using deterministic seeds.

---

## Axiom 5 — Conservation
* **Definition:** Nothing appears from nothingness. Energy, matter, information, time, and population are fully conserved, allowing civilizations to calculate optimization scores.
