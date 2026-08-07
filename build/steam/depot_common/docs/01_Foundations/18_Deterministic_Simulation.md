# 18_Deterministic_Simulation

This document defines the determinism contract — the guarantee that the simulation produces identical output given identical inputs, regardless of platform, hardware, or execution timing.

---

## 1. The Determinism Contract
> **Definition:** A simulation is **deterministic** if and only if:
> Given the same initial state $S_0$ and the same sequence of inputs $[I_1, I_2, \dots, I_n]$, the simulation always produces the same final state $S_n$, on every machine, in every run.

This contract is the foundation of:
* **Replay Systems:** Record only initial seed + input events; replay by re-simulating.
* **Network Synchronization:** Peers share only inputs, not state; each peer simulates identically.
* **Regression Testing:** Unit tests assert deterministic state values after N ticks.

---

## 2. Sources of Non-Determinism (Must Be Eliminated)
| Source                     | Solution                                                 |
|---------------------------|----------------------------------------------------------|
| `float` arithmetic variance | Use `-fno-fast-math` compiler flag; never use `std::rand()` |
| `std::unordered_map` ordering | Use sorted or indexed maps for iteration              |
| Thread scheduling          | All simulation systems run single-threaded on one logical core |
| System clock queries       | Use fixed tick counter, never `SDL_GetTicks()` for simulation |
| Pointer addresses          | Never use raw pointer values as sort keys               |
| `rand()` without seeding   | Use deterministic LCG or MT19937 seeded from saved seed |

---

## 3. Fixed Timestep Guarantee
The simulation enforces a **fixed timestep** of exactly $16666\mu\text{s}$ per tick. No system may use real elapsed time inside a simulation update:

```cpp
// FORBIDDEN inside simulation systems:
float dt = sdl_get_real_delta(); // ❌ Non-deterministic

// REQUIRED:
constexpr f32 FIXED_DT = 1.0f / 60.0f; // ✅ Deterministic
```

---

## 4. Deterministic Random Number Generation
All stochastic systems (mutation, weather, spawning) use a shared `SimRNG` seeded from the initial world seed:

```
SimRNG {
    seed   : u64   // World seed (saved with every snapshot)
    state  : u64   // Current LCG state
}
```

**LCG Formula:**
$$\text{state}_{t+1} = (a \times \text{state}_t + c) \mod m$$
with constants $a = 6364136223846793005$, $c = 1442695040888963407$, $m = 2^{64}$.

---

## 5. Snapshot & Verification
After every N ticks, a simulation hash is computed from the aggregate state of all entities:
$$H_t = \text{CRC32}\left(\text{concat}(x_1, y_1, E_1, x_2, y_2, E_2, \dots)\right)$$

In multiplayer or replay mode, all peers assert identical hashes at the same tick. A mismatch indicates a non-determinism bug and triggers a **desync report**.
