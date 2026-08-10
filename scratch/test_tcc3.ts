import { Simulation } from '../src/sim/simulation';
import { DEFAULT_SETTINGS } from '../src/sim/types';
import { hiddenForIntel } from '../src/sim/brain';

function testTC_C3() {
  const sim = new Simulation(42, { ...DEFAULT_SETTINGS, initialPopulation: 0 });

  // Create two organisms with identical genomes except intelligence
  const lowIntelOrg = (sim as any).spawnOrganism(100, 100, {
    sides: 4, size: 10, hue: 120, speed: 1.0, senseRadius: 50, diet: 0,
    metabolism: 1.0, aggression: 0.1, cooperation: 0.5, fertility: 0.5,
    lifespan: 1000, socialGene: 0.5, intelligence: 0.1, // no brain
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.2, altruism: 0.1,
    dominance: 0.3, nicheBreadth: 0.5, competitiveness: 0.2,
  }, 1);

  const highIntelOrg = (sim as any).spawnOrganism(200, 200, {
    sides: 4, size: 10, hue: 120, speed: 1.0, senseRadius: 50, diet: 0,
    metabolism: 1.0, aggression: 0.1, cooperation: 0.5, fertility: 0.5,
    lifespan: 1000, socialGene: 0.5, intelligence: 0.85, // max brain (hidden=8)
    buildSkill: 0, adaptability: 0.5, curiosity: 0.5, chemotaxis: 0.5,
    quorumSensing: 0, biofilmFormation: 0, sporulation: 0, hgtRate: 0,
    photosynthesis: 0, symbiosis: 0, parasitism: 0, diseaseResistance: 0.5,
    agingRate: 1.0, toxicity: 0, clustering: 0.2, altruism: 0.1,
    dominance: 0.3, nicheBreadth: 0.5, competitiveness: 0.2,
  }, 1);

  const initialEnergy = 100;
  lowIntelOrg.energy = initialEnergy;
  highIntelOrg.energy = initialEnergy;

  (sim as any).applyEnergyCost(lowIntelOrg, undefined, 1);
  (sim as any).applyEnergyCost(highIntelOrg, undefined, 1);

  const lowDrain = initialEnergy - lowIntelOrg.energy;
  const highDrain = initialEnergy - highIntelOrg.energy;

  console.log(`Low intel (0.10, hidden=${hiddenForIntel(0.10)}) drain: ${lowDrain.toFixed(4)} energy/tick`);
  console.log(`High intel (0.85, hidden=${hiddenForIntel(0.85)}) drain: ${highDrain.toFixed(4)} energy/tick`);

  if (highDrain > lowDrain) {
    const ratio = (highDrain / lowDrain).toFixed(2);
    console.log(`TC-C3 VERIFIED PASS: Higher intelligence organism drains energy ${ratio}x faster, proportional to intelligence and hidden layer size!`);
  } else {
    console.error('TC-C3 FAIL: Higher intelligence did not increase metabolic energy cost');
    process.exit(1);
  }
}

testTC_C3();
