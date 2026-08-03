import type { Organism, Genome } from './types';
import type { ChemicalField } from './chemicalField';
import { Rng } from './rng';

// Microbial behavior systems: the kind of intelligence that real bacteria
// and single-celled organisms exhibit. These operate at the low end of
// the intelligence spectrum, before neural brains evolve.

export type BiofilmCluster = {
  id: number;
  cx: number;
  cy: number;
  radius: number;
  memberCount: number;
  speciesId: number;
  age: number;
};

// ─── Run-and-tumble movement ──────────────────────────────────────────
// Bacteria move in straight "runs" and randomly reorient in "tumbles".
// When following a favorable chemical gradient, runs lengthen (less tumbling).
// When moving toward unfavorable conditions, tumbles increase (reorient).

export function runAndTumble(
  org: Organism,
  field: ChemicalField | null,
  rng: Rng,
): { angle: number; speedMod: number } {
  // Base tumble frequency: every 20-60 ticks
  const baseTumbleInterval = 40 - org.genome.chemotaxis * 20;

  if (org.tumbleTimer <= 0) {
    // Time to tumble — pick a new direction
    let newAngle = rng.range(0, Math.PI * 2);

    // If chemotaxis is active and we have a chemical field, bias the new
    // direction toward the attractant gradient and away from repellents
    if (field && org.genome.chemotaxis > 0.2) {
      const att = field.sampleAttractant(org.x, org.y);
      const rep = field.sampleRepellent(org.x, org.y);

      // Attractant gradient: steer toward higher concentration
      const attStrength = Math.hypot(att.gradX, att.gradY);
      if (attStrength > 0.01) {
        const gradAngle = Math.atan2(att.gradY, att.gradX);
        const bias = org.genome.chemotaxis * 0.8;
        newAngle = blendAngles(newAngle, gradAngle, bias);
      }

      // Repellent gradient: steer away from higher concentration
      const repStrength = Math.hypot(rep.gradX, rep.gradY);
      if (repStrength > 0.01) {
        const awayAngle = Math.atan2(-rep.gradY, -rep.gradX);
        const bias = org.genome.chemotaxis * 0.6;
        newAngle = blendAngles(newAngle, awayAngle, bias);
      }

      // If we're in a good spot (high attractant), lengthen the run
      if (att.value > 0.5) {
        org.tumbleTimer = baseTumbleInterval * (1 + org.genome.chemotaxis);
      } else {
        org.tumbleTimer = baseTumbleInterval;
      }
    } else {
      org.tumbleTimer = baseTumbleInterval + rng.range(-10, 10);
    }

    return { angle: newAngle, speedMod: 1 };
  }

  org.tumbleTimer--;
  // Continue running in the same direction
  return { angle: org.angle, speedMod: 1 };
}

function blendAngles(a: number, b: number, t: number): number {
  let diff = b - a;
  while (diff > Math.PI) diff -= Math.PI * 2;
  while (diff < -Math.PI) diff += Math.PI * 2;
  return a + diff * t;
}

// ─── Quorum sensing ───────────────────────────────────────────────────
// Bacteria sense local population density and coordinate behavior.
// At high density, they may form biofilms, increase reproduction,
// or coordinate attacks. Returns the local density (0..1) and a
// coordination bonus.

export function quorumSense(
  org: Organism,
  organisms: Organism[],
  senseRadius: number,
): { density: number; neighbors: number; sameSpeciesNeighbors: number } {
  const r2 = senseRadius * senseRadius;
  let neighbors = 0;
  let sameSpecies = 0;

  for (const other of organisms) {
    if (other === org || !other.alive) continue;
    const dx = other.x - org.x;
    const dy = other.y - org.y;
    if (dx * dx + dy * dy < r2) {
      neighbors++;
      if (other.speciesId === org.speciesId) sameSpecies++;
    }
  }

  // Density normalized: 15+ neighbors = saturated
  const density = Math.min(1, neighbors / 15);
  return { density, neighbors, sameSpeciesNeighbors: sameSpecies };
}

// ─── Biofilm formation ──────────────────────────────────────────────
// When quorum sensing detects high density and the organism has biofilm
// genes, it joins or forms a biofilm cluster. Biofilms provide protection
// (reduced energy drain, immunity to predation) but reduce movement.

