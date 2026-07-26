import { createClient } from '@supabase/supabase-js';

const supabaseUrl = import.meta.env.VITE_SUPABASE_URL;
const supabaseAnonKey = import.meta.env.VITE_SUPABASE_ANON_KEY;

export const supabase = createClient(supabaseUrl, supabaseAnonKey);

export type SnapshotRow = {
  id: string;
  name: string;
  tick: number;
  population: number;
  species_count: number;
  generations: number;
  snapshot: SimSnapshot;
  created_at: string;
};

export type SimSnapshot = {
  tick: number;
  population: number;
  speciesCount: number;
  generations: number;
  organisms: SerializedOrganism[];
  species: SerializedSpecies[];
  colonies: SerializedColony[];
  structures: SerializedStructure[];
  biomes: SerializedBiome[];
  remains: SerializedRemains[];
  knowledgeNodes: SerializedKnowledgeNode[];
  stats: SimStats;
  settings: SimSettings;
};

export type SerializedBiome = {
  id: number;
  type: string;
  cx: number;
  cy: number;
  radius: number;
  foodRate: number;
  energyDrain: number;
  speedMod: number;
  hue: number;
};

export type SerializedRemains = {
  id: number;
  x: number;
  y: number;
  energy: number;
  age: number;
  hue: number;
};

export type SerializedKnowledgeNode = {
  id: number;
  x: number;
  y: number;
  intelBoost: number;
  discovered: boolean;
  discoveredBy: number | null;
  age: number;
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
  boundaryMode: 'wrap' | 'reflect' | 'open';
  blueprints: boolean;
};

export type SerializedOrganism = {
  id: number;
  x: number;
  y: number;
  angle: number;
  energy: number;
  age: number;
  generation: number;
  genome: Genome;
  speciesId: number;
  colonyId: number | null;
  colonyRole: 'leader' | 'member' | 'solitary';
  threatLevel: number;
  buildCooldown: number;
  carrying: number;
  knowledgeBoost: number;
  biomeId: number;
};

export type SerializedSpecies = {
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

export type SerializedColony = {
  id: number;
  speciesId: number;
  members: number;
  centerX: number;
  centerY: number;
  hue: number;
  nestId: number | null;
};

export type SerializedStructure = {
  id: number;
  type: string;
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
  tier: 'basic' | 'advanced';
  contributingColonies: number[];
};

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
  adaptability: number;
  curiosity: number;
};
