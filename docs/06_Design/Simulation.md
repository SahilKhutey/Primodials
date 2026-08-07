# Simulation

This document specifies the fixed-timestep simulation kernel, update scheduling order, and execution ticks.

---

## 1. Tick Execution Scheduler
To enforce 100% determinism, simulation updates execute at a fixed tick frequency of exactly $16.67\text{ms}$ ($16,666\mu\text{s}$):

```
Tick Start
 │
 ├── 1. Read Inputs (Pulls unprojected clicks and keyboard commands)
 ├── 2. Physics Update (Kinematics, SAT, spatial grid rebuild)
 ├── 3. Cognitive Update (Needs decay, spatial fields sum)
 ├── 4. Utility AI (Action evaluation and selection)
 ├── 5. Careers & Ranks (Practice skill growth, promotions)
 ├── 6. Flush CommandBuffer (Batch creates/deletes entities)
 │
 Tick End
```

---

## 2. Decoupled Frame Rates
* The simulation tick loop executes independently of rendering frame rates.
* Visual positions interpolate between physics frames using the clock accumulator fraction $\alpha$, preventing micro-stuttering on high refresh-rate monitors.

---

## 3. Statistics Collection
At the end of each tick, the simulation writes time-series metrics (active populations, starvation rates, average fitness, coordinate desync hashes) to a diagnostics collector for debugging.
