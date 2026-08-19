/**
 * Entity types for the simulation
 */
import type { SpeciesConfig, ShapeType } from './biome';

export interface Position {
  x: number;
  y: number;
}

export interface Velocity {
  dx: number;
  dy: number;
}

export interface Entity {
  id: number;
  speciesId: string;
  species: SpeciesConfig;

  // Transform
  position: Position;
  velocity: Velocity;
  heading: number; // radians

  // Lifecycle
  age: number; // seconds
  lifespan: number; // seconds
  energy: number; // 0..maxEnergy
  hunger: number; // 0..maxHunger

  // Reproduction
  reproductionCooldown: number;
  maturityAge: number;

  // State
  alive: boolean;
  generation: number;

  // Visuals (cached for rendering)
  color: string;
  size: number;
  shape: ShapeType;
}
