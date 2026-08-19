import type { Biome } from '@/types/biome';
import { algaeBloom } from './algaeBloom';
import { deepSea } from './deepSea';
import { primordialSoup } from './primordialSoup';
import { tundra } from './tundra';
import { coralReef } from './coralReef';
import { volcanicVent } from './volcanicVent';

export const biomes: Biome[] = [
  algaeBloom,
  deepSea,
  primordialSoup,
  tundra,
  coralReef,
  volcanicVent,
];

export function getBiome(id: string): Biome | undefined {
  return biomes.find((b) => b.id === id);
}

export {
  algaeBloom,
  deepSea,
  primordialSoup,
  tundra,
  coralReef,
  volcanicVent,
};
