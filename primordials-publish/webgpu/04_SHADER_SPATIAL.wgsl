/**
 * Primordials — Spatial Hash Compute Shader (WGSL)
 *
 * Parallel binning of entities into uniform spatial grid cells.
 */

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

@group(0) @binding(0) var<uniform> params: SimulationParams;
@group(0) @binding(1) var<storage, read> positions: array<vec2<f32>>;
@group(0) @binding(2) var<storage, read> alive: array<u32>;
@group(0) @binding(3) var<storage, read_write> cell_counts: array<atomic<u32>>;
@group(0) @binding(4) var<storage, read_write> cell_starts: array<u32>;
@group(0) @binding(5) var<storage, read_write> cell_entries: array<u32>;

@compute @workgroup_size(256)
fn count_cells(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= params.entity_count) { return; }
    if (alive[idx] == 0u) { return; }

    let pos = positions[idx];
    let cell_x = u32(pos.x / params.cell_size);
    let cell_y = u32(pos.y / params.cell_size);
    let grid_w = u32(params.width / params.cell_size);
    let cell_idx = cell_y * grid_w + cell_x;

    if (cell_idx < params.num_cells) {
        atomicAdd(&cell_counts[cell_idx], 1u);
    }
}

@compute @workgroup_size(256)
fn place_entities(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= params.entity_count) { return; }
    if (alive[idx] == 0u) { return; }

    let pos = positions[idx];
    let cell_x = u32(pos.x / params.cell_size);
    let cell_y = u32(pos.y / params.cell_size);
    let grid_w = u32(params.width / params.cell_size);
    let cell_idx = cell_y * grid_w + cell_x;

    if (cell_idx < params.num_cells) {
        let slot = atomicAdd(&cell_counts[cell_idx], 1u);
        cell_entries[cell_starts[cell_idx] + slot] = idx;
    }
}
