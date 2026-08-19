/**
 * Main simulation loop — applies Lotka-Volterra dynamics + behaviors
 */
import { ECS } from './ECS';
import type { Biome } from '@/types/biome';
import type { Entity, Position } from '@/types/entity';
import type { SimulationParameters, SimulationStats } from '@/types/simulation';
import { clamp, distance, normalizeAngle, shortestAngleDiff } from './math';

export class Simulation {
  private ecs: ECS;
  private biome: Biome;
  private parameters: SimulationParameters;
  private tickCount: number = 0;
  private elapsedTime: number = 0;

  // Stats
  private preyCount: number = 0;
  private predatorCount: number = 0;
  private fpsSamples: number[] = [];
  private lastFrameTime: number = 0;

  // Spatial hash for neighbor queries
  private spatialHash: Map<string, number[]> = new Map();
  private cellSize: number = 50;

  constructor(biome: Biome, parameters: SimulationParameters, seed: number) {
    this.biome = biome;
    this.parameters = parameters;
    this.ecs = new ECS(seed);
    this.lastFrameTime = performance.now();
    this.initialize();
  }

  private initialize(): void {
    for (const species of this.biome.species) {
      for (let i = 0; i < species.initial; i++) {
        const pos = this.ecs.randomPosition(
          this.biome.world.width,
          this.biome.world.height
        );
        this.ecs.createEntity(species, pos);
      }
    }
  }

  step(dt: number): void {
    if (this.parameters.paused) return;
    dt = Math.min(dt, 1 / 30);
    this.tickCount++;
    this.elapsedTime += dt;

    const entities = this.ecs.getAll();
    this.rebuildSpatialHash(entities);

    for (const entity of entities) {
      if (!entity.alive) continue;
      this.applyBehavior(entity, dt);
    }

    for (const entity of entities) {
      if (!entity.alive) continue;
      this.applyMovement(entity, dt);
    }

    for (const entity of entities) {
      if (!entity.alive) continue;
      this.applyMetabolism(entity, dt);
    }

    this.applyEcology(entities, dt);
    this.checkExtinction();
    this.updateStats();

    const now = performance.now();
    const frameTime = now - this.lastFrameTime;
    this.lastFrameTime = now;
    if (frameTime > 0) {
      this.fpsSamples.push(1000 / frameTime);
      if (this.fpsSamples.length > 30) this.fpsSamples.shift();
    }
  }

  private applyBehavior(entity: Entity, dt: number): void {
    const behavior = entity.species.behavior;
    const speed = behavior.movement.speed * (entity.species.stats.speedMultiplier ?? 1) * this.parameters.speedMultiplier;

    switch (behavior.movement.type) {
      case 'wander': {
        if (Math.random() < 0.1 * dt * 60) {
          entity.heading += (Math.random() - 0.5) * 0.5;
        }
        break;
      }
      case 'seek': {
        const prey = this.findNearest(entity, behavior.feeding.diet);
        if (prey) {
          const desiredHeading = Math.atan2(
            prey.position.y - entity.position.y,
            prey.position.x - entity.position.x
          );
          entity.heading = normalizeAngle(entity.heading);
          const diff = shortestAngleDiff(entity.heading, desiredHeading);
          entity.heading += clamp(diff * 5 * dt, -behavior.movement.turnRate * dt, behavior.movement.turnRate * dt);
        }
        break;
      }
      case 'flee': {
        const predator = this.findNearestOfType(entity, 'predator');
        if (predator) {
          const desiredHeading = Math.atan2(
            entity.position.y - predator.position.y,
            entity.position.x - predator.position.x
          );
          const diff = shortestAngleDiff(entity.heading, desiredHeading);
          entity.heading += clamp(diff * 5 * dt, -behavior.movement.turnRate * dt, behavior.movement.turnRate * dt);
        }
        break;
      }
      case 'patrol': {
        if (Math.random() < 0.05 * dt * 60) {
          entity.heading += (Math.random() - 0.5) * 1.0;
        }
        break;
      }
    }

    entity.velocity.dx = Math.cos(entity.heading) * speed;
    entity.velocity.dy = Math.sin(entity.heading) * speed;
  }

