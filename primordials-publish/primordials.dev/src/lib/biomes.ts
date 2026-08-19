/**
 * Primordials — Biome metadata for landing page
 */

export interface Biome {
  id: string;
  name: string;
  tagline: string;
  description: string;
  color: string;
  textColor: string;
  emoji: string;
  species: number;
  population: string;
  heroImage: string;
}

export const biomes: Biome[] = [
  {
    id: 'algae-bloom',
    name: 'Algae Bloom',
    tagline: 'A teeming pond at dawn.',
    description: 'Green algae flourish, daphnia graze, copepods hunt — the default biome, peaceful and bright.',
    color: '#4ECDC4',
    textColor: '#0E2E2F',
    emoji: '🌿',
    species: 3,
    population: '~1,100',
    heroImage: '/biomes/algae-bloom--hero--A1.webp',
  },
  {
    id: 'deep-sea',
    name: 'Deep Sea',
    tagline: 'Two kilometers down.',
    description: 'Where only red light reaches — anglerfish lure, squid dart, krill drift in bioluminescent clouds.',
    color: '#FF6B6B',
    textColor: '#0A0908',
    emoji: '🐙',
    species: 4,
    population: '~280',
    heroImage: '/biomes/deep-sea--hero--D1.webp',
  },
  {
    id: 'primordial-soup',
    name: 'Primordial Soup',
    tagline: 'Four billion years ago.',
    description: 'Amino acids spark, protocells form — the chemistry of life, just beginning.',
    color: '#FFD93D',
    textColor: '#0E2E2F',
    emoji: '🍲',
    species: 3,
    population: '~600',
    heroImage: '/biomes/primordial-soup--hero--P1.webp',
  },
  {
    id: 'tundra',
    name: 'Tundra',
    tagline: 'Arctic summer boom-bust.',
    description: 'Lemmings swarm, snowy owls hunt — population cycles of brutal beauty.',
    color: '#6BCB77',
    textColor: '#0A0908',
    emoji: '❄️',
    species: 3,
    population: '~85',
    heroImage: '/biomes/tundra--hero--T1.webp',
  },
  {
    id: 'coral-reef',
    name: 'Coral Reef',
    tagline: 'A riot of tropical color.',
    description: 'Clownfish, anemones, parrotfish, and the patient moray — peak biodiversity.',
    color: '#F18F01',
    textColor: '#0A0908',
    emoji: '🪸',
    species: 4,
    population: '~440',
    heroImage: '/biomes/coral-reef--hero--C1.webp',
  },
  {
    id: 'volcanic-vent',
    name: 'Volcanic Vent',
    tagline: 'Where nothing should live.',
    description: 'Smoke, sulfur, and the extremophiles that thrive in the harshest place on Earth.',
    color: '#D62246',
    textColor: '#F4F1DE',
    emoji: '🌋',
    species: 3,
    population: '~320',
    heroImage: '/biomes/volcanic-vent--hero--V1.webp',
  },
];

export function getBiome(id: string): Biome | undefined {
  return biomes.find((b) => b.id === id);
}
