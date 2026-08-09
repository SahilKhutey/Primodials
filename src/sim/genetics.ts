import { Rng } from './rng';
import type { Genome } from './types';
import { makeBrain, mutateBrain, crossoverBrain, cloneBrain, hiddenForIntel, resizeBrain, N_INPUTS, N_OUTPUTS, type Brain } from './brain';

export function randomGenome(rng: Rng): Genome {
  return {
    sides: rng.int(3, 8),
    size: rng.range(6, 14),
    hue: rng.range(0, 360),
    speed: rng.range(0.4, 1.6),
    senseRadius: rng.range(40, 120),
    diet: rng.next() < 0.15 ? 1 : 0,
    metabolism: rng.range(0.7, 1.3),
    aggression: rng.range(0, 1),
    cooperation: rng.range(0, 1),
    fertility: rng.range(0.4, 1),
    lifespan: rng.range(400, 900),
    socialGene: rng.range(0, 1),
    intelligence: rng.range(0, 0.6),
    buildSkill: rng.range(0, 0.4),
    adaptability: rng.range(0, 0.5),
    curiosity: rng.range(0, 0.5),
    chemotaxis: rng.range(0.3, 0.8),
    quorumSensing: rng.range(0, 0.5),
    biofilmFormation: rng.range(0, 0.4),
    sporulation: rng.range(0, 0.3),
    hgtRate: rng.range(0, 0.3),
    photosynthesis: rng.range(0, 0.3),
    symbiosis: rng.range(0, 0.3),
    parasitism: rng.range(0, 0.2),
    diseaseResistance: rng.range(0.2, 0.6),
    agingRate: rng.range(0.7, 1.3),
    toxicity: rng.range(0, 0.2),
    clustering: rng.range(0.1, 0.5),
    altruism: rng.range(0, 0.3),
    dominance: rng.range(0.1, 0.5),
    nicheBreadth: rng.range(0.3, 0.7),
    competitiveness: rng.range(0.2, 0.6),
    camouflage: rng.range(0, 0.4),
    bioluminescence: rng.range(0, 0.4),
    echolocation: rng.range(0, 0.4),
    hibernation: rng.range(0, 0.4),
  };
}

export function makeBrainForGenome(genome: Genome, rng: Rng): Brain | null {
  const hidden = hiddenForIntel(genome.intelligence);
  if (hidden === 0) return null;
  return makeBrain(14, hidden, 5, rng);
}

function clamp(v: number, min: number, max: number): number {
  return Math.max(min, Math.min(max, v));
}

const EVOLUTION_LEAP_CHANCE = 0.003; // per mutation, chance of a beneficial leap

