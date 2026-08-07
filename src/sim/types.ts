import type { Brain } from './brain';

export type Genome = {
  sides: number;
  size: number;
  hue: number;
  speed: number;
  senseRadius: number;
  diet: number;
  metabolism: number;
  aggression: number;
  cooperation: number;
  fertility: number;
  lifespan: number;
  socialGene: number;
  intelligence: number;
  buildSkill: number;
  adaptability: number; // resistance to biome penalties + faster evolution in harsh biomes
  curiosity: number;   // drives exploration toward knowledge nodes
  // Microbial evolution genes (active at low intelligence tiers)
  chemotaxis: number;    // ability to follow chemical gradients toward food / away from danger
  quorumSensing: number; // ability to coordinate behavior based on local population density
  biofilmFormation: number; // tendency to cluster and form protective biofilm communities
  sporulation: number;  // ability to form spores under harsh conditions (survival pods)
  hgtRate: number;       // horizontal gene transfer rate — shares beneficial mutations with neighbors
  // Advanced biological genes
  photosynthesis: number;  // ability to generate energy from ambient light (in lit biomes)
  symbiosis: number;      // tendency to form mutually beneficial relationships with nearby organisms
  parasitism: number;     // tendency to exploit other organisms without killing them (drains energy)
  diseaseResistance: number; // resistance to disease events that periodically sweep the population
  agingRate: number;      // rate of senescence — lower = longer functional lifespan
  toxicity: number;      // produces repellent chemicals that deter predators (toxic species)
  // Social & ecological strategy genes
  clustering: number;     // tendency to form dense spatial clusters with same-species neighbors
  altruism: number;       // kin selection — share energy with nearby same-species organisms in need
  dominance: number;      // drive to assert social rank within colony hierarchy
  nicheBreadth: number;   // ecological niche width — broad niches tolerate more biomes, narrow ones specialize
  competitiveness: number; // tendency to compete for resources rather than share
};

export type Organism = {
  id: number;
  x: number;
  y: number;
  vx: number;
  vy: number;
  angle: number;
  energy: number;
  age: number;
  generation: number;
  genome: Genome;
  speciesId: number;
  alive: boolean;
  reproductionCooldown: number;
  colonyId: number | null;
  colonyRole: 'leader' | 'member' | 'solitary';
  threatLevel: number;
  buildCooldown: number;
  carrying: number;       // amount of dead material gathered for building
  knowledgeBoost: number; // temporary intelligence boost from discovered knowledge nodes
  biomeId: number;
  brain: Brain | null;    // neural network — null for instinctual creatures (intel < 0.2)
  fitness: number;        // accumulated fitness score for the current generation cycle
  lastInputs: Float32Array | null;  // last neural inputs (for visualization)
  lastOutputs: Float32Array | null; // last neural outputs (for visualization)
  // Microbial state
  tumbleTimer: number;    // run-and-tumble: ticks until next direction change
  inBiofilm: boolean;    // whether this organism is part of a biofilm cluster
  biofilmId: number | null; // which biofilm cluster it belongs to
  sporeMode: boolean;    // dormant survival state
  sporeTimer: number;     // how long until exiting spore mode
  // Advanced biological state
  infected: boolean;      // currently carrying a disease
  infectionTimer: number; // ticks until infection clears
  symbiosisPartner: number | null; // id of organism in symbiotic relationship
  // Social hierarchy state
  socialRank: 'alpha' | 'beta' | 'omega' | 'solitary'; // rank within colony hierarchy
  clusterId: number | null; // which spatial cluster this organism belongs to
};

export type Food = {
  id: number;
  x: number;
  y: number;
  energy: number;
};

export type StructureType = 'nest' | 'cache' | 'watchtower' | 'wall'
  | 'monument' | 'sanctuary' | 'observatory' | 'bridge';

export type StructureTier = 'basic' | 'advanced';

export const BASIC_STRUCTURES: StructureType[] = ['nest', 'cache', 'watchtower', 'wall'];
export const ADVANCED_STRUCTURES: StructureType[] = ['monument', 'sanctuary', 'observatory', 'bridge'];

