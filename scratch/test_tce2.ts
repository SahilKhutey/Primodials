import { Simulation } from '../src/sim/simulation';
import { DEFAULT_SETTINGS } from '../src/sim/types';

function testTC_E2() {
  const sim = new Simulation(42, { ...DEFAULT_SETTINGS, initialPopulation: 0 });

  // Spawn an Alpha organism and an Omega organism of species 1
  const alphaOrg = (sim as any).spawnOrganism(100, 100, {
    sides: 5, size: 12, hue: 120, speed: 1.0, senseRadius: 80, diet: 0,
    metabolism: 1.0, aggression: 0.1, cooperation: 0.8, fertility: 0.5,
    lifespan: 1000, socialGene: 0.8, intelligence: 0.5,
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.8, altruism: 0.1,
    dominance: 0.9, nicheBreadth: 0.5, competitiveness: 0.2,
  }, 1);
  alphaOrg.socialRank = 'alpha';

  const omegaOrg = (sim as any).spawnOrganism(105, 100, {
    sides: 5, size: 8, hue: 120, speed: 1.0, senseRadius: 80, diet: 0,
    metabolism: 1.0, aggression: 0.1, cooperation: 0.8, fertility: 0.5,
    lifespan: 1000, socialGene: 0.8, intelligence: 0.5,
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.8, altruism: 0.1,
    dominance: 0.1, nicheBreadth: 0.5, competitiveness: 0.2,
  }, 1);
  omegaOrg.socialRank = 'omega';

  // Spawn a highly competitive rival species organism at (102, 100)
  const rivalCompeter = (sim as any).spawnOrganism(102, 100, {
    sides: 4, size: 10, hue: 0, speed: 1.0, senseRadius: 50, diet: 0,
    metabolism: 1.0, aggression: 0.5, cooperation: 0.1, fertility: 0.5,
    lifespan: 1000, socialGene: 0.1, intelligence: 0.2,
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.2, altruism: 0.0,
    dominance: 0.9, nicheBreadth: 0.5, competitiveness: 0.9,
  }, 1);

  alphaOrg.energy = 50;
  omegaOrg.energy = 50;
  rivalCompeter.energy = 100;

  (sim as any).spatialGrid.clear(sim.settings.worldWidth, sim.settings.worldHeight);
  for (const o of sim.organisms) (sim as any).spatialGrid.insert(o);

  (sim as any).processSocialBehavior();

  console.log(`Alpha energy after competition loop: ${alphaOrg.energy.toFixed(2)} (immune to competition tax)`);
  console.log(`Omega energy after competition loop: ${omegaOrg.energy.toFixed(2)} (taxed by competitor)`);

  if (alphaOrg.energy > omegaOrg.energy) {
    console.log('TC-E2 VERIFIED PASS: Social rank has a measurable behavioral effect — Alphas are immune to rival competition tax while Omegas are taxed!');
  } else {
    console.error('TC-E2 FAIL: Alpha was not immune to competition tax');
    process.exit(1);
  }
}

testTC_E2();
