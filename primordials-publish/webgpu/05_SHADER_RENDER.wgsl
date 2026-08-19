/**
 * Primordials — Hardware Instanced Render Shader (WGSL)
 *
 * Renders millions of polygon entities in single instanced draw call batches.
 */

struct VertexInput {
    @builtin(vertex_index) vertex_idx: u32,
    @builtin(instance_index) instance_idx: u32,
};

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) color: vec4<f32>,
};

struct RenderParams {
    view: mat4x4<f32>,
    width: f32,
    height: f32,
    _padding: vec2<f32>,
};

struct InstanceData {
    position: vec2<f32>,
    color: vec4<f32>,
    size: f32,
    shape_type: u32,
    rotation: f32,
    _padding: f32,
};

@group(0) @binding(0) var<uniform> render_params: RenderParams;
@group(0) @binding(1) var<storage, read> instances: array<InstanceData>;

const VERTICES: array<vec2<f32>, 6> = array<vec2<f32>, 6>(
    vec2<f32>(-1.0, -1.0),
    vec2<f32>( 1.0, -1.0),
    vec2<f32>(-1.0,  1.0),
    vec2<f32>(-1.0,  1.0),
    vec2<f32>( 1.0, -1.0),
    vec2<f32>( 1.0,  1.0),
);

@vertex
fn vs_main(input: VertexInput) -> VertexOutput {
    let instance = instances[input.instance_idx];
    let corner = VERTICES[input.vertex_idx];

    let local_pos = corner * instance.size;
    let c = cos(instance.rotation);
    let s = sin(instance.rotation);
    let rotated = vec2<f32>(
        local_pos.x * c - local_pos.y * s,
        local_pos.x * s + local_pos.y * c
    );

    let world_pos = instance.position + rotated;

    let ndc_x = (world_pos.x / render_params.width) * 2.0 - 1.0;
    let ndc_y = 1.0 - (world_pos.y / render_params.height) * 2.0;

    return VertexOutput(
        vec4<f32>(ndc_x, ndc_y, 0.0, 1.0),
        instance.color,
    );
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4<f32> {
    return input.color;
}
