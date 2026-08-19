/**
 * Entity Component System — TypeScript port of the C++ engine
 * Data-oriented design for performance.
 */
import type { Entity, Position, Velocity } from '@/types/entity';
import type { SpeciesConfig } from '@/types/biome';
import { Random } from './random';

let nextEntityId = 0;

export class ECS {
  private entities: Map<number, Entity> = new Map();
  private speciesIndex: Map<string, Entity[]> = new Map();
  private rng: Random;

  constructor(seed: number = Date.now()) {
    this.rng = new Random(seed);
  }

  createEntity(
    species: SpeciesConfig,
    position: Position,
    velocity: Velocity = { dx: 0, dy: 0 }
  ): Entity {
    const entity: Entity = {
      id: nextEntityId++,
      speciesId: species.id,
      species,
      position: { ...position },
      velocity: { ...velocity },
      heading: this.rng.angle(),
      age: 0,
      lifespan: species.stats.lifespan * this.rng.float(0.9, 1.1),
      energy: species.stats.maxEnergy * 0.8,
      hunger: 0,
      reproductionCooldown: species.behavior.reproduction.maturityAge,
      maturityAge: species.behavior.reproduction.maturityAge,
      alive: true,
      generation: 0,
      color: species.color,
      size: species.size,
      shape: species.shape,
    };

    this.entities.set(entity.id, entity);
    let bucket = this.speciesIndex.get(species.id);
    if (!bucket) {
      bucket = [];
      this.speciesIndex.set(species.id, bucket);
    }
    bucket.push(entity);
    return entity;
  }

  removeEntity(id: number): void {
    const entity = this.entities.get(id);
    if (!entity) return;
    const bucket = this.speciesIndex.get(entity.speciesId);
    if (bucket) {
      const idx = bucket.indexOf(entity);
      if (idx !== -1) bucket.splice(idx, 1);
    }
    this.entities.delete(id);
  }

  getAll(): Entity[] {
    return Array.from(this.entities.values());
  }

  getBySpecies(speciesId: string): Entity[] {
    return this.speciesIndex.get(speciesId) ?? [];
  }

  count(): number {
    return this.entities.size;
  }

  countByType(type: SpeciesConfig['type']): number {
    let count = 0;
    for (const entity of this.entities.values()) {
      if (entity.species.type === type) count++;
    }
    return count;
  }

  clear(): void {
    this.entities.clear();
    this.speciesIndex.clear();
  }

  randomPosition(worldWidth: number, worldHeight: number): Position {
    return {
      x: this.rng.float(0, worldWidth),
      y: this.rng.float(0, worldHeight),
    };
  }

  randomHeading(): number {
    return this.rng.angle();
  }

  getRNG(): Random {
    return this.rng;
  }

  reseed(seed: number): void {
    this.rng = new Random(seed);
  }
}