export function updateBiofilms(
  organisms: Organism[],
  biofilms: BiofilmCluster[],
  rng: Rng,
  nextId: number,
): { biofilms: BiofilmCluster[]; nextId: number } {
  // Reset cluster membership
  for (const org of organisms) {
    if (!org.alive) continue;
    org.inBiofilm = false;
    org.biofilmId = null;
  }

  const activeBiofilms: BiofilmCluster[] = [];
  let id = nextId;

  // Group organisms by proximity — simple spatial clustering
  const visited = new Set<number>();
  for (const org of organisms) {
    if (!org.alive || visited.has(org.id)) continue;
    if (org.genome.biofilmFormation < 0.3) continue;

    // Find nearby organisms with biofilm genes
    const cluster: Organism[] = [org];
    visited.add(org.id);

    for (const other of organisms) {
      if (!other.alive || visited.has(other.id)) continue;
      if (other.genome.biofilmFormation < 0.3) continue;
      const dx = other.x - org.x;
      const dy = other.y - org.y;
      if (dx * dx + dy * dy < 2500) { // 50px radius
        cluster.push(other);
        visited.add(other.id);
      }
    }

    // Need at least 3 organisms to form a biofilm
    if (cluster.length >= 3) {
      let cx = 0, cy = 0;
      for (const o of cluster) { cx += o.x; cy += o.y; }
      cx /= cluster.length;
      cy /= cluster.length;

      const speciesId = cluster[0].speciesId;
      const biofilm: BiofilmCluster = {
        id: id++,
        cx, cy,
        radius: 30 + cluster.length * 2,
        memberCount: cluster.length,
        speciesId,
        age: 0,
      };

      for (const o of cluster) {
        o.inBiofilm = true;
        o.biofilmId = biofilm.id;
      }

      activeBiofilms.push(biofilm);
    }
  }

  return { biofilms: activeBiofilms, nextId: id };
}

// ─── Sporulation ─────────────────────────────────────────────────────
// Under harsh conditions (low energy, bad biome, high threat), organisms
// with sporulation genes can enter a dormant spore state. Spores survive
// harsh conditions with minimal energy cost but cannot move or reproduce.

export function trySporulation(
  org: Organism,
  biomeEnergyDrain: number,
  threatLevel: number,
): boolean {
  if (org.genome.sporulation < 0.2) return false;
  if (org.sporeMode) {
    // Exit spore mode when conditions improve
    org.sporeTimer--;
    if (org.sporeTimer <= 0 && org.energy > 50 && threatLevel < 0.3) {
      org.sporeMode = false;
      org.sporeTimer = 0;
      return false;
    }
    return true; // still in spore mode
  }

  // Enter spore mode under harsh conditions
  const harshness = (biomeEnergyDrain - 1) * 2 + threatLevel;
  const energyStress = org.energy < 40 ? 1 : 0;
  if (harshness > 0.5 && energyStress > 0 && org.genome.sporulation > rngThreshold(org.genome.sporulation)) {
    org.sporeMode = true;
    org.sporeTimer = 200 + Math.floor(harshness * 100);
    return true;
  }

  return false;
}

function rngThreshold(rate: number): number {
  return 1 - rate;
}

// ─── Horizontal gene transfer (HGT) ─────────────────────────────────
// Bacteria share genes with nearby organisms, spreading beneficial
// mutations through the population without reproduction. This is a key
// driver of rapid bacterial evolution (e.g. antibiotic resistance spread).

export function tryHGT(
  org: Organism,
  organisms: Organism[],
  rng: Rng,
): { donor: Organism; gene: keyof Genome } | null {
  if (org.genome.hgtRate < 0.1) return null;
  if (rng.next() > org.genome.hgtRate * 0.02) return null; // rare event per tick

  // Find a nearby organism
  for (const other of organisms) {
    if (other === org || !other.alive) continue;
    const dx = other.x - org.x;
    const dy = other.y - org.y;
    if (dx * dx + dy * dy > 400) continue; // within 20px

    // Only transfer if the other organism has a higher value in some gene
    const genes: (keyof Genome)[] = [
      'chemotaxis', 'quorumSensing', 'biofilmFormation', 'sporulation',
      'intelligence', 'adaptability', 'cooperation', 'speed',
    ];

    for (const gene of genes) {
      const donorVal = other.genome[gene] as number;
      const recipientVal = org.genome[gene] as number;
      if (donorVal > recipientVal + 0.05) {
        return { donor: other, gene };
      }
    }
  }

  return null;
}

export function applyHGT(
  recipient: Organism,
  donor: Organism,
  gene: keyof Genome,
): void {
  const donorVal = donor.genome[gene] as number;
  const recipientVal = recipient.genome[gene] as number;
  // Transfer partial value — move partway toward donor's value
  (recipient.genome[gene] as number) = recipientVal + (donorVal - recipientVal) * 0.3;
}
