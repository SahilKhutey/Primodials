import { createClient, type SupabaseClient } from '@supabase/supabase-js';

const supabaseUrl = import.meta.env.VITE_SUPABASE_URL;
const supabaseAnonKey = import.meta.env.VITE_SUPABASE_ANON_KEY;

function isValidUrl(url?: string): boolean {
  if (!url || typeof url !== 'string' || !url.trim()) return false;
  if (url.includes('your-project-ref') || url.includes('YOUR_SUPABASE_URL')) return false;
  try {
    const parsed = new URL(url);
    return parsed.protocol === 'http:' || parsed.protocol === 'https:';
  } catch {
    return false;
  }
}

function isValidKey(key?: string): boolean {
  if (!key || typeof key !== 'string' || !key.trim()) return false;
  if (key.includes('your-anon-key') || key.includes('YOUR_SUPABASE_ANON_KEY')) return false;
  return key.length > 20;
}

// Cloud history/checkpoints are optional. If no valid Supabase project is
// configured, the sim still runs fully offline — history saving/loading
// is simply disabled instead of crashing the app on boot.
export const supabaseEnabled = isValidUrl(supabaseUrl) && isValidKey(supabaseAnonKey);

export const supabase: SupabaseClient | null = supabaseEnabled
  ? createClient(supabaseUrl!, supabaseAnonKey!)
  : null;

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
  generationCycle: number;
  worldExpansions: number;
  brainDrivenPopulation: number;
  avgFitness: number;
  peakFitness: number;
  biofilmClusters: number;
  biofilmPopulation: number;
  spores: number;
  hgtEvents: number;
  avgChemotaxis: number;
  avgQuorum: number;
  avgBiofilm: number;
  photosynthesizers: number;
  symbioticPairs: number;
  parasiticEvents: number;
  infected: number;
  diseaseEvents: number;
  avgPhotosynthesis: number;
  avgSymbiosis: number;
  avgToxicity: number;
  newSpeciesIntroduced: number;
  spatialClusters: number;
  clusteredPopulation: number;
  alphaCount: number;
  altruismEvents: number;
  avgClustering: number;
  avgAltruism: number;
  avgDominance: number;
  avgNicheBreadth: number;
  avgCompetitiveness: number;
  competitionEvents: number;
  neutralDriftEvents: number;
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
  neuralBrains: boolean;
  generationCycleTicks: number;
  worldExpansion: boolean;
  worldExpansionInterval: number;
  autoCheckpoint: boolean;
  chemicalField: boolean;
  microbialBehavior: boolean;
  endlessGeneration: boolean;
  diseaseEvents: boolean;
  advancedBiology: boolean;
  socialBehavior: boolean;
  neutralDrift: boolean;
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
  brain: SerializedBrain | null;
  fitness: number;
  tumbleTimer: number;
  inBiofilm: boolean;
  biofilmId: number | null;
  sporeMode: boolean;
  sporeTimer: number;
  infected: boolean;
  infectionTimer: number;
  symbiosisPartner: number | null;
  socialRank: string;
  clusterId: number | null;
  hibernating: boolean;
  sonarPulse: number;
  leapTimer: number;
  speciationTimer: number;
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

export type SerializedBrain = {
  nInputs: number;
  nHidden: number;
  nOutputs: number;
  weights: number[];
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
  chemotaxis: number;
  quorumSensing: number;
  biofilmFormation: number;
  sporulation: number;
  hgtRate: number;
  photosynthesis: number;
  symbiosis: number;
  parasitism: number;
  diseaseResistance: number;
  agingRate: number;
  toxicity: number;
  clustering: number;
  altruism: number;
  dominance: number;
  nicheBreadth: number;
  competitiveness: number;
  camouflage: number;
  bioluminescence: number;
  echolocation: number;
  hibernation: number;
};
