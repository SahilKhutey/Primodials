/**
 * Primordials — Lotka-Volterra Dynamics Compute Shader (WGSL)
 *
 * Applies predator-prey dynamics, metabolic drain, and reproduction.
 * Utilizes spatial hash grid for O(1) neighbor interactions.
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

struct SpeciesConfig {
    species_id: u32,
    species_type: u32,  // 0=producer, 1=grazer, 2=predator, 3=scavenger
    color: vec4<f32>,
    size: f32,
    max_energy: f32,
    metabolism: f32,
    lifespan: f32,
    vision: f32,
    speed: f32,
    _padding: vec3<f32>,
};

@group(0) @binding(0) var<uniform> params: SimulationParams;
@group(0) @binding(1) var<storage, read_write> positions: array<vec2<f32>>;
@group(0) @binding(2) var<storage, read_write> velocities: array<vec2<f32>>;
@group(0) @binding(3) var<storage, read_write> headings: array<f32>;
@group(0) @binding(4) var<storage, read_write> species_ids: array<u32>;
@group(0) @binding(5) var<storage, read_write> energies: array<f32>;
@group(0) @binding(6) var<storage, read_write> ages: array<f32>;
@group(0) @binding(7) var<storage, read_write> alive: array<u32>;
@group(0) @binding(8) var<storage, read_write> generations: array<u32>;
@group(0) @binding(9) var<storage, read> species_configs: array<SpeciesConfig>;

// Spatial hash grid
@group(0) @binding(10) var<storage, read> cell_starts: array<u32>;
@group(0) @binding(11) var<storage, read> cell_entries: array<u32>;

// Cross-workgroup birth counters
@group(0) @binding(12) var<storage, read_write> birth_counter: atomic<u32>;
@group(0) @binding(13) var<storage, read_write> birth_positions: array<vec2<f32>>;
@group(0) @binding(14) var<storage, read_write> birth_species: array<u32>;

const PRODUCER: u32 = 0u;
const GRAZER: u32 = 1u;
const PREDATOR: u32 = 2u;
const TAU: f32 = 6.28318530718;

fn hash(u: u32) -> u32 {
    var x = u;
    x = x ^ (x >> 16u);
    x = x * 0x45d9f3bu;
    x = x ^ (x >> 16u);
    x = x * 0x45d9f3bu;
    x = x ^ (x >> 16u);
    return x;
}

fn random(seed: u32, salt: u32) -> f32 {
    return f32(hash(seed ^ salt)) / 4294967295.0;
}

@compute @workgroup_size(256)
fn main(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= params.entity_count) { return; }
    if (alive[idx] == 0u) { return; }

    let my_species_id = species_ids[idx];
    let my_config = species_configs[my_species_id];
    let pos = positions[idx];

    var new_energy = energies[idx];
    var new_age = ages[idx] + params.dt;

    // ─── 1. FEEDING (Spatial Hash Lookup) ──────────────────
    if (my_config.species_type == PREDATOR || my_config.species_type == GRAZER) {
        let vision_sq = my_config.vision * my_config.vision;
        let cell_size = params.cell_size;
        let my_cell_x = i32(pos.x / cell_size);
        let my_cell_y = i32(pos.y / cell_size);

        var found_prey = false;

        // Query 3x3 surrounding cells
        for (var dy = -1; dy <= 1; dy = dy + 1) {
            for (var dx = -1; dx <= 1; dx = dx + 1) {
                let cell_x = my_cell_x + dx;
                let cell_y = my_cell_y + dy;

                if (cell_x < 0 || cell_y < 0) { continue; }
                let grid_w = u32(params.width / cell_size);
                let cell_idx = u32(cell_y) * grid_w + u32(cell_x);
                if (cell_idx >= params.num_cells) { continue; }

                let start = cell_starts[cell_idx];
                let end = cell_starts[cell_idx + 1u];

                for (var i = start; i < end; i = i + 1u) {
                    let other_idx = cell_entries[i];
                    if (other_idx == idx) { continue; }
                    if (alive[other_idx] == 0u) { continue; }

                    let other_species = species_ids[other_idx];
                    let is_prey = (my_config.species_type == PREDATOR) ?
                        (species_configs[other_species].species_type == GRAZER) :
                        (species_configs[other_species].species_type == PRODUCER);

                    if (!is_prey) { continue; }

                    let other_pos = positions[other_idx];
                    let diff = other_pos - pos;
                    let dist_sq = dot(diff, diff);

                    if (dist_sq < vision_sq && dist_sq > 0.001) {
                        if (my_config.species_type == PREDATOR) {
                            let success = params.predator_success * 0.45;
                            if (random(idx, u32(new_age * 1000.0)) < success) {
                                new_energy = min(my_config.max_energy, new_energy + 0.4);
                                alive[other_idx] = 0u;
                                found_prey = true;
                                break;
                            }
                        } else {
                            new_energy = min(my_config.max_energy, new_energy + 0.15);
                            alive[other_idx] = 0u;
                            found_prey = true;
                            break;
                        }
                    }
                }
                if (found_prey) { break; }
            }
            if (found_prey) { break; }
        }
    }

    // ─── 2. METABOLISM & LIFECYCLE ─────────────────────────
    new_energy = new_energy - my_config.metabolism * params.dt;

    if (new_energy <= 0.0 || new_age >= my_config.lifespan) {
        alive[idx] = 0u;
        return;
    }

    // ─── 3. REPRODUCTION ──────────────────────────────────
    let repro_threshold = 0.7 * my_config.max_energy;
    if (new_energy >= repro_threshold && new_age >= 25.0) {
        let birth_idx = atomicAdd(&birth_counter, 1u);
        if (birth_idx < params.entity_count) {
            let angle = random(idx, u32(new_age)) * TAU;
            let offset = vec2<f32>(cos(angle), sin(angle)) * 6.0;
            let child_pos = (pos + offset + vec2<f32>(params.width, params.height)) %
                            vec2<f32>(params.width, params.height);

            birth_positions[birth_idx] = child_pos;
            birth_species[birth_idx] = my_species_id;
        }
        new_energy = new_energy * 0.6;
    }

    energies[idx] = new_energy;
    ages[idx] = new_age;
    alive[idx] = 1u;
}
