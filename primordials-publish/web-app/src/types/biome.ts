/**
 * Biome configuration types
 */

export type SpeciesType = 'producer' | 'grazer' | 'predator' | 'scavenger' | 'omnivore';
export type ShapeType = 'circle' | 'triangle' | 'hexagon' | 'square' | 'star';

export interface BehaviorConfig {
  movement: {
    type: 'wander' | 'seek' | 'flee' | 'patrol';
    speed: number;
    turnRate: number;
  };
  feeding: {
    diet: string[];
    consumptionRate: number;
    hungerRate: number;
    maxHunger: number;
    huntSuccess?: number;
    preferredPreySize?: number;
  };
  reproduction: {
    rate: number;
    threshold: number;
    offspring: number;
    maturityAge: number;
  };
  social?: {
    herdSize?: number;
    herdCohesion?: number;
    huntingPackSize?: number;
  };
}

export interface StatsConfig {
  maxEnergy: number;
  metabolism: number;
  lifespan: number;
  vision: number;
  armor?: number;
  speedMultiplier?: number;
}

export interface SpeciesConfig {
  id: string;
  name: string;
  type: SpeciesType;
  color: string;
  shape: ShapeType;
  size: number;
  initial: number;
  behavior: BehaviorConfig;
  stats: StatsConfig;
}

export interface WorldConfig {
  width: number;
  height: number;
  wrap: boolean;
  temperature?: number;
  timeOfDay?: boolean;
}

export interface RulesConfig {
  carryingCapacity: number;
  extinctionThreshold: number;
  respawnDelay: number;
  seasonalEvents?: Array<{
    trigger: 'time';
    interval: number;
    effect: string;
    magnitude?: number;
  }>;
}

export interface VisualsConfig {
  background: string;
  gradient?: string[];
  particles?: {
    enabled: boolean;
    count: number;
    type: string;
  };
}

export interface Biome {
  id: string;
  name: string;
  version: string;
  description: string;
  author: string;
  tags: string[];
  world: WorldConfig;
  species: SpeciesConfig[];
  rules: RulesConfig;
  visuals: VisualsConfig;
}