export function mutateGenome(g: Genome, mutationRate: number, rng: Rng): Genome {
  const m = (v: number, lo: number, hi: number, amt: number) =>
    rng.next() < mutationRate ? clamp(v + rng.range(-amt, amt), lo, hi) : v;

  // Evolution leap: rare beneficial mutation that boosts intelligence or build skill
  const leap = rng.next() < EVOLUTION_LEAP_CHANCE;
  let intelBoost = 0;
  let buildBoost = 0;
  if (leap) {
    const choice = rng.next();
    if (choice < 0.5) intelBoost = rng.range(0.05, 0.15);
    else buildBoost = rng.range(0.05, 0.12);
  }

  // Build every trait except hue first. Hue is deliberately computed last so
  // its drift can be scaled by how much the REST of the genome actually
  // changed (see below) — a lineage that mutated very little elsewhere
  // should look nearly identical to its parent; one that changed a lot
  // (a bigger evolutionary step) can drift further in color too. Without
  // this, hue was a fully independent random walk with no relationship to
  // the rest of the genome, so two visually-similar-looking creatures could
  // be genetically distant, and close relatives could look unrelated by pure
  // chance — undermining "watch the family tree branch with your eyes."
  const withoutHue: Omit<Genome, 'hue'> = {
    sides: clamp(Math.round(m(g.sides, 3, 8, 2)), 3, 8),
    size: clamp(m(g.size, 4, 18, 3), 4, 18),
    speed: clamp(m(g.speed, 0.2, 2.4, 0.3), 0.2, 2.4),
    senseRadius: clamp(m(g.senseRadius, 30, 160, 25), 30, 160),
    diet: rng.next() < mutationRate * 0.3 ? (g.diet === 0 ? 1 : 0) : g.diet,
    metabolism: clamp(m(g.metabolism, 0.5, 1.6, 0.2), 0.5, 1.6),
    aggression: clamp(m(g.aggression, 0, 1, 0.2), 0, 1),
    cooperation: clamp(m(g.cooperation, 0, 1, 0.2), 0, 1),
    fertility: clamp(m(g.fertility, 0.2, 1, 0.15), 0.2, 1),
    lifespan: clamp(m(g.lifespan, 300, 1200, 100), 300, 1200),
    socialGene: clamp(m(g.socialGene, 0, 1, 0.2), 0, 1),
    intelligence: clamp(m(g.intelligence, 0, 1, 0.1) + intelBoost, 0, 1),
    buildSkill: clamp(m(g.buildSkill, 0, 1, 0.12) + buildBoost, 0, 1),
    adaptability: clamp(m(g.adaptability, 0, 1, 0.1), 0, 1),
    curiosity: clamp(m(g.curiosity, 0, 1, 0.1), 0, 1),
    chemotaxis: clamp(m(g.chemotaxis, 0, 1, 0.1), 0, 1),
    quorumSensing: clamp(m(g.quorumSensing, 0, 1, 0.1), 0, 1),
    biofilmFormation: clamp(m(g.biofilmFormation, 0, 1, 0.1), 0, 1),
    sporulation: clamp(m(g.sporulation, 0, 1, 0.08), 0, 1),
    hgtRate: clamp(m(g.hgtRate, 0, 1, 0.08), 0, 1),
    photosynthesis: clamp(m(g.photosynthesis, 0, 1, 0.08), 0, 1),
    symbiosis: clamp(m(g.symbiosis, 0, 1, 0.08), 0, 1),
    parasitism: clamp(m(g.parasitism, 0, 1, 0.06), 0, 1),
    diseaseResistance: clamp(m(g.diseaseResistance, 0, 1, 0.08), 0, 1),
    agingRate: clamp(m(g.agingRate, 0.3, 2, 0.1), 0.3, 2),
    toxicity: clamp(m(g.toxicity, 0, 1, 0.06), 0, 1),
    clustering: clamp(m(g.clustering, 0, 1, 0.08), 0, 1),
    altruism: clamp(m(g.altruism, 0, 1, 0.06), 0, 1),
    dominance: clamp(m(g.dominance, 0, 1, 0.08), 0, 1),
    nicheBreadth: clamp(m(g.nicheBreadth, 0, 1, 0.08), 0, 1),
    competitiveness: clamp(m(g.competitiveness, 0, 1, 0.06), 0, 1),
    camouflage: clamp(m(g.camouflage, 0, 1, 0.08), 0, 1),
    bioluminescence: clamp(m(g.bioluminescence, 0, 1, 0.08), 0, 1),
    echolocation: clamp(m(g.echolocation, 0, 1, 0.08), 0, 1),
    hibernation: clamp(m(g.hibernation, 0, 1, 0.08), 0, 1),
  };

  // Distance contributed by everything except hue, using the same weights
  // geneticDistance() uses (duplicated rather than imported to avoid a
  // circular dependency, since geneticDistance operates on full Genomes).
  // A typical single mutation event (only a few traits actually change,
  // since each is gated by mutationRate independently) lands well under 1.0
  // on this scale; a rare multi-trait jump or evolution leap lands higher.
  const nonHueDistance =
    Math.abs(g.sides - withoutHue.sides) / 5 * 1.5 +
    Math.abs(g.size - withoutHue.size) / 14 * 2 +
    Math.abs(g.speed - withoutHue.speed) / 2.4 * 1.5 +
    Math.abs(g.senseRadius - withoutHue.senseRadius) / 130 * 1 +
    Math.abs(g.diet - withoutHue.diet) * 4 +
    Math.abs(g.intelligence - withoutHue.intelligence) * 3 +
    Math.abs(g.buildSkill - withoutHue.buildSkill) * 2 +
    Math.abs(g.socialGene - withoutHue.socialGene) * 2;

  // Small non-hue change → small hue drift (stays visually close to parent).
  // Larger change → allowed to drift further, up to the original 30° ceiling
  // so overall population color variety over time is unchanged — only the
  // *correlation* with ancestry is new, not the total range of possible hues.
  const hueMutAmount = clamp(4 + nonHueDistance * 22, 4, 30);

  return {
    ...withoutHue,
    hue: (m(g.hue, 0, 360, hueMutAmount) + 360) % 360,
  };
}

