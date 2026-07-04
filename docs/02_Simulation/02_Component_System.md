# 02_Component_System

This document specifies the data layout, access patterns, and storage architecture for all ECS components.

---

## 1. Component Philosophy

A **Component** is a plain data struct — it contains only data, zero methods, and zero virtual functions. Systems provide all behavior.

```cpp
// CORRECT — plain data struct
struct RigidBodyComponent {
    Vector2 position;
    Vector2 velocity;
    f32     angle;
    f32     angularVelocity;
    f32     mass;
    f32     invMass;
    f32     inertia;
    f32     invInertia;
    f32     restitution;
    f32     drag;
};

// FORBIDDEN — behavior inside component
struct BadComponent {
    virtual void Update() {} // ❌ no virtual methods
    f32 x, y;
};
```

---

## 2. Sparse Set Storage Architecture

Each component type is stored in its own `SparseSet<T>`:

```
SparseSet<RigidBodyComponent>
 ├── sparse[]    : u32[MAX_ENTITIES]   — maps Entity.Index() → dense index
 ├── dense[]     : RigidBodyComponent[]— contiguous data array (cache-friendly)
 └── entities[]  : Entity[]            — reverse map: dense index → Entity handle
```

**Access:** $O(1)$ for random access via sparse lookup.
**Iteration:** $O(N)$ via dense array — fully cache-coherent.
**Contains check:** $O(1)$ — compare sparse[index] bounds and generation.

---

## 3. Complete Component Registry

| Component              | File                        | Description                                    |
|-----------------------|-----------------------------|------------------------------------------------|
| `RigidBodyComponent`   | Physics/RigidBodyComponent.hpp | Position, velocity, mass, inertia            |
| `GeometryComponent`    | ECS/GeometryComponent.hpp   | Vertices, area, centroid, bounding radius     |
| `LifecycleComponent`   | ECS/LifecycleComponent.hpp  | Energy, health, age, lifespan                 |
| `GeneticsComponent`    | ECS/GeneticsComponent.hpp   | 12-gene float vector                          |
| `AIComponent`          | ECS/AIComponent.hpp         | Current action, target, action timer          |
| `MindComponent`        | ECS/MindComponent.hpp       | Needs, emotions, memory log                   |
| `CareerComponent`      | ECS/CareerComponent.hpp     | Skills, rank, department interests            |
| `ResourceComponent`    | ECS/ResourceComponent.hpp   | Richness, type, depletion rate                |
| `InfluenceComponent`   | ECS/InfluenceComponent.hpp  | Emission type, strength, radius               |
| `SentinelComponent`    | Sentinel/SentinelComponent.hpp | Command radius, policy, linked Sentinels  |

---

## 4. Component Lifecycle Rules

1. **Added** via `CommandBuffer.DeferAddComponent<T>()` — flushed post-tick.
2. **Removed** via `CommandBuffer.DeferRemoveComponent<T>()` — flushed post-tick.
3. **Read/Write** directly during system updates — no deferred access needed.
4. **No constructors with side effects** — components must be trivially constructible.

---

## 5. Multi-Component Queries

Systems request a view over entities possessing all specified component types:

```cpp
// Returns a range of Entity handles having ALL listed components
auto entities = world.Query<AIComponent, RigidBodyComponent,
                            LifecycleComponent, MindComponent>();
```

The query engine intersects the sparse sets of the listed types, returning only entities that exist in all sets. Iteration is ordered by the smallest set to minimize traversal.