export type Structure = {
  id: number;
  type: StructureType;
  x: number;
  y: number;
  speciesId: number;
  colonyId: number | null;
  hp: number;
  maxHp: number;
  radius: number;
  hue: number;
  age: number;
  materials: number;
  tier: StructureTier;
  contributingColonies: number[]; // for multi-colony blueprints
};

export type BiomeType = 'plains' | 'forest' | 'desert' | 'tundra' | 'wetland' | 'ocean' | 'volcanic' | 'toxic' | 'crystal';

export type Biome = {
  id: number;
  type: BiomeType;
  cx: number;
  cy: number;
  radius: number;
  foodRate: number;   // food growth multiplier
  energyDrain: number; // extra energy cost multiplier
  speedMod: number;    // movement speed multiplier
  hue: number;
};

export type Remains = {
  id: number;
  x: number;
  y: number;
  energy: number;
  age: number;
  hue: number;
};

export type ParticleType = 'birth' | 'death' | 'kill' | 'sparkle';

export type Particle = {
  x: number;
  y: number;
  vx: number;
  vy: number;
  life: number;
  maxLife: number;
  hue: number;
  type: ParticleType;
  size: number;
};

export type KnowledgeNode = {
  id: number;
  x: number;
  y: number;
  intelBoost: number;
  discovered: boolean;
  discoveredBy: number | null;
  age: number;
};

export type Species = {
  id: number;
  hue: number;
  count: number;
  representative: Genome;
  generations: number;
  avgIntelligence: number;
  rank: number;
  totalKills: number;
  structuresBuilt: number;
  civilizationLevel: number;
  knowledgeDiscovered: number;
  evolutionLeaps: number;
};

export type Colony = {
  id: number;
  speciesId: number;
  members: number;
  centerX: number;
  centerY: number;
  hue: number;
  nestId: number | null;
};

export type SimStats = {
  births: number;
  deaths: number;
  kills: number;
  maxGeneration: number;
  herbivores: number;
  carnivores: number;
  colonies: number;
  sexualReproductions: number;
  asexualReproductions: number;
  avgIntelligence: number;
  dominantSpeciesId: number | null;
  apexKills: number;
  structures: number;
  avgBuildSkill: number;
  remains: number;
  knowledgeDiscovered: number;
  evolutionLeaps: number;
  avgCuriosity: number;
  avgAdaptability: number;
  generationCycle: number;      // current generation cycle (epoch) number
  worldExpansions: number;      // how many times the world has expanded
  brainDrivenPopulation: number; // how many organisms use neural brains
  avgFitness: number;           // average fitness across alive organisms
  peakFitness: number;          // highest fitness this cycle
  // Microbial stats
  biofilmClusters: number;      // number of active biofilm clusters
  biofilmPopulation: number;    // organisms currently in biofilms
  spores: number;               // organisms currently in spore mode
  hgtEvents: number;            // total horizontal gene transfer events
  avgChemotaxis: number;        // average chemotaxis ability across population
  avgQuorum: number;            // average quorum sensing ability
  avgBiofilm: number;           // average biofilm formation tendency
  // Advanced biological stats
  photosynthesizers: number;    // organisms gaining energy from photosynthesis
  symbioticPairs: number;       // active symbiotic relationships
  parasiticEvents: number;      // total parasitism energy drains
  infected: number;             // currently infected organisms
  diseaseEvents: number;        // total disease outbreak events
  avgPhotosynthesis: number;    // average photosynthesis gene
  avgSymbiosis: number;         // average symbiosis gene
  avgToxicity: number;          // average toxicity gene
  newSpeciesIntroduced: number; // species introduced by endless mode
  // Social & ecological stats
  spatialClusters: number;      // number of distinct spatial clusters
  clusteredPopulation: number;  // organisms in clusters
  alphaCount: number;           // number of alpha-ranked organisms
  altruismEvents: number;       // total energy-sharing events
  avgClustering: number;        // average clustering gene
  avgAltruism: number;          // average altruism gene
  avgDominance: number;         // average dominance gene
  avgNicheBreadth: number;      // average niche breadth gene
  avgCompetitiveness: number;  // average competitiveness gene
  competitionEvents: number;   // total resource competition events
  neutralDriftEvents: number;  // random gene drift events (neutral theory)
};

