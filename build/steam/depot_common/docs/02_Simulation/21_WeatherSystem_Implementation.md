# 21. Weather System (Implementation Reference)

## Status: **Implemented** — `Engine/Simulation/src/Systems/WeatherSystem.cpp`

> This document supplements the design specification in `docs/02_Simulation/06_Weather_System.md` with implementation details.

---

## Overview

`WeatherSystem` is an `ISimulationSystem` that drives a dynamic, season-aware environmental state machine. It exports `EnvironmentState` each tick for consumption by downstream systems (metabolism, resource growth, entity AI).

---

## Season Cycle

The simulation advances through 4 seasons in order: **Spring → Summer → Autumn → Winter → Spring...**

Each season lasts `ticks_per_season` ticks (default: `1200` at 60Hz ≈ 20 real seconds per season).

| Season | Temp Range | Humidity | Base Weather |
|---|---|---|---|
| Spring | 10–20°C | 0.6 | Rain |
| Summer | 28–38°C | 0.3 | Clear / Heatwave |
| Autumn | 5–15°C | 0.7 | Rain / Storm |
| Winter | -15–0°C | 0.5 | Blizzard / Clear |

---

## Weather State Machine

```
Clear ──→ Rain ──→ Storm
  ↑                 ↓
Heatwave ←── Clear ←──
     ↑
  Blizzard (Winter only)
```

Weather transitions fire probabilistically each tick. Transitions are seeded from `TickContext::rng` for full determinism.

---

## EnvironmentState

Exported read-only each tick via `WeatherSystem::state()`:

```cpp
struct EnvironmentState {
    Season      season;
    WeatherType current_weather;
    float       temperature_celsius;
    float       humidity;                    // 0.0 – 1.0
    Vector2f    wind_vector;
    float       metabolic_drain_multiplier;  // >1 = harder survival
    float       resource_growth_multiplier;  // >1 = more resource yield
};
```

| Extreme Weather | Metabolic Drain | Resource Growth |
|---|---|---|
| Heatwave | 2.0× | 0.3× |
| Blizzard | 2.5× | 0.1× |
| Storm | 1.5× | 0.6× |
| Clear | 1.0× | 1.0× |

---

## Public API

```cpp
WeatherSystem weather(ticks_per_season);

weather.force_season(Season::Winter);          // Override season immediately
weather.force_weather(WeatherType::Blizzard);  // Override active weather

const EnvironmentState& env = weather.state(); // Read environment state
```

---

## Integration Point

Systems that need weather context should read `EnvironmentState` from a shared `WeatherSystem*` pointer:

```cpp
float effective_drain = lifecycle.base_drain * env_state.metabolic_drain_multiplier;
```

Priority: `-40` (runs before all simulation systems, ensures environment state is up-to-date for the tick).
