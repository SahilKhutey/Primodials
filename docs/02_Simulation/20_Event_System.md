# 20_Event_System

This document specifies the simulation event bus — how discrete events are generated, queued, dispatched, and consumed by subscriber systems.

---

## 1. Event System Philosophy

Events decouple systems: a source system emits an event without knowing who will respond. Subscriber systems react asynchronously on the next flush. This prevents circular system dependencies while preserving the strict tick ordering (see [03_World_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/03_World_System.md)).

```
Source System → EventQueue.Push(event) → [End of Tick] → Subscriber Systems Read
```

---

## 2. Event Type Registry

All event types are strongly typed structs:

```cpp
struct CollisionEvent {
    Entity    entityA;
    Entity    entityB;
    Vector2   normal;
    f32       impulse;
    f32       damageA;
    f32       damageB;
};

struct DeathEvent {
    Entity    deceased;
    Vector2   position;
    f32       mass;
    DeathCause cause;   // Starvation, Combat, Age, Environment
};

struct ResourceDiscoveredEvent {
    Entity    discoverer;
    Entity    resource;
    Vector2   position;
    f32       richness;
};

struct ReproductionEvent {
    Entity    parent;
    Entity    offspring;
    Vector2   spawnPosition;
};

struct TerritoryContestedEvent {
    Entity    colonyA;
    Entity    colonyB;
    Vector2   contestedCell;
};
```

---

## 3. Event Queue Architecture

```cpp
class EventQueue {
    std::vector<CollisionEvent>          collisions;
    std::vector<DeathEvent>             deaths;
    std::vector<ResourceDiscoveredEvent> discoveries;
    std::vector<ReproductionEvent>       births;
    std::vector<TerritoryContestedEvent> conflicts;

    void Push(CollisionEvent e)  { collisions.push_back(e); }
    void Flush();                // Called at tick end; clears all queues
};
```

**No heap allocation during tick:** All event vectors are pre-reserved at world initialization with estimated capacities to avoid dynamic reallocation during hot tick paths (System Law 5).

---

## 4. Event Subscriber Pattern

Systems subscribe to event types they care about, processed at the start of the subsequent tick:

| Event Type              | Primary Subscribers                                      |
|------------------------|----------------------------------------------------------|
| `CollisionEvent`        | BiologySystem (health), CognitiveSystem (fear)           |
| `DeathEvent`            | EcosystemTracker, SentinelSystem (reallocation trigger)  |
| `ResourceDiscoveredEvent` | MindComponent memory writer, Sentinel broadcaster     |
| `ReproductionEvent`     | PopulationTracker, GeneticsLogger                        |
| `TerritoryContestedEvent` | MilitarySystem, DiplomacySystem (future)             |

---

## 5. Event Analytics & History

The event queue feeds a **statistics collector** that aggregates time-series data:

| Metric                | Derivation                                   |
|----------------------|----------------------------------------------|
| Deaths per tick       | Count of `DeathEvent` by cause per tick      |
| Birth rate            | Count of `ReproductionEvent` per tick        |
| Combat frequency      | Count of `CollisionEvent` with damage > 0    |
| Discovery rate        | Count of `ResourceDiscoveredEvent` per tick  |
| Territory conflicts   | Count of `TerritoryContestedEvent` per tick  |

These metrics are exported to the analytics overlay, research replay system, and multiplayer state hash (see [18_Deterministic_Simulation](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/18_Deterministic_Simulation.md)).
