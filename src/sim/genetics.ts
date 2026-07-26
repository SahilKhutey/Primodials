import { Rng } from './rng';
import type { Genome } from './types';

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
  };
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

  return {
    sides: clamp(Math.round(m(g.sides, 3, 8, 2)), 3, 8),
    size: clamp(m(g.size, 4, 18, 3), 4, 18),
    hue: (m(g.hue, 0, 360, 30) + 360) % 360,
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
  };
}

export function hadEvolutionLeap(parent: Genome, child: Genome): boolean {
  return child.intelligence > parent.intelligence + 0.04 || child.buildSkill > parent.buildSkill + 0.04;
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
    dCurio * 1.5
  );
}

export const SPECIATION_THRESHOLD = 3.5;
