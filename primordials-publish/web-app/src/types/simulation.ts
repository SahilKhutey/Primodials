/**
 * Simulation state types
 */

export interface SimulationStats {
  prey: number;
  predators: number;
  total: number;
  fps: number;
  ticks: number;
  generations: number;
}

export interface SimulationParameters {
  preyReproductionRate: number;
  predatorHuntSuccess: number;
  carryingCapacity: number;
  speedMultiplier: number;
  paused: boolean;
}

export interface SimulationState {
  biomeId: string;
  seed: number;
  isRunning: boolean;
  isPaused: boolean;
  stats: SimulationStats;
  parameters: SimulationParameters;
}
