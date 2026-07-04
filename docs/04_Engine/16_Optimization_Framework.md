# 16_Optimization_Framework

This document specifies cache locality rules, multi-threaded worker pools, and compiler optimizations.

---

## 1. Cache Locality Rules
* **Contiguous Arrays:** Components must be aligned contiguously in memory. Linked lists are banned in performance-critical paths.
* **Component Splitting:** Large components are split into smaller structs (e.g. splitting standard physics and visual transform fields) to maximize cache line utility during iterations.

---

## 2. Multi-Threaded Region Updates
To scale to millions of entities:
* The 2D plane is divided into isolated spatial regions (see [11_Space_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/11_Space_Framework.md)).
* Disconnected regions are updated in parallel using a lock-free thread pool.
* Regions containing overlapping boundaries are synchronized on the main thread, preventing race conditions.

---

## 3. Compiler Flag Constraints
To ensure code generation efficiency:
* **Release Config:** Compiles with `-O3` (aggressive optimizations) and `-fno-fast-math` (guaranteeing IEEE-754 floating-point determinism).
* **Link Time Optimization (LTO):** Enabled on static libraries to optimize function calls across compilation units.
