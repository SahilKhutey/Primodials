// Headless simulation benchmark script
// Run with: npx tsx src/sim/simBenchmark.ts

import { Simulation } from './simulation';
import { DEFAULT_SETTINGS } from './types';

function runBenchmark(ticks = 500) {
  console.log(`🚀 Starting Primordials Simulation Benchmark (${ticks} ticks)...`);

  const sim = new Simulation(12345, {
    ...DEFAULT_SETTINGS,
    initialPopulation: 60,
    maxPopulation: 300,
    chemicalField: true,
    microbialBehavior: true,
    neuralBrains: true,
    worldExpansion: true,
  });

  const start = performance.now();
  let peakPop = 0;
  let maxGen = 0;

  for (let i = 0; i < ticks; i++) {
    sim.step();
    peakPop = Math.max(peakPop, sim.population);
    maxGen = Math.max(maxGen, sim.stats.maxGeneration);
  }

  const durationMs = performance.now() - start;
  const ticksPerSec = (ticks / (durationMs / 1000)).toFixed(1);
  const avgMsPerTick = (durationMs / ticks).toFixed(3);

  console.log(`
✅ Benchmark Complete!
----------------------------------
Total Ticks:          ${ticks}
Duration:             ${durationMs.toFixed(2)} ms
Average Tick Time:    ${avgMsPerTick} ms/tick
Throughput:           ${ticksPerSec} ticks/sec
Final Population:     ${sim.population} (Peak: ${peakPop})
Species Count:        ${sim.species.length}
Max Generation:       ${maxGen}
Total Births:         ${sim.stats.births}
Total Deaths:         ${sim.stats.deaths}
Total Kills:          ${sim.stats.kills}
Civilization Level:   ${sim.stats.avgIntelligence.toFixed(3)}
----------------------------------
  `);
}

runBenchmark();