  private applyMovement(entity: Entity, dt: number): void {
    entity.position.x += entity.velocity.dx * dt;
    entity.position.y += entity.velocity.dy * dt;
    const w = this.biome.world.width;
    const h = this.biome.world.height;

    if (this.biome.world.wrap) {
      if (entity.position.x < 0) entity.position.x += w;
      if (entity.position.x >= w) entity.position.x -= w;
      if (entity.position.y < 0) entity.position.y += h;
      if (entity.position.y >= h) entity.position.y -= h;
    } else {
      if (entity.position.x < 0 || entity.position.x >= w) {
        entity.velocity.dx = -entity.velocity.dx;
        entity.position.x = clamp(entity.position.x, 0, w - 1);
      }
      if (entity.position.y < 0 || entity.position.y >= h) {
        entity.velocity.dy = -entity.velocity.dy;
        entity.position.y = clamp(entity.position.y, 0, h - 1);
      }
    }
  }

  private applyMetabolism(entity: Entity, dt: number): void {
    entity.age += dt;
    entity.hunger += entity.species.behavior.feeding.hungerRate * dt;
    entity.energy -= entity.species.stats.metabolism * dt;

    if (entity.age >= entity.lifespan || entity.energy <= 0) {
      entity.alive = false;
    }
    if (entity.hunger >= entity.species.behavior.feeding.maxHunger) {
      entity.alive = false;
    }
  }

  private applyEcology(entities: Entity[], dt: number): void {
    for (const entity of entities) {
      if (!entity.alive) continue;

      if (entity.species.type === 'predator' || entity.species.type === 'omnivore' || entity.species.type === 'grazer') {
        this.feed(entity, dt);
      }

      if (entity.reproductionCooldown > 0) {
        entity.reproductionCooldown -= dt;
      } else if (
        entity.energy >= entity.species.behavior.reproduction.threshold * entity.species.stats.maxEnergy &&
        entity.age >= entity.maturityAge
      ) {
        this.reproduce(entity);
      }
    }
  }

  private feed(entity: Entity, dt: number): void {
    const diet = entity.species.behavior.feeding.diet;
    if (diet.length === 0) return;

    const vision = entity.species.stats.vision;
    const nearby = this.queryRadius(entity.position, vision);
    let target: Entity | undefined;
    let minDist = Infinity;

    for (const other of nearby) {
      if (other.id === entity.id || !other.alive) continue;
      if (!diet.includes(other.speciesId)) continue;
      const d = distance(entity.position, other.position);
      if (d < minDist) {
        minDist = d;
        target = other;
      }
    }

    if (target) {
      const baseHunt = entity.species.behavior.feeding.huntSuccess ?? 0.5;
      const successChance = baseHunt * this.parameters.predatorHuntSuccess;
      if (Math.random() < successChance * dt * 2) {
        const energyGain = (target.species.stats.maxEnergy ?? 1) * entity.species.behavior.feeding.consumptionRate;
        entity.energy = Math.min(entity.species.stats.maxEnergy, entity.energy + energyGain);
        entity.hunger = Math.max(0, entity.hunger - energyGain * 2);
        target.alive = false;
      }
    }
  }

  private reproduce(entity: Entity): void {
    const repro = entity.species.behavior.reproduction;
    if (this.ecs.count() >= this.parameters.carryingCapacity) return;

    const effectiveOffspring = Math.max(1, Math.round(repro.offspring * this.parameters.preyReproductionRate));
    const rng = this.ecs.getRNG();

    for (let i = 0; i < effectiveOffspring; i++) {
      if (this.ecs.count() >= this.parameters.carryingCapacity) break;
      const offsetAngle = rng.angle();
      const offsetDist = entity.size * 2;
      const childPos: Position = {
        x: entity.position.x + Math.cos(offsetAngle) * offsetDist,
        y: entity.position.y + Math.sin(offsetAngle) * offsetDist,
      };

      const w = this.biome.world.width;
      const h = this.biome.world.height;
      if (this.biome.world.wrap) {
        childPos.x = ((childPos.x % w) + w) % w;
        childPos.y = ((childPos.y % h) + h) % h;
      }

      const child = this.ecs.createEntity(entity.species, childPos);
      child.generation = entity.generation + 1;
      child.energy = entity.species.stats.maxEnergy * 0.6;
    }

    entity.reproductionCooldown = repro.maturityAge * 0.5;
    entity.energy *= 0.7;
  }