export function hadEvolutionLeap(parent: Genome, child: Genome): boolean {
  return (
    child.intelligence > parent.intelligence + 0.04 ||
    child.buildSkill > parent.buildSkill + 0.04 ||
    child.chemotaxis > parent.chemotaxis + 0.08 ||
    child.quorumSensing > parent.quorumSensing + 0.08 ||
    child.biofilmFormation > parent.biofilmFormation + 0.08 ||
    child.sporulation > parent.sporulation + 0.06 ||
    child.photosynthesis > parent.photosynthesis + 0.08 ||
    child.symbiosis > parent.symbiosis + 0.08 ||
    child.diseaseResistance > parent.diseaseResistance + 0.08 ||
    child.clustering > parent.clustering + 0.08 ||
    child.altruism > parent.altruism + 0.08 ||
    child.dominance > parent.dominance + 0.08 ||
    child.nicheBreadth > parent.nicheBreadth + 0.08 ||
    child.bioluminescence > parent.bioluminescence + 0.08 ||
    child.echolocation > parent.echolocation + 0.08
  );
}

export function crossoverGenome(a: Genome, b: Genome, rng: Rng): Genome {
  const avg = (x: number, y: number) => (x + y) / 2;

  return {
    sides: Math.round(avg(a.sides, b.sides)),
    size: avg(a.size, b.size),
    hue: blendHue(a.hue, b.hue, rng.next()),
    speed: avg(a.speed, b.speed),
    senseRadius: avg(a.senseRadius, b.senseRadius),
    diet: avg(a.diet, b.diet) >= 0.5 ? 1 : 0,
    metabolism: avg(a.metabolism, b.metabolism),
    aggression: avg(a.aggression, b.aggression),
    cooperation: avg(a.cooperation, b.cooperation),
    fertility: avg(a.fertility, b.fertility),
    lifespan: avg(a.lifespan, b.lifespan),
    socialGene: avg(a.socialGene, b.socialGene),
    intelligence: clamp(avg(a.intelligence, b.intelligence) + 0.03, 0, 1),
    buildSkill: clamp(avg(a.buildSkill, b.buildSkill) + 0.02, 0, 1),
    adaptability: clamp(avg(a.adaptability, b.adaptability) + 0.01, 0, 1),
    curiosity: clamp(avg(a.curiosity, b.curiosity) + 0.01, 0, 1),
    chemotaxis: avg(a.chemotaxis, b.chemotaxis),
    quorumSensing: avg(a.quorumSensing, b.quorumSensing),
    biofilmFormation: avg(a.biofilmFormation, b.biofilmFormation),
    sporulation: avg(a.sporulation, b.sporulation),
    hgtRate: avg(a.hgtRate, b.hgtRate),
    photosynthesis: avg(a.photosynthesis, b.photosynthesis),
    symbiosis: avg(a.symbiosis, b.symbiosis),
    parasitism: avg(a.parasitism, b.parasitism),
    diseaseResistance: avg(a.diseaseResistance, b.diseaseResistance),
    agingRate: avg(a.agingRate, b.agingRate),
    toxicity: avg(a.toxicity, b.toxicity),
    clustering: avg(a.clustering, b.clustering),
    altruism: avg(a.altruism, b.altruism),
    dominance: avg(a.dominance, b.dominance),
    nicheBreadth: avg(a.nicheBreadth, b.nicheBreadth),
    competitiveness: avg(a.competitiveness, b.competitiveness),
    camouflage: avg(a.camouflage, b.camouflage),
    bioluminescence: avg(a.bioluminescence, b.bioluminescence),
    echolocation: avg(a.echolocation, b.echolocation),
    hibernation: avg(a.hibernation, b.hibernation),
  };
}

function blendHue(h1: number, h2: number, t: number): number {
  const diff = Math.abs(h1 - h2);
  if (diff > 180) {
    if (h1 < h2) h1 += 360;
    else h2 += 360;
  }
  const result = h1 * (1 - t) + h2 * t;
  return ((result % 360) + 360) % 360;
}