export type HistoryPoint = {
  tick: number;
  population: number;
  speciesCount: number;
  avgIntelligence: number;
  avgFitness: number;
  herbivores: number;
  carnivores: number;
  colonies: number;
  structures: number;
  births: number;
  deaths: number;
  kills: number;
  biofilmPopulation: number;
  spores: number;
  hgtEvents: number;
  photosynthesizers: number;
  symbioticPairs: number;
  infected: number;
  spatialClusters: number;
  altruismEvents: number;
  competitionEvents: number;
  neutralDriftEvents: number;
  avgClustering: number;
  avgAltruism: number;
  avgDominance: number;
  avgNicheBreadth: number;
  avgCompetitiveness: number;
};

export type BoundaryMode = 'wrap' | 'reflect' | 'open';

export type Camera = {
  x: number;      // world coords at center of viewport
  y: number;
  zoom: number;   // 1 = fit world; >1 = zoomed in
};

export function defaultCamera(worldWidth: number, worldHeight: number): Camera {
  return { x: worldWidth / 2, y: worldHeight / 2, zoom: 1 };
}

export type SimSettings = {
  worldWidth: number;
  worldHeight: number;
  initialPopulation: number;
  initialFood: number;
  foodGrowthRate: number;
  mutationRate: number;
  reproductionThreshold: number;
  maxPopulation: number;
  maxFood: number;
  colonyFormation: boolean;
  sexualReproduction: boolean;
  colonyRadius: number;
  structureBuilding: boolean;
  biomes: boolean;
  knowledgeNodes: boolean;
  boundaryMode: BoundaryMode;
  blueprints: boolean;
  neuralBrains: boolean;        // evolved neural networks drive intelligent behavior
  generationCycleTicks: number;  // how many ticks per generation cycle (epoch)
  worldExpansion: boolean;      // world grows over time, adding new biomes and space
  worldExpansionInterval: number; // ticks between world expansions
  autoCheckpoint: boolean;      // auto-save snapshots at generation cycle boundaries
  chemicalField: boolean;        // simulate chemical gradients for chemotaxis
  microbialBehavior: boolean;    // enable bacterial-level behaviors (run-and-tumble, biofilm, quorum, HGT)
  endlessGeneration: boolean;    // auto-spawn new life and species when population drops
  diseaseEvents: boolean;        // periodic disease outbreaks that test resistance
  advancedBiology: boolean;      // photosynthesis, symbiosis, parasitism, toxicity
  socialBehavior: boolean;      // clustering, hierarchy, altruism, competition
  neutralDrift: boolean;         // random neutral genetic drift (neutral theory of evolution)
};

export const DEFAULT_SETTINGS: SimSettings = {
  worldWidth: 1200,
  worldHeight: 800,
  initialPopulation: 40,
  initialFood: 120,
  foodGrowthRate: 2,
  mutationRate: 0.15,
  reproductionThreshold: 140,
  maxPopulation: 400,
  maxFood: 250,
  colonyFormation: true,
  sexualReproduction: true,
  colonyRadius: 80,
  structureBuilding: true,
  biomes: true,
  knowledgeNodes: true,
  boundaryMode: 'wrap',
  blueprints: true,
  neuralBrains: true,
  generationCycleTicks: 600,   // 20 seconds at 30fps
  worldExpansion: true,
  worldExpansionInterval: 1200, // every 40 seconds at 30fps
  autoCheckpoint: true,
  chemicalField: true,          // chemical gradients for chemotaxis
  microbialBehavior: true,      // bacterial-level behaviors
  endlessGeneration: true,       // auto-spawn life when population drops
  diseaseEvents: true,           // periodic disease outbreaks
  advancedBiology: true,         // photosynthesis, symbiosis, parasitism, toxicity
  socialBehavior: true,          // clustering, hierarchy, altruism, competition
  neutralDrift: true,           // random neutral genetic drift
};

