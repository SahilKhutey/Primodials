import { Simulation } from "@/sim/simulation";
import type { SimSettings } from "@/sim/types";

export type SoakResult = {
  seed: number;
  seconds: number;
  targetTicks: number;
  actualTicks: number;
  startingPopulation: number;
  endingPopulation: number;
  maxPopulation: number;
  minPopulation: number;
  species: number;
  finiteState: boolean;
  completed: boolean;
};

function isFiniteSimulation(sim: Simulation): boolean {
  const values = [
    sim.tick,
    sim.population,
    ...sim.organisms.slice(0, 500).flatMap((o) => [
      o.x, o.y, o.vx, o.vy, o.energy, o.age, o.fitness,
    ]),
  ];

  return values.every(Number.isFinite);
}

export function runSoakTest(
  seed: number,
  settings: SimSettings,
  seconds = 60,
): SoakResult {
  const sim = new Simulation(seed >>> 0, settings);
  const targetTicks = Math.max(1, Math.floor(seconds * 30));
  const startingPopulation = sim.population;
  let minPopulation = startingPopulation;
  let maxPopulation = startingPopulation;
  let finiteState = true;

  for (let i = 0; i < targetTicks; i += 1) {
    sim.step();
    const pop = sim.population;
    minPopulation = Math.min(minPopulation, pop);
    maxPopulation = Math.max(maxPopulation, pop);

    if (!isFiniteSimulation(sim)) {
      finiteState = false;
      break;
    }

    if (pop > settings.maxPopulation * 2) {
      finiteState = false;
      break;
    }
  }

  return {
    seed,
    seconds,
    targetTicks,
    actualTicks: sim.tick,
    startingPopulation,
    endingPopulation: sim.population,
    maxPopulation,
    minPopulation,
    species: sim.species.length,
    finiteState,
    completed: finiteState && sim.tick >= targetTicks,
  };
}
