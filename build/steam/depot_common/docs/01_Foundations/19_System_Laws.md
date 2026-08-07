# 19_System_Laws

This document enumerates the invariant system laws that every engine subsystem must respect. These are non-negotiable contracts enforced through assertions, unit tests, and validation frameworks.

---

## Law 1 — Layer Dependency (No Upward Calls)
A system at layer $L$ may only depend on systems at layer $L-1$ or below.

```
Physics  → may call Math          ✅
Cognition → may call Physics       ✅
Physics  → may call Cognition      ❌  (upward call — forbidden)
```

---

## Law 2 — No Global Mutable State
No system may maintain global mutable variables accessible from outside its module boundary. All state is stored in ECS components and queried through the World interface.

---

## Law 3 — Deferred Structural Mutations
No system may add or remove entities or components during its own update iteration. All such mutations must be submitted to a `CommandBuffer` and flushed **after** all systems complete their update pass.

```cpp
// FORBIDDEN during system update:
world.CreateEntity(); // ❌ Iterator invalidation

// REQUIRED:
commandBuffer.DeferCreateEntity(); // ✅ Flushed post-tick
```

---

## Law 4 — Conservation Assertion
After every tick, the total inventory of energy + resources must satisfy:
$$|E_{\text{total},t+1} - E_{\text{total},t}| \le \varepsilon_{\text{tolerance}}$$
Violations trigger a `SHAPE_ASSERT` and are logged with the tick number and delta magnitude.

---

## Law 5 — No Heap Allocations During Tick
Simulation systems must not call `new`, `malloc`, or `std::vector::push_back` (when reallocation is possible) during a tick update. All transient memory must be allocated from the `ArenaAllocator` which is reset each tick.

---

## Law 6 — Every Decision Is Inspectable
Every utility score computed during AI decision evaluation must be queryable at runtime. The `Sentinel Debug Overlay` and unit test harnesses must be able to print:
```
Entity #42 | Tick 1200
  Gather:    7.34
  Explore:   2.11
  Escape:    0.84
  Reproduce: 1.20
  → Selected: Gather
```

---

## Law 7 — Reproducible Tests
Every unit test must produce identical output across runs. Tests must not use `time()`, `rand()`, or real timestamps as inputs. Randomized tests must fix a known seed value.
