// Spatial hash grid: buckets organism indices by position into fixed-size
// cells, so "find everything near (x,y) within radius r" only has to look
// at a small neighborhood of cells instead of scanning every organism in
// the simulation. This is what turns the altruism/competition/hunting/
// threat-detection loops from O(n²) into roughly O(n) at realistic
// population densities (each organism only has to check the handful of
// others actually near it, not the entire population).
//
// This returns CANDIDATES only — cells are square, queries are circular,
// so a candidate from a corner of the cell neighborhood might actually be
// just outside the real radius. Every call site still does its own exact
// `dx*dx + dy*dy < radius*radius` check on the returned candidates, exactly
// as it already did before this existed; this class only narrows down
// which organisms are worth checking in the first place.
//
// Known, deliberate trade-off: the grid is rebuilt once per tick from each
// organism's position at the START of that tick. An organism that moves
// during the tick (after being processed) is still queried at its
// old bucket location by organisms processed later in the same tick —
// its up-to-date x/y is still what gets distance-checked (this class only
// stores indices, never cached positions), so the only possible effect is
// an organism that moved far enough this tick to change cells being
// missed as a candidate by someone who processes after it moves. Given
// per-tick movement distances are small relative to the interaction radii
// this is used for (20-200+px) and the cell size below, this is the same
// class of negligible staleness every spatial-hashed simulation accepts —
// not a correctness bug, just worth documenting honestly rather than
// claiming byte-for-byte equivalence with the old O(n²) scan.
export class SpatialGrid {
  private readonly cellSize: number;
  private readonly cells: Map<number, number[]> = new Map();

  constructor(cellSize: number) {
    this.cellSize = cellSize;
  }

  private cellKeyFor(cx: number, cy: number): number {
    // Pack two 20-bit-ish signed cell coords into one number key — avoids
    // allocating a string per lookup, which matters since this runs for
    // every organism every tick. Offset keeps coords non-negative.
    return (cx + 100000) * 1000000 + (cy + 100000);
  }

  clear(): void {
    this.cells.clear();
  }

  insert(index: number, x: number, y: number): void {
    const cx = Math.floor(x / this.cellSize);
    const cy = Math.floor(y / this.cellSize);
    const key = this.cellKeyFor(cx, cy);
    let bucket = this.cells.get(key);
    if (!bucket) {
      bucket = [];
      this.cells.set(key, bucket);
    }
    bucket.push(index);
  }

  // Returns candidate indices whose cell overlaps a (x,y)-centered
  // bounding box of the given radius. Caller does the exact circular
  // distance check — see class-level comment.
  queryRadius(x: number, y: number, radius: number): number[] {
    const result: number[] = [];
    const cx = Math.floor(x / this.cellSize);
    const cy = Math.floor(y / this.cellSize);
    const cellRadius = Math.max(1, Math.ceil(radius / this.cellSize));
    for (let dcx = -cellRadius; dcx <= cellRadius; dcx++) {
      for (let dcy = -cellRadius; dcy <= cellRadius; dcy++) {
        const bucket = this.cells.get(this.cellKeyFor(cx + dcx, cy + dcy));
        if (bucket) result.push(...bucket);
      }
    }
    return result;
  }
}
