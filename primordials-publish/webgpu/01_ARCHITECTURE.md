# 🏛️ PRIMODIALS — WebGPU Pipeline Architecture

---

## 📐 PIPELINE OVERVIEW

```
User Input & React UI
       │
       ▼
Simulation Controller
       │
       ▼
GPUEngine (WebGPU Context & Queues)
       │
 ┌─────┴──────────────────────────────────────────────────────┐
 │ GPU Compute & Render Passes (Single Frame Submission)       │
 │                                                            │
 │ 1. Movement Pass (Compute)       ──► Position updates & wrap│
 │ 2. Spatial Hash Count (Compute)  ──► Atomic cell bins      │
 │ 3. Prefix Scan (Compute)         ──► Cell start offsets    │
 │ 4. Spatial Sort (Compute)        ──► Cell index ordering   │
 │ 5. LV Dynamics (Compute)         ──► Predation & reproduction
 │ 6. Instanced Render (Graphics)   ──► Hardware quad draw    │
 └────────────────────────────────────────────────────────────┘
```

---

## 🗂️ GPU STORAGE BUFFER LAYOUT (for 1,000,000 entities)

| Buffer Name | Format & Dimensions | Size | Purpose |
| :--- | :--- | :--- | :--- |
| `positions` | `vec2<f32>` × 1,000,000 | 8 MB | World coordinates $(x, y)$ |
| `velocities` | `vec2<f32>` × 1,000,000 | 8 MB | Dynamic speed vectors $(dx, dy)$ |
| `headings` | `f32` × 1,000,000 | 4 MB | Orientation radians $\theta$ |
| `species_ids` | `u32` × 1,000,000 | 4 MB | Species lookup index |
| `energies` | `f32` × 1,000,000 | 4 MB | Current metabolic reserve |
| `ages` | `f32` × 1,000,000 | 4 MB | Age in seconds |
| `alive` | `u32` × 1,000,000 | 4 MB | Active lifecycle flag (0/1) |
| `cell_counts` | `atomic<u32>` × 2,073,600 | 8.3 MB | Spatial grid occupation count |
| `cell_starts` | `u32` × 2,073,600 | 8.3 MB | Prefix scan cell start indices |
| `cell_entries` | `u32` × 1,000,000 | 4 MB | Sorted entity IDs in grid |
| `instance_data`| `struct InstanceData` × 1M | 32 MB | Vertex render attributes |
| **Total VRAM** | | **~84.6 MB** | Easily fits in mobile/desktop GPUs |

---

## ⚡ UNIFORM BUFFER STRUCT (WGSL)

```wgsl
struct SimulationParams {
    dt: f32,
    width: f32,
    height: f32,
    carrying_capacity: f32,
    prey_reproduction: f32,
    predator_success: f32,
    speed_mult: f32,
    time: f32,
    entity_count: u32,
    num_cells: u32,
    cell_size: f32,
    _padding: u32,
};
```
