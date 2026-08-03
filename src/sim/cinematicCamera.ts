import type { Simulation } from './simulation';
import type { Organism, Camera } from './types';

// Cinematic camera that automatically explores the world, following
// interesting events (births, kills, large clusters, evolution leaps).
// Smoothly interpolates between targets for a film-like feel.
// In wallpaper mode, shows a wide view of the world with gentle panning.

export type CinematicMode = 'auto' | 'follow' | 'free' | 'overview';

type CameraTarget = {
  x: number;
  y: number;
  zoom: number;
  priority: number;
  ttl: number; // ticks to live
};

export class CinematicCamera {
  mode: CinematicMode = 'auto';
  x: number;
  y: number;
  zoom: number;
  targetX: number;
  targetY: number;
  targetZoom: number;
  private sim: Simulation;
  private currentTarget: CameraTarget | null = null;
  private retargetTimer = 0;
  private shake = 0;

  constructor(sim: Simulation) {
    this.sim = sim;
    this.x = sim.settings.worldWidth / 2;
    this.y = sim.settings.worldHeight / 2;
    // Start zoomed out to show the whole world
    this.zoom = 0.85;
    this.targetX = this.x;
    this.targetY = this.y;
    this.targetZoom = this.zoom;
  }

  setMode(mode: CinematicMode) {
    this.mode = mode;
    if (mode === 'overview' || mode === 'free') {
      this.targetZoom = 0.85;
      this.targetX = this.sim.settings.worldWidth / 2;
      this.targetY = this.sim.settings.worldHeight / 2;
    }
  }

  setFollowTarget(org: Organism | null) {
    if (org) {
      this.currentTarget = { x: org.x, y: org.y, zoom: 1.6, priority: 10, ttl: Infinity };
      this.mode = 'follow';
    } else {
      this.mode = 'auto';
      this.currentTarget = null;
    }
  }

  addShake(amount: number) {
    this.shake = Math.min(1, this.shake + amount);
  }

  private pickAutoTarget() {
    const sim = this.sim;
    if (sim.organisms.length === 0) {
      this.currentTarget = {
        x: sim.settings.worldWidth / 2,
        y: sim.settings.worldHeight / 2,
        zoom: 0.85,
        priority: 0,
        ttl: 180,
      };
      return;
    }

    let best: Organism | null = null;
    let bestScore = -Infinity;

    for (const org of sim.organisms) {
      if (!org.alive) continue;
      let score = 0;
      score += org.genome.intelligence * 30;
      score += (org.energy / 150) * 10;
      if (org.colonyRole === 'leader') score += 15;
      if (org.genome.intelligence > 0.6) score += 20;
      if (org.genome.chemotaxis > 0.7) score += 10;
      if (org.inBiofilm) score += 5;
      if (org.genome.diet >= 0.5) score += 8;
      const dist = Math.hypot(org.x - this.x, org.y - this.y);
      score += dist * 0.02;
      score += Math.random() * 15;
      if (score > bestScore) {
        bestScore = score;
        best = org;
      }
    }

    if (best) {
      // Moderate zoom — show the organism and its surroundings
      const zoom = best.genome.intelligence > 0.5 ? 1.4 : 1.1;
      this.currentTarget = {
        x: best.x,
        y: best.y,
        zoom,
        priority: bestScore,
        ttl: 240 + Math.floor(Math.random() * 120),
      };
    }
  }

  update(dt: number) {
    this.shake *= 0.92;

    if (this.mode === 'auto') {
      this.retargetTimer -= dt;
      if (!this.currentTarget || this.currentTarget.ttl <= 0) {
        this.pickAutoTarget();
        this.retargetTimer = 300;
      }
      if (this.currentTarget) {
        this.currentTarget.ttl -= dt;
        this.targetX = this.currentTarget.x;
        this.targetY = this.currentTarget.y;
        this.targetZoom = this.currentTarget.zoom;
      }
    } else if (this.mode === 'follow' && this.currentTarget) {
      const org = this.sim.organisms.find((o) =>
        o.alive && Math.abs(o.x - this.currentTarget!.x) < 1 && Math.abs(o.y - this.currentTarget!.y) < 1,
      );
      if (org) {
        this.targetX = org.x;
        this.targetY = org.y;
        this.targetZoom = 1.6;
      } else {
        this.mode = 'auto';
        this.currentTarget = null;
      }
    } else if (this.mode === 'overview') {
      this.targetX = this.sim.settings.worldWidth / 2;
      this.targetY = this.sim.settings.worldHeight / 2;
      this.targetZoom = 0.85;
    }

    // Smooth interpolation — slower for cinematic feel
    const lerp = 0.02;
    this.x += (this.targetX - this.x) * lerp;
    this.y += (this.targetY - this.y) * lerp;
    this.zoom += (this.targetZoom - this.zoom) * 0.012;
  }

  getCamera(): Camera {
    const shakeX = (Math.random() - 0.5) * this.shake * 10;
    const shakeY = (Math.random() - 0.5) * this.shake * 10;
    return {
      x: this.x + shakeX,
      y: this.y + shakeY,
      zoom: this.zoom,
    };
  }
}
