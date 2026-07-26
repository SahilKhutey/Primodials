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

export type BiomeType = 'plains' | 'forest' | 'desert' | 'tundra' | 'wetland';

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
};

export type BoundaryMode = 'wrap' | 'reflect' | 'open';

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
};

export type IntelligenceTier = { min: number; label: string; desc: string };

export const INTELLIGENCE_TIERS: readonly IntelligenceTier[] = [
  { min: 0.0, label: 'Instinctual', desc: 'Reacts only to immediate stimuli. No memory or planning.' },
  { min: 0.2, label: 'Reactive', desc: 'Basic threat detection. Flees from larger predators.' },
  { min: 0.4, label: 'Adaptive', desc: 'Prefers richer food sources. Avoids dangerous areas. Begins building.' },
  { min: 0.6, label: 'Strategic', desc: 'Hunts in packs. Flanks prey. Prioritizes weak targets. Gathers materials.' },
  { min: 0.8, label: 'Tactical', desc: 'Coordinated pack tactics. Defends colony intelligently. Seeks knowledge.' },
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

export const BIOME_INFO: Record<BiomeType, { label: string; desc: string; hue: number; foodRate: number; energyDrain: number; speedMod: number }> = {
  plains: { label: 'Plains', desc: 'Open grassland. Balanced food growth and easy movement.', hue: 80, foodRate: 1.0, energyDrain: 1.0, speedMod: 1.0 },
  forest: { label: 'Forest', desc: 'Dense vegetation. Rich food but slower movement.', hue: 140, foodRate: 1.6, energyDrain: 0.9, speedMod: 0.8 },
  desert: { label: 'Desert', desc: 'Harsh and dry. Little food, high energy cost.', hue: 35, foodRate: 0.3, energyDrain: 1.6, speedMod: 1.1 },
  tundra: { label: 'Tundra', desc: 'Frozen ground. Scarce food, slow movement, high drain.', hue: 200, foodRate: 0.4, energyDrain: 1.5, speedMod: 0.7 },
  wetland: { label: 'Wetland', desc: 'Marshy terrain. Good food, moderate drain, slow movement.', hue: 180, foodRate: 1.3, energyDrain: 1.1, speedMod: 0.85 },
};
