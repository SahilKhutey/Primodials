# Systems

This document catalogs the 16 core systems of the engine, defining the purpose, inputs, outputs, and dependencies for each.

---

## 1. Systems Matrix

| System | Purpose | Primary Inputs | Primary Outputs | Dependencies |
|--------|---------|----------------|-----------------|--------------|
| **Core** | Fundamental types & compiler macros | None | Basic types, macros | None |
| **Platform** | OS services insulation (SDL3) | Keyboard, mouse, timing | Raw input events | Core |
| **Memory** | Custom allocators (Pool/Arena) | Pre-allocated heap block | Memory offsets, blocks | Core |
| **Math** | Math constructs (Vector, Matrix) | Transform angles, vertices | Areas, impulses | Core |
| **Container** | Contiguous dense-sparse sets | Entity indices | Component slots | Memory |
| **Utility** | Event buffers, config, hashing | Raw config files | Synced configurations | Core |
| **Reflection**| Type metadata lookup (future) | System classes | Type descriptors | Core |
| **Logging** | Async file and console logger | Log messages queue | Log files, console output| Core |
| **Job** | Parallel task scheduler | Task graphs, threads | parallelized ticks | Platform |
| **Simulation**| Ticks scheduler & clock | Real delta time | Unified game state | Core, Utility |
| **Physics** | Kinematic kinematics & SAT | Forces, velocities | Positions, impulses | Math, Container |
| **AI** | Saliency attention, Utility AI | Needs, memory tables | Selected actions | Container, Math |
| **Civilization**| Depot inventories, technology | Research points, trade | Blueprints, pricing | Container |
| **Rendering** | Draw primitives, visual HUD | Mesh shapes, transforms | Screen pixel arrays | Platform |
| **Networking**| Lockstep packet sync, desyncs | Input packets, CRC32 hashes| Replay files | Utility |
| **Editor** | Dev widgets sandbox overlays | Entity selection | Sandbox coordinates | Rendering |

---

## 2. Dependencies Rule
Every system must respect the dependency constraints logged in the table. Cyclic dependencies (e.g. Physics depending on Rendering) are prohibited and will fail compilation.
