// Grid-based chemical field for chemotaxis simulation.
// Two layers: attractant (from food, colonies) and repellent (from dead organisms, predators).
// Lightweight diffusion + decay each tick. Microbes sample the gradient at their position.

export class ChemicalField {
  width: number;
  height: number;
  cellSize: number;
  cols: number;
  rows: number;
  attractant: Float32Array;
  repellent: Float32Array;
  // Scratch buffers for double-buffered diffusion
  private nextAttractant: Float32Array;
  private nextRepellent: Float32Array;

  constructor(width: number, height: number, cellSize = 20) {
    this.width = width;
    this.height = height;
    this.cellSize = cellSize;
    this.cols = Math.ceil(width / cellSize);
    this.rows = Math.ceil(height / cellSize);
    const n = this.cols * this.rows;
    this.attractant = new Float32Array(n);
    this.repellent = new Float32Array(n);
    this.nextAttractant = new Float32Array(n);
    this.nextRepellent = new Float32Array(n);
  }

  resize(width: number, height: number): void {
    if (width === this.width && height === this.height) return;
    const oldCols = this.cols;
    const oldRows = this.rows;
    const oldAtt = this.attractant;
    const oldRep = this.repellent;

    this.width = width;
    this.height = height;
    this.cols = Math.ceil(width / this.cellSize);
    this.rows = Math.ceil(height / this.cellSize);
    const n = this.cols * this.rows;
    this.attractant = new Float32Array(n);
    this.repellent = new Float32Array(n);
    this.nextAttractant = new Float32Array(n);
    this.nextRepellent = new Float32Array(n);

    // Copy existing grid cell values so chemical memory & trails persist across expansions
    for (let cy = 0; cy < oldRows; cy++) {
      for (let cx = 0; cx < oldCols; cx++) {
        const oldIdx = cy * oldCols + cx;
        const newIdx = cy * this.cols + cx;
        this.attractant[newIdx] = oldAtt[oldIdx];
        this.repellent[newIdx] = oldRep[oldIdx];
      }
    }
  }

  private idx(cx: number, cy: number): number {
    return cy * this.cols + cx;
  }

  deposit(x: number, y: number, attractantAmount: number, repellentAmount: number): void {
    if (attractantAmount > 0) this.depositAttractant(x, y, attractantAmount);
    if (repellentAmount > 0) this.depositRepellent(x, y, repellentAmount);
  }

  depositAttractant(x: number, y: number, amount: number): void {
    const cx = Math.floor(x / this.cellSize);
    const cy = Math.floor(y / this.cellSize);
    if (cx < 0 || cx >= this.cols || cy < 0 || cy >= this.rows) return;
    this.attractant[this.idx(cx, cy)] += amount;
  }

  depositRepellent(x: number, y: number, amount: number): void {
    const cx = Math.floor(x / this.cellSize);
    const cy = Math.floor(y / this.cellSize);
    if (cx < 0 || cx >= this.cols || cy < 0 || cy >= this.rows) return;
    this.repellent[this.idx(cx, cy)] += amount;
  }

  // Sample the attractant gradient at a world position.
  // Returns {value, gradX, gradY} — gradient points toward higher concentration.
  sampleAttractant(x: number, y: number): { value: number; gradX: number; gradY: number } {
    const cx = Math.floor(x / this.cellSize);
    const cy = Math.floor(y / this.cellSize);
    if (cx < 1 || cx >= this.cols - 1 || cy < 1 || cy >= this.rows - 1) {
      return { value: 0, gradX: 0, gradY: 0 };
    }
    const i = this.idx(cx, cy);
    const value = this.attractant[i];
    const gradX = this.attractant[this.idx(cx + 1, cy)] - this.attractant[this.idx(cx - 1, cy)];
    const gradY = this.attractant[this.idx(cx, cy + 1)] - this.attractant[this.idx(cx, cy - 1)];
    return { value, gradX, gradY };
  }

  sampleRepellent(x: number, y: number): { value: number; gradX: number; gradY: number } {
    const cx = Math.floor(x / this.cellSize);
    const cy = Math.floor(y / this.cellSize);
    if (cx < 1 || cx >= this.cols - 1 || cy < 1 || cy >= this.rows - 1) {
      return { value: 0, gradX: 0, gradY: 0 };
    }
    const i = this.idx(cx, cy);
    const value = this.repellent[i];
    const gradX = this.repellent[this.idx(cx + 1, cy)] - this.repellent[this.idx(cx - 1, cy)];
    const gradY = this.repellent[this.idx(cx, cy + 1)] - this.repellent[this.idx(cx, cy - 1)];
    return { value, gradX, gradY };
  }

  // Diffuse + decay. Call once per tick.
  diffuse(): void {
    const { cols, rows, attractant, repellent, nextAttractant, nextRepellent } = this;
    const decay = 0.97;
    const diffusion = 0.15;

    for (let cy = 0; cy < rows; cy++) {
      for (let cx = 0; cx < cols; cx++) {
        const i = cy * cols + cx;
        let sumA = attractant[i];
        let sumR = repellent[i];
        let count = 1;

        if (cx > 0) { sumA += attractant[i - 1]; sumR += repellent[i - 1]; count++; }
        if (cx < cols - 1) { sumA += attractant[i + 1]; sumR += repellent[i + 1]; count++; }
        if (cy > 0) { sumA += attractant[i - cols]; sumR += repellent[i - cols]; count++; }
        if (cy < rows - 1) { sumA += attractant[i + cols]; sumR += repellent[i + cols]; count++; }

        const avgA = sumA / count;
        const avgR = sumR / count;
        nextAttractant[i] = (attractant[i] + (avgA - attractant[i]) * diffusion) * decay;
        nextRepellent[i] = (repellent[i] + (avgR - repellent[i]) * diffusion) * decay;
      }
    }

    // Swap buffers
    const tmpA = this.attractant;
    this.attractant = this.nextAttractant;
    this.nextAttractant = tmpA;

    const tmpR = this.repellent;
    this.repellent = this.nextRepellent;
    this.nextRepellent = tmpR;
  }

  clear(): void {
    this.attractant.fill(0);
    this.repellent.fill(0);
  }
}
