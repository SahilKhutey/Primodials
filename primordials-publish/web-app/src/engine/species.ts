import type { SpeciesConfig, SpeciesType, ShapeType } from '@/types/biome';

export function createDefaultSpecies(
  id: string,
  name: string,
  type: SpeciesType,
  color: string,
  shape: ShapeType = 'circle'
): SpeciesConfig {
  return {
    id,
    name,
    type,
    color,
    shape,
    size: type === 'producer' ? 4 : type === 'grazer' ? 6 : 8,
    initial: type === 'producer' ? 300 : type === 'grazer' ? 100 : 15,
    behavior: {
      movement: {
        type: type === 'producer' ? 'wander' : type === 'grazer' ? 'flee' : 'seek',
        speed: type === 'producer' ? 1.0 : type === 'grazer' ? 10.0 : 20.0,
        turnRate: 2.0,
      },
      feeding: {
        diet: type === 'predator' ? ['grazer'] : type === 'grazer' ? ['producer'] : [],
        consumptionRate: 0.1,
        hungerRate: type === 'producer' ? 0 : 0.02,
        maxHunger: 1.0,
        huntSuccess: type === 'predator' ? 0.4 : 1.0,
      },
      reproduction: {
        rate: type === 'producer' ? 0.05 : type === 'grazer' ? 0.03 : 0.015,
        threshold: 0.7,
        offspring: type === 'producer' ? 1 : 2,
        maturityAge: 20,
      },
    },
    stats: {
      maxEnergy: 1.0,
      metabolism: type === 'producer' ? 0.005 : 0.015,
      lifespan: 300,
      vision: type === 'predator' ? 100 : 60,
    },
  };
}
