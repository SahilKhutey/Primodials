import type { Organism } from './types';

// High-performance spatial hash grid for O(n) proximity queries
export class SpatialHashGrid {
  private cellSize: number;
  private cols: number = 0;
  private rows: number = 0;
  private buckets: Map<number, Organism[]> = new Map();

  constructor(cellSize = 100) {
    this.cellSize = cellSize;
  }

  clear(worldWidth: number, worldHeight: number): void {
    this.cols = Math.max(1, Math.ceil(worldWidth / this.cellSize));
    this.rows = Math.max(1, Math.ceil(worldHeight / this.cellSize));
    this.buckets.clear();
  }

  insert(org: Organism): void {
    const cx = Math.max(0, Math.min(this.cols - 1, Math.floor(org.x / this.cellSize)));
    const cy = Math.max(0, Math.min(this.rows - 1, Math.floor(org.y / this.cellSize)));
    const idx = cy * this.cols + cx;
    let list = this.buckets.get(idx);
    if (!list) {
      list = [];
      this.buckets.set(idx, list);
    }
    list.push(org);
  }

  getNearby(x: number, y: number, radius: number, result: Organism[]): void {
    result.length = 0;
    if (this.cols === 0 || this.rows === 0) return;
    const minCx = Math.max(0, Math.floor((x - radius) / this.cellSize));
    const maxCx = Math.min(this.cols - 1, Math.floor((x + radius) / this.cellSize));
    const minCy = Math.max(0, Math.floor((y - radius) / this.cellSize));
    const maxCy = Math.min(this.rows - 1, Math.floor((y + radius) / this.cellSize));

    const r2 = radius * radius;
    for (let cy = minCy; cy <= maxCy; cy++) {
      for (let cx = minCx; cx <= maxCx; cx++) {
        const idx = cy * this.cols + cx;
        const list = this.buckets.get(idx);
        if (!list) continue;
        for (let i = 0; i < list.length; i++) {
          const org = list[i];
          const dx = org.x - x;
          const dy = org.y - y;
          if (dx * dx + dy * dy <= r2) {
            result.push(org);
          }
        }
      }
    }
  }
}
