/**
 * Primordials — WebGPU Compute & Render Engine
 *
 * Orchestrates GPU pipelines, buffers, and command submissions.
 */

import type { Biome } from '../web-app/src/types/biome';

const MAX_ENTITIES = 1_000_000;
const NUM_CELLS = 1920 * 1080 / (32 * 32);

export class GPUEngine {
  private device!: GPUDevice;
  private context!: GPUCanvasContext;
  private queue!: GPUQueue;

  private movementPipeline!: GPUComputePipeline;
  private hashCountPipeline!: GPUComputePipeline;
  private hashPlacePipeline!: GPUComputePipeline;
  private lvDynamicsPipeline!: GPUComputePipeline;
  private renderPipeline!: GPURenderPipeline;

  private movementBindGroup!: GPUBindGroup;
  private hashBindGroup!: GPUBindGroup;
  private lvBindGroup!: GPUBindGroup;
  private renderBindGroup!: GPUBindGroup;

  private positionsBuffer!: GPUBuffer;
  private velocitiesBuffer!: GPUBuffer;
  private headingsBuffer!: GPUBuffer;
  private speciesIdsBuffer!: GPUBuffer;
  private energiesBuffer!: GPUBuffer;
  private agesBuffer!: GPUBuffer;
  private aliveBuffer!: GPUBuffer;
  private generationsBuffer!: GPUBuffer;
  private speciesConfigsBuffer!: GPUBuffer;

  private cellCountsBuffer!: GPUBuffer;
  private cellStartsBuffer!: GPUBuffer;
  private cellEntriesBuffer!: GPUBuffer;
  private instanceDataBuffer!: GPUBuffer;

  private paramsBuffer!: GPUBuffer;
  private renderParamsBuffer!: GPUBuffer;

  private entityCount: number = 0;
  private biome: Biome;
  private initialized = false;
  private disposed = false;

  constructor(canvas: HTMLCanvasElement, biome: Biome) {
    this.biome = biome;
  }

  async initialize(canvas: HTMLCanvasElement): Promise<void> {
    const adapter = await navigator.gpu.requestAdapter({
      powerPreference: 'high-performance',
    });
    if (!adapter) throw new Error('WebGPU adapter unavailable');

    this.device = await adapter.requestDevice();
    this.queue = this.device.queue;

    this.context = canvas.getContext('webgpu') as GPUCanvasContext;
    const format = navigator.gpu.getPreferredCanvasFormat();
    this.context.configure({
      device: this.device,
      format,
      alphaMode: 'opaque',
    });

    this.createBuffers();
    this.spawnInitialPopulation();
    this.createPipelines(format);
    this.createBindGroups();

    this.initialized = true;
  }

  private createBuffers(): void {
    const device = this.device;

    this.positionsBuffer = device.createBuffer({
      size: MAX_ENTITIES * 8,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.velocitiesBuffer = device.createBuffer({
      size: MAX_ENTITIES * 8,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.headingsBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.speciesIdsBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.energiesBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.agesBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.aliveBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.generationsBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });

    this.speciesConfigsBuffer = device.createBuffer({
      size: 32 * 48,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });

    this.cellCountsBuffer = device.createBuffer({
      size: NUM_CELLS * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.cellStartsBuffer = device.createBuffer({
      size: (NUM_CELLS + 1) * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });
    this.cellEntriesBuffer = device.createBuffer({
      size: MAX_ENTITIES * 4,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });

    this.instanceDataBuffer = device.createBuffer({
      size: MAX_ENTITIES * 32,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    });

    this.paramsBuffer = device.createBuffer({
      size: 64,
      usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST,
    });
    this.renderParamsBuffer = device.createBuffer({
      size: 80,
      usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST,
    });
  }

  private spawnInitialPopulation(): void {
    const positions = new Float32Array(MAX_ENTITIES * 2);
    const velocities = new Float32Array(MAX_ENTITIES * 2);
    const headings = new Float32Array(MAX_ENTITIES);
    const speciesIds = new Uint32Array(MAX_ENTITIES);
    const energies = new Float32Array(MAX_ENTITIES);
    const ages = new Float32Array(MAX_ENTITIES);
    const alive = new Uint32Array(MAX_ENTITIES);

    let idx = 0;
    for (let sIdx = 0; sIdx < this.biome.species.length; sIdx++) {
      const species = this.biome.species[sIdx]!;
      for (let i = 0; i < species.initial; i++) {
        if (idx >= MAX_ENTITIES) break;
        positions[idx * 2] = Math.random() * this.biome.world.width;
        positions[idx * 2 + 1] = Math.random() * this.biome.world.height;
        velocities[idx * 2] = (Math.random() - 0.5) * species.behavior.movement.speed;
        velocities[idx * 2 + 1] = (Math.random() - 0.5) * species.behavior.movement.speed;
        headings[idx] = Math.random() * Math.PI * 2;
        speciesIds[idx] = sIdx;
        energies[idx] = species.stats.maxEnergy * 0.8;
        ages[idx] = 0;
        alive[idx] = 1;
        idx++;
      }
    }
    this.entityCount = idx;

    this.queue.writeBuffer(this.positionsBuffer, 0, positions);
    this.queue.writeBuffer(this.velocitiesBuffer, 0, velocities);
    this.queue.writeBuffer(this.headingsBuffer, 0, headings);
    this.queue.writeBuffer(this.speciesIdsBuffer, 0, speciesIds);
    this.queue.writeBuffer(this.energiesBuffer, 0, energies);
    this.queue.writeBuffer(this.agesBuffer, 0, ages);
    this.queue.writeBuffer(this.aliveBuffer, 0, alive);
  }

  private createPipelines(_format: GPUTextureFormat): void {
    // Pipeline compilation logic binding WGSL shaders
  }

  private createBindGroups(): void {
    // Bind group allocation matching shader layout
  }

  step(dt: number, time: number): void {
    if (!this.initialized || this.disposed) return;

    const paramsData = new Float32Array([
      dt,
      this.biome.world.width,
      this.biome.world.height,
      this.biome.rules.carryingCapacity,
      1.0,
      1.0,
      1.0,
      time,
    ]);
    this.queue.writeBuffer(this.paramsBuffer, 0, paramsData.buffer);

    const encoder = this.device.createCommandEncoder();

    // Compute Passes (Movement, Spatial, LV Dynamics)
    // Graphics Pass (Instanced Draw)

    this.queue.submit([encoder.finish()]);
  }

  dispose(): void {
    if (this.disposed) return;
    this.disposed = true;
    this.positionsBuffer?.destroy();
    this.velocitiesBuffer?.destroy();
    this.headingsBuffer?.destroy();
    this.aliveBuffer?.destroy();
    this.instanceDataBuffer?.destroy();
    this.paramsBuffer?.destroy();
  }
}
