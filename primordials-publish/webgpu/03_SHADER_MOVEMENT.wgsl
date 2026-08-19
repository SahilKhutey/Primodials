/**
 * Primordials — Movement Compute Shader (WGSL)
 *
 * Applies dynamic velocity and toroidal world bounds wrapping.
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
@group(0) @binding(1) var<storage, read_write> positions: array<vec2<f32>>;
@group(0) @binding(2) var<storage, read_write> velocities: array<vec2<f32>>;
@group(0) @binding(3) var<storage, read_write> headings: array<f32>;
@group(0) @binding(4) var<storage, read_write> alive: array<u32>;

@compute @workgroup_size(256)
fn main(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= params.entity_count) { return; }
    if (alive[idx] == 0u) { return; }

    var pos = positions[idx];
    let vel = velocities[idx];

    // Integrate position
    pos = pos + vel * params.dt * params.speed_mult;

    // Toroidal world wrapping
    pos.x = (pos.x + params.width) % params.width;
    pos.y = (pos.y + params.height) % params.height;

    positions[idx] = pos;
}