  private findNearest(entity: Entity, dietIds: string[]): Entity | undefined {
    if (dietIds.length === 0) return undefined;
    const vision = entity.species.stats.vision;
    const nearby = this.queryRadius(entity.position, vision);
    let nearest: Entity | undefined;
    let minDist = Infinity;

    for (const other of nearby) {
      if (other.id === entity.id || !other.alive) continue;
      if (!dietIds.includes(other.speciesId)) continue;
      const d = distance(entity.position, other.position);
      if (d < minDist) {
        minDist = d;
        nearest = other;
      }
    }
    return nearest;
  }

  private findNearestOfType(entity: Entity, type: string): Entity | undefined {
    const vision = entity.species.stats.vision * 1.5;
    const nearby = this.queryRadius(entity.position, vision);
    let nearest: Entity | undefined;
    let minDist = Infinity;

    for (const other of nearby) {
      if (other.id === entity.id || !other.alive) continue;
      if (other.species.type !== type) continue;
      const d = distance(entity.position, other.position);
      if (d < minDist) {
        minDist = d;
        nearest = other;
      }
    }
    return nearest;
  }

  private rebuildSpatialHash(entities: Entity[]): void {
    this.spatialHash.clear();
    for (const entity of entities) {
      if (!entity.alive) continue;
      const cx = Math.floor(entity.position.x / this.cellSize);
      const cy = Math.floor(entity.position.y / this.cellSize);
      const key = `${cx},${cy}`;
      let bucket = this.spatialHash.get(key);
      if (!bucket) {
        bucket = [];
        this.spatialHash.set(key, bucket);
      }
      bucket.push(entity.id);
    }
  }

  private queryRadius(center: Position, radius: number): Entity[] {
    const results: Entity[] = [];
    const cellRadius = Math.ceil(radius / this.cellSize);
    const cx = Math.floor(center.x / this.cellSize);
    const cy = Math.floor(center.y / this.cellSize);

    for (let dx = -cellRadius; dx <= cellRadius; dx++) {
      for (let dy = -cellRadius; dy <= cellRadius; dy++) {
        const key = `${cx + dx},${cy + dy}`;
        const ids = this.spatialHash.get(key);
        if (!ids) continue;
        for (const id of ids) {
          const entity = this.ecs.getAll().find((e) => e.id === id);
          if (entity && entity.alive) results.push(entity);
        }
      }
    }
    return results;
  }

  private checkExtinction(): void {
    for (const species of this.biome.species) {
      const count = this.ecs.getBySpecies(species.id).length;
      if (count < this.biome.rules.extinctionThreshold) {
        const respawnCount = Math.floor(species.initial * 0.3);
        for (let i = 0; i < respawnCount; i++) {
          const pos = this.ecs.randomPosition(
            this.biome.world.width,
            this.biome.world.height
          );
          this.ecs.createEntity(species, pos);
        }
      }
    }
  }

  private updateStats(): void {
    this.preyCount = 0;
    this.predatorCount = 0;
    for (const entity of this.ecs.getAll()) {
      if (!entity.alive) continue;
      if (entity.species.type === 'predator') {
        this.predatorCount++;
      } else if (entity.species.type === 'grazer' || entity.species.type === 'producer') {
        this.preyCount++;
      }
    }
  }

  getStats(): SimulationStats {
    const avgFps = this.fpsSamples.length > 0 ? this.fpsSamples.reduce((a, b) => a + b, 0) / this.fpsSamples.length : 60;
    return {
      prey: this.preyCount,
      predators: this.predatorCount,
      total: this.ecs.count(),
      fps: Math.round(avgFps),
      ticks: this.tickCount,
      generations: Math.floor(this.elapsedTime / 10),
    };
  }

  getEntities(): Entity[] {
    return this.ecs.getAll().filter((e) => e.alive);
  }

  getBiome(): Biome {
    return this.biome;
  }

  setParameters(params: Partial<SimulationParameters>): void {
    this.parameters = { ...this.parameters, ...params };
  }

  pause(): void {
    this.parameters.paused = true;
  }

  resume(): void {
    this.parameters.paused = false;
  }

  reset(seed?: number): void {
    this.ecs.clear();
    this.tickCount = 0;
    this.elapsedTime = 0;
    if (seed !== undefined) {
      this.ecs.reseed(seed);
    }
    this.initialize();
  }
}
