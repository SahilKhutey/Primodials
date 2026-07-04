# 10_Time_Framework

This document outlines simulation clocks, deterministic update intervals, and historical replay snaps.

---

## 1. Fixed Timestep Schedulers
To ensure bit-identical replication across systems, time steps are represented using 64-bit integer values in microseconds ($\mu\text{s}$):
* **Fixed Timestep ($\Delta t_{\text{sim}}$):** Exactly $16666\mu\text{s}$ (corresponding to a 60Hz tick rate).
* **Accumulator Loop:** Real elapsed frame times ($\Delta t_{\text{frame}}$) are scaled by a Speed Multiplier and added to a tick accumulator. The engine ticks the systems repeatedly as long as the accumulator exceeds the fixed step:
  $$\text{Ticks} = \text{floor} \left( \frac{\text{Accumulator}}{\Delta t_{\text{sim}}} \right)$$
* **C++ Symbol:** [SimulationClock](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Simulation/SimulationClock.hpp).

---

## 2. Rendering Interpolation (Alpha)
* **Smooth Transitions:** Because rendering frame rates vary, visual positions are interpolated between the previous and current physics frame using the remaining accumulator fraction:
  $$\alpha = \frac{\text{Accumulator}}{\Delta t_{\text{sim}}}$$
  $$\text{Render Position} = x_{t-1} \times (1.0 - \alpha) + x_t \times \alpha$$

---

## 3. Speed Controls & Snap Recording
* **Speed Scaling:** Speed multipliers range from 1x to 100000x for fast-forward simulations.
* **History State snapshots:** State coordinates are cached periodically inside snapshot buffers to support rollback, replay, and network synchronizations.
* **C++ Symbols:** [SimulationHistory](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Simulation/HistoryState.hpp) and [TickSnapshot](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Simulation/HistoryState.hpp).