export function geneticDistance(a: Genome, b: Genome): number {
  const dHue = Math.min(Math.abs(a.hue - b.hue), 360 - Math.abs(a.hue - b.hue)) / 360;
  const dSides = Math.abs(a.sides - b.sides) / 5;
  const dSize = Math.abs(a.size - b.size) / 14;
  const dSpeed = Math.abs(a.speed - b.speed) / 2.4;
  const dSense = Math.abs(a.senseRadius - b.senseRadius) / 130;
  const dDiet = Math.abs(a.diet - b.diet);
  const dMeta = Math.abs(a.metabolism - b.metabolism) / 1.1;
  const dAggr = Math.abs(a.aggression - b.aggression);
  const dCoop = Math.abs(a.cooperation - b.cooperation);
  const dSocial = Math.abs(a.socialGene - b.socialGene);
  const dIntel = Math.abs(a.intelligence - b.intelligence);
  const dBuild = Math.abs(a.buildSkill - b.buildSkill);
  const dAdapt = Math.abs(a.adaptability - b.adaptability);
  const dCurio = Math.abs(a.curiosity - b.curiosity);
  const dChemotaxis = Math.abs(a.chemotaxis - b.chemotaxis);
  const dQuorum = Math.abs(a.quorumSensing - b.quorumSensing);
  const dBiofilm = Math.abs(a.biofilmFormation - b.biofilmFormation);
  const dSporulation = Math.abs(a.sporulation - b.sporulation);
  const dHgt = Math.abs(a.hgtRate - b.hgtRate);
  const dPhoto = Math.abs(a.photosynthesis - b.photosynthesis);
  const dSymb = Math.abs(a.symbiosis - b.symbiosis);
  const dPara = Math.abs(a.parasitism - b.parasitism);
  const dDisease = Math.abs(a.diseaseResistance - b.diseaseResistance);
  const dAging = Math.abs(a.agingRate - b.agingRate);
  const dTox = Math.abs(a.toxicity - b.toxicity);
  const dCluster = Math.abs(a.clustering - b.clustering);
  const dAltruism = Math.abs(a.altruism - b.altruism);
  const dDominance = Math.abs(a.dominance - b.dominance);
  const dNiche = Math.abs(a.nicheBreadth - b.nicheBreadth);
  const dCompete = Math.abs(a.competitiveness - b.competitiveness);
  return (
    dHue * 3 +
    dSides * 1.5 +
    dSize * 2 +
    dSpeed * 1.5 +
    dSense * 1 +
    dDiet * 4 +
    dMeta * 1 +
    dAggr * 1.5 +
    dCoop * 1.5 +
    dSocial * 2 +
    dIntel * 3 +
    dBuild * 2 +
    dAdapt * 1.5 +
    dCurio * 1.5 +
    dChemotaxis * 1.5 +
    dQuorum * 1.2 +
    dBiofilm * 1.5 +
    dSporulation * 1 +
    dHgt * 1 +
    dPhoto * 1.2 +
    dSymb * 1.5 +
    dPara * 1.5 +
    dDisease * 1.2 +
    dAging * 1 +
    dTox * 1.2 +
    dCluster * 1.5 +
    dAltruism * 1.2 +
    dDominance * 1 +
    dNiche * 1.5 +
    dCompete * 1 +
    Math.abs(a.camouflage - b.camouflage) * 1.2 +
    Math.abs(a.bioluminescence - b.bioluminescence) * 1.5 +
    Math.abs(a.echolocation - b.echolocation) * 1.5 +
    Math.abs(a.hibernation - b.hibernation) * 1.2
  );
}

export const SPECIATION_THRESHOLD = 3.5;

export function mutateBrainForGenome(parent: Genome, parentBrain: Brain | null, mutationRate: number, rng: Rng): Brain | null {
  const newHidden = hiddenForIntel(parent.intelligence);
  if (newHidden === 0) return null;
  if (parentBrain) {
    const resized = resizeBrain(parentBrain, newHidden, rng);
    return mutateBrain(resized, mutationRate, rng);
  }
  return makeBrain(N_INPUTS, newHidden, N_OUTPUTS, rng);
}

export function crossoverBrainForGenome(
  genomeA: Genome, genomeB: Genome,
  brainA: Brain | null, brainB: Brain | null,
  rng: Rng,
): Brain | null {
  const avgIntel = (genomeA.intelligence + genomeB.intelligence) / 2;
  const newHidden = hiddenForIntel(avgIntel);
  if (newHidden === 0) return null;

  const rA = brainA ? resizeBrain(brainA, newHidden, rng) : null;
  const rB = brainB ? resizeBrain(brainB, newHidden, rng) : null;

  if (rA && rB) {
    return crossoverBrain(rA, rB, rng);
  }
  if (rA) return mutateBrain(rA, mutationRateForCrossover(0.05), rng);
  if (rB) return mutateBrain(rB, mutationRateForCrossover(0.05), rng);
  return makeBrain(N_INPUTS, newHidden, N_OUTPUTS, rng);
}

function mutationRateForCrossover(val: number): number {
  return val;
}
