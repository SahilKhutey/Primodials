# 03_ECS_Architecture

This document specifies the sparse set ECS storage architecture, generational validation indices, and deferred structural mutation pipelines.

---

## 1. Generational Validation
Entity IDs contain a validation mask to prevent stale references:

```cpp
struct Entity {
    u32 index      : 20;
    u32 generation : 12;
};
```

When querying a component, the store checks if the entity is valid:
$$\text{IsValid} = (\text{SparseRegistry}[\text{index}].\text{generation} == \text{generation})$$

---

## 2. Sparse Set Storage Layout
Components are stored contiguously in memory to maximize CPU cache hit rates:
* **Sparse Array:** Bounded array indexed by Entity Index, storing indices into the Dense Array.
* **Dense Array:** Contiguous array of component data structs.
* **Entity Array:** Maps Dense Array indices back to Entity IDs, allowing swap-and-pop removals.

```
Sparse Set Removals (Swap-and-Pop)
 [Comp A] [Comp B] [Comp C] [Comp D]
                      ▲         │ (Swap target to deleted index)
                      └─────────┘
 [Comp A] [Comp B] [Comp D] ──→ Pop last element (O(1) delete, keeps array contiguous)
```

---

## 3. Deferred CommandBuffer Actions
Modifying components or destroying entities during system iteration invalidates indices. To prevent this, modifications are deferred:
* Systems write modifications to a thread-local command buffer.
* At tick completion, `CommandBuffer::Flush()` executes, executing mutations in batch mode.
