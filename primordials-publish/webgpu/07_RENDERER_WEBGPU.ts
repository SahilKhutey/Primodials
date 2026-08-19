/**
 * Primordials — Hybrid WebGPU / CPU Renderer
 *
 * Automatically provisions WebGPU if present, or falls back to Canvas 2D.
 */

import { GPUEngine } from './06_GPU_ENGINE';
import { Renderer as CanvasRenderer } from '../web-app/src/engine/Renderer';
import type { Biome } from '../web-app/src/types/biome';

export class HybridRenderer {
  private gpuEngine: GPUEngine | null = null;
  private cpuRenderer: CanvasRenderer | null = null;
  private mode: 'webgpu' | 'canvas' = 'canvas';

  async initialize(canvas: HTMLCanvasElement, biome: Biome): Promise<void> {
    if (typeof navigator !== 'undefined' && 'gpu' in navigator && navigator.gpu) {
      try {
        const engine = new GPUEngine(canvas, biome);
        await engine.initialize(canvas);
        this.gpuEngine = engine;
        this.mode = 'webgpu';
        return;
      } catch (err) {
        console.warn('WebGPU failed, falling back to Canvas 2D:', err);
      }
    }

    this.cpuRenderer = new CanvasRenderer(canvas);
    this.mode = 'canvas';
  }

  step(dt: number, time: number): void {
    if (this.mode === 'webgpu' && this.gpuEngine) {
      this.gpuEngine.step(dt, time);
    }
  }

  getMode(): 'webgpu' | 'canvas' {
    return this.mode;
  }

  dispose(): void {
    this.gpuEngine?.dispose();
    this.cpuRenderer = null;
  }
}
