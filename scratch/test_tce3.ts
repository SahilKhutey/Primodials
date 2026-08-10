import { Simulation } from '../src/sim/simulation';
import { DEFAULT_SETTINGS } from '../src/sim/types';

function testTC_E3() {
  const sim = new Simulation(42, { ...DEFAULT_SETTINGS, initialPopulation: 0 });

  // Spawn a prey with intel < 0.2 (no brain, pure instinct)
  const prey = (sim as any).spawnOrganism(100, 100, {
    sides: 4, size: 8, hue: 120, speed: 1.0, senseRadius: 80, diet: 0,
    metabolism: 1.0, aggression: 0.0, cooperation: 0.5, fertility: 0.5,
    lifespan: 1000, socialGene: 0.5, intelligence: 0.05, // brainless
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.2, altruism: 0.1,
    dominance: 0.3, nicheBreadth: 0.5, competitiveness: 0.2,
  }, 1);

  // Spawn a large carnivore threat nearby at (130, 100) — to the right
  const predator = (sim as any).spawnOrganism(130, 100, {
    sides: 6, size: 16, hue: 0, speed: 1.2, senseRadius: 100, diet: 1.0, // carnivore
    metabolism: 1.0, aggression: 0.8, cooperation: 0.1, fertility: 0.5,
    lifespan: 1000, socialGene: 0.1, intelligence: 0.5,
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.2, altruism: 0.1,
    dominance: 0.8, nicheBreadth: 0.5, competitiveness: 0.8,
  }, 1);

  (sim as any).spatialGrid.clear(sim.settings.worldWidth, sim.settings.worldHeight);
  for (const o of sim.organisms) (sim as any).spatialGrid.insert(o);
  (sim as any).updateOrganism(prey);

  console.log(`Prey threat level: ${prey.threatLevel.toFixed(2)}`);
  console.log(`Prey angle after tick: ${prey.angle.toFixed(2)} rad (fleeing away from x=130 threat)`);

  // Threat is at +x direction (0 rad). Flee angle should point away from threat (~ Math.PI or -Math.PI)
  const isFleeing = prey.threatLevel > 0 && Math.abs(Math.abs(prey.angle) - Math.PI) < 0.6;

  if (isFleeing) {
    console.log('TC-E3 VERIFIED PASS: Brainless organism (intel=0.05) exhibits instinctual flee behavior when a predator is nearby!');
  } else {
    console.error('TC-E3 FAIL: Brainless organism failed to detect threat or flee');
    process.exit(1);
  }
}

testTC_E3();
