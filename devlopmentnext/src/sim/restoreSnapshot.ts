import { Simulation } from "./simulation";
import type { SimSettings } from "./types";
import type { LocalSimulationSnapshot } from "./localSnapshot";

export function restoreSimulationSnapshot(
  snapshot: LocalSimulationSnapshot,
): Simulation {
  const sim = new Simulation(Date.now(), snapshot.settings as SimSettings);
  sim.tick = snapshot.tick;
  sim.stats = snapshot.stats;
  sim.food = snapshot.food;
  sim.species = snapshot.species.map((s) => ({
    ...s,
    representative: {
      ...s.representative,
      camouflage: s.representative.camouflage ?? 0,
      bioluminescence: s.representative.bioluminescence ?? 0,
      echolocation: s.representative.echolocation ?? 0,
      hibernation: s.representative.hibernation ?? 0,
    },
    avgIntelligence: s.avgIntelligence ?? 0,
    rank: s.rank ?? 0,
    totalKills: s.totalKills ?? 0,
    structuresBuilt: s.structuresBuilt ?? 0,
    civilizationLevel: s.civilizationLevel ?? 0,
    knowledgeDiscovered: s.knowledgeDiscovered ?? 0,
    evolutionLeaps: s.evolutionLeaps ?? 0,
  }));

  sim.colonies = snapshot.colonies ?? [];
  sim.structures = snapshot.structures ?? [];
  sim.biomes = snapshot.biomes ?? [];
  sim.remains = snapshot.remains ?? [];
  sim.knowledgeNodes = snapshot.knowledgeNodes ?? [];

  sim.organisms = snapshot.organisms.map((o) => ({
    ...o,
    vx: 0,
    vy: 0,
    alive: true,
    reproductionCooldown: o.reproductionCooldown ?? 30,
    threatLevel: o.threatLevel ?? 0,
    buildCooldown: o.buildCooldown ?? 0,
    carrying: o.carrying ?? 0,
    knowledgeBoost: o.knowledgeBoost ?? 0,
    biomeId: o.biomeId ?? 0,
    brain: o.brain
      ? {
          nInputs: o.brain.nInputs,
          nHidden: o.brain.nHidden,
          nOutputs: o.brain.nOutputs,
          weights: new Float32Array(o.brain.weights),
        }
      : null,
    fitness: o.fitness ?? 0,
    lastInputs: null,
    lastOutputs: null,
    tumbleTimer: o.tumbleTimer ?? 0,
    inBiofilm: o.inBiofilm ?? false,
    biofilmId: o.biofilmId ?? null,
    sporeMode: o.sporeMode ?? false,
    sporeTimer: o.sporeTimer ?? 0,
    infected: o.infected ?? false,
    infectionTimer: o.infectionTimer ?? 0,
    symbiosisPartner: o.symbiosisPartner ?? null,
    socialRank:
      (o.socialRank as "alpha" | "beta" | "omega" | "solitary") ?? "solitary",
    clusterId: o.clusterId ?? null,
    hibernating: o.hibernating ?? false,
    sonarPulse: o.sonarPulse ?? 0,
    leapTimer: o.leapTimer ?? 0,
    speciationTimer: o.speciationTimer ?? 0,
  }));

  return sim;
}