export type IntelligenceTier = { min: number; label: string; desc: string };

export const INTELLIGENCE_TIERS: readonly IntelligenceTier[] = [
  { min: 0.0, label: 'Microbial', desc: 'Single-cell intelligence: chemotaxis, run-and-tumble, biofilm clustering, quorum sensing. No brain — chemical-driven behavior.' },
  { min: 0.15, label: 'Instinctual', desc: 'Reacts to immediate stimuli. Beginning of direct sensory response. Chemical gradients still guide movement.' },
  { min: 0.2, label: 'Reactive', desc: 'Basic threat detection. Flees from larger predators. Neural brain begins forming.' },
  { min: 0.4, label: 'Adaptive', desc: 'Prefers richer food sources. Avoids dangerous areas. Begins building. Neural network guides decisions.' },
  { min: 0.6, label: 'Strategic', desc: 'Hunts in packs. Flanks prey. Prioritizes weak targets. Gathers materials. Larger neural network.' },
  { min: 0.8, label: 'Tactical', desc: 'Coordinated pack tactics. Defends colony intelligently. Seeks knowledge. Full neural control.' },
];

export function intelligenceTier(intelligence: number): IntelligenceTier {
  let tier = INTELLIGENCE_TIERS[0];
  for (const t of INTELLIGENCE_TIERS) {
    if (intelligence >= t.min) tier = t;
  }
  return tier;
}

export type BlueprintInfo = {
  label: string;
  desc: string;
  minIntel: number;
  minBuild: number;
  materialCost: number;
  tier: StructureTier;
  minColonySize: number;     // minimum members to attempt
  minEvolutionLeaps: number;  // species must have evolved this many times
  requiresMultipleColonies: boolean;
};

export const STRUCTURE_INFO: Record<StructureType, BlueprintInfo> = {
  nest: { label: 'Nest', desc: 'Shelter where colony members rest and recover energy. Boosts reproduction.', minIntel: 0.4, minBuild: 0.3, materialCost: 5, tier: 'basic', minColonySize: 3, minEvolutionLeaps: 0, requiresMultipleColonies: false },
  cache: { label: 'Food Cache', desc: 'Stockpiled food that colony members can draw from during scarcity.', minIntel: 0.45, minBuild: 0.35, materialCost: 3, tier: 'basic', minColonySize: 3, minEvolutionLeaps: 0, requiresMultipleColonies: false },
  watchtower: { label: 'Watchtower', desc: 'Extends the sense radius of nearby colony members, spotting predators earlier.', minIntel: 0.6, minBuild: 0.5, materialCost: 8, tier: 'basic', minColonySize: 4, minEvolutionLeaps: 0, requiresMultipleColonies: false },
  wall: { label: 'Wall', desc: 'Defensive barrier that slows predators entering the colony territory.', minIntel: 0.7, minBuild: 0.55, materialCost: 10, tier: 'basic', minColonySize: 5, minEvolutionLeaps: 0, requiresMultipleColonies: false },
  monument: { label: 'Monument', desc: 'A grand structure that radiates cultural energy, boosting intelligence and cooperation of all nearby colony members.', minIntel: 0.75, minBuild: 0.65, materialCost: 18, tier: 'advanced', minColonySize: 6, minEvolutionLeaps: 2, requiresMultipleColonies: false },
  sanctuary: { label: 'Sanctuary', desc: 'A protected haven where creatures heal faster and cannot be attacked. A beacon of peace in a hostile world.', minIntel: 0.8, minBuild: 0.7, materialCost: 15, tier: 'advanced', minColonySize: 5, minEvolutionLeaps: 2, requiresMultipleColonies: false },
  observatory: { label: 'Observatory', desc: 'A towering structure that reveals distant knowledge nodes and extends sense range across the entire colony. Requires two colonies to combine.', minIntel: 0.85, minBuild: 0.75, materialCost: 22, tier: 'advanced', minColonySize: 6, minEvolutionLeaps: 3, requiresMultipleColonies: true },
  bridge: { label: 'Bridge', desc: 'A pathway connecting two colonies, allowing members to travel between them and share resources. Requires two colonies to combine.', minIntel: 0.82, minBuild: 0.72, materialCost: 20, tier: 'advanced', minColonySize: 5, minEvolutionLeaps: 3, requiresMultipleColonies: true },
};

export const BLUEPRINT_ORDER: StructureType[] = ['monument', 'sanctuary', 'observatory', 'bridge'];

export type CivilizationLevel = { min: number; label: string; desc: string };

export const CIVILIZATION_LEVELS: readonly CivilizationLevel[] = [
  { min: 0, label: 'Nomadic', desc: 'No permanent structures. Wanders in search of food.' },
  { min: 1, label: 'Settled', desc: 'Builds basic nests. Beginning of a home territory.' },
  { min: 3, label: 'Organized', desc: 'Nests and food caches. Sustained communities.' },
  { min: 5, label: 'Advanced', desc: 'Watchtowers for early warning. Organized defense.' },
  { min: 8, label: 'Fortified', desc: 'Walls and watchtowers. Full civilization infrastructure.' },
];

export function civilizationLevel(structureCount: number): CivilizationLevel {
  let level = CIVILIZATION_LEVELS[0];
  for (const l of CIVILIZATION_LEVELS) {
    if (structureCount >= l.min) level = l;
  }
  return level;
}

export const BIOME_INFO: Record<BiomeType, { label: string; desc: string; hue: number; foodRate: number; energyDrain: number; speedMod: number; light: number; toxic: boolean }> = {
  plains: { label: 'Plains', desc: 'Open grassland. Balanced food growth and easy movement.', hue: 80, foodRate: 1.0, energyDrain: 1.0, speedMod: 1.0, light: 1.0, toxic: false },
  forest: { label: 'Forest', desc: 'Dense vegetation. Rich food but slower movement.', hue: 140, foodRate: 1.6, energyDrain: 0.9, speedMod: 0.8, light: 0.6, toxic: false },
  desert: { label: 'Desert', desc: 'Harsh and dry. Little food, high energy cost.', hue: 35, foodRate: 0.3, energyDrain: 1.6, speedMod: 1.1, light: 1.5, toxic: false },
  tundra: { label: 'Tundra', desc: 'Frozen ground. Scarce food, slow movement, high drain.', hue: 200, foodRate: 0.4, energyDrain: 1.5, speedMod: 0.7, light: 0.5, toxic: false },
  wetland: { label: 'Wetland', desc: 'Marshy terrain. Good food, moderate drain, slow movement.', hue: 180, foodRate: 1.3, energyDrain: 1.1, speedMod: 0.85, light: 0.7, toxic: false },
  ocean: { label: 'Ocean', desc: 'Vast open water. Abundant food, low light, moderate drain.', hue: 210, foodRate: 1.4, energyDrain: 0.8, speedMod: 0.9, light: 0.4, toxic: false },
  volcanic: { label: 'Volcanic', desc: 'Molten terrain. Extreme heat drains energy fast but rich minerals boost food.', hue: 15, foodRate: 1.2, energyDrain: 2.0, speedMod: 0.6, light: 1.2, toxic: false },
  toxic: { label: 'Toxic', desc: 'Poisoned land. Low food, high drain. Only resistant species survive.', hue: 85, foodRate: 0.5, energyDrain: 1.8, speedMod: 0.8, light: 0.8, toxic: true },
  crystal: { label: 'Crystal', desc: 'Luminous crystal fields. Moderate food, low drain, bright light for photosynthesis.', hue: 280, foodRate: 0.9, energyDrain: 0.7, speedMod: 1.0, light: 1.8, toxic: false },
};
