import { Rng } from './rng';
import {
  type Organism,
  type Food,
  type Species,
  type Colony,
  type Structure,
  type StructureType,
  type SimStats,
  type SimSettings,
  type Genome,
  type Biome,
  type BiomeType,
  type Remains,
  type KnowledgeNode,
  DEFAULT_SETTINGS,
  STRUCTURE_INFO,
  BIOME_INFO,
  BASIC_STRUCTURES,
  ADVANCED_STRUCTURES,
} from './types';
import {
  randomGenome,
  mutateGenome,
  crossoverGenome,
  geneticDistance,
  hadEvolutionLeap,
  SPECIATION_THRESHOLD,
} from './genetics';

const COLONY_MIN_MEMBERS = 3;
const BUILD_COOLDOWN = 200;
const MAX_STRUCTURES_PER_COLONY = 8;
const REMAINS_DECAY_TICKS = 300;
const KNOWLEDGE_NODES_COUNT = 8;
const MAX_CARRYING = 15;
const SANCTUARY_RADIUS = 60;
const OBSERVATORY_SENSE_BOOST = 2.0;
const MONUMENT_RADIUS = 70;
const BRIDGE_CONNECT_RANGE = 200;

export class Simulation {
  rng: Rng;
  settings: SimSettings;
  organisms: Organism[] = [];
  food: Food[] = [];
  species: Species[] = [];
  colonies: Colony[] = [];
  structures: Structure[] = [];
  biomes: Biome[] = [];
  remains: Remains[] = [];
  knowledgeNodes: KnowledgeNode[] = [];
  stats: SimStats;
  tick = 0;
  nextId = 1;
  nextFoodId = 1;
  nextSpeciesId = 1;
  nextColonyId = 1;
  nextStructureId = 1;
  nextRemainsId = 1;
  nextKnowledgeId = 1;

  constructor(seed: number, settings: SimSettings = DEFAULT_SETTINGS) {
    this.rng = new Rng(seed);
    this.settings = { ...settings };
    this.stats = this.makeStats();
    this.reset();
  }

  private makeStats(): SimStats {
    return {
      births: 0, deaths: 0, kills: 0, maxGeneration: 0,
      herbivores: 0, carnivores: 0, colonies: 0,
      sexualReproductions: 0, asexualReproductions: 0,
      avgIntelligence: 0, dominantSpeciesId: null, apexKills: 0,
      structures: 0, avgBuildSkill: 0,
      remains: 0, knowledgeDiscovered: 0, evolutionLeaps: 0,
      avgCuriosity: 0, avgAdaptability: 0,
    };
  }

  reset() {
    this.organisms = [];
    this.food = [];
    this.species = [];
    this.colonies = [];
    this.structures = [];
    this.remains = [];
    this.knowledgeNodes = [];
    this.biomes = [];
    this.tick = 0;
    this.nextId = 1;
    this.nextFoodId = 1;
    this.nextSpeciesId = 1;
    this.nextColonyId = 1;
    this.nextStructureId = 1;
    this.nextRemainsId = 1;
    this.nextKnowledgeId = 1;
    this.stats = this.makeStats();

    if (this.settings.biomes) this.generateBiomes();
    if (this.settings.knowledgeNodes) this.generateKnowledgeNodes();

    for (let i = 0; i < this.settings.initialPopulation; i++) {
      this.spawnOrganism(
        this.rng.range(0, this.settings.worldWidth),
        this.rng.range(0, this.settings.worldHeight),
        randomGenome(this.rng),
        0,
      );
    }
    for (let i = 0; i < this.settings.initialFood; i++) {
      this.spawnFood();
    }
  }

  private generateBiomes() {
    const types: BiomeType[] = ['plains', 'forest', 'desert', 'tundra', 'wetland'];
    const count = this.rng.int(4, 6);
    for (let i = 0; i < count; i++) {
      const type = types[i % types.length];
      const info = BIOME_INFO[type];
      this.biomes.push({
        id: i + 1,
        type,
        cx: this.rng.range(100, this.settings.worldWidth - 100),
        cy: this.rng.range(100, this.settings.worldHeight - 100),
        radius: this.rng.range(150, 280),
        foodRate: info.foodRate,
        energyDrain: info.energyDrain,
        speedMod: info.speedMod,
        hue: info.hue,
      });
    }
  }

  private generateKnowledgeNodes() {
    for (let i = 0; i < KNOWLEDGE_NODES_COUNT; i++) {
      this.knowledgeNodes.push({
        id: this.nextKnowledgeId++,
        x: this.rng.range(50, this.settings.worldWidth - 50),
        y: this.rng.range(50, this.settings.worldHeight - 50),
        intelBoost: this.rng.range(0.08, 0.2),
        discovered: false,
        discoveredBy: null,
        age: 0,
      });
    }
  }

  private spawnOrganism(x: number, y: number, genome: Genome, generation: number): Organism {
    const org: Organism = {
      id: this.nextId++,
      x, y, vx: 0, vy: 0,
      angle: this.rng.range(0, Math.PI * 2),
      energy: 80, age: 0, generation,
      genome,
      speciesId: this.assignSpecies(genome),
      alive: true,
      reproductionCooldown: 30,
      colonyId: null, colonyRole: 'solitary',
      threatLevel: 0, buildCooldown: 0,
      carrying: 0, knowledgeBoost: 0,
      biomeId: this.biomeAt(x, y)?.id ?? 0,
    };
    this.organisms.push(org);
    if (org.genome.diet >= 0.5) this.stats.carnivores++;
    else this.stats.herbivores++;
    if (generation > this.stats.maxGeneration) this.stats.maxGeneration = generation;
    return org;
  }

  private spawnFood() {
    let x = this.rng.range(0, this.settings.worldWidth);
    let y = this.rng.range(0, this.settings.worldHeight);
    // Bias food toward richer biomes
    if (this.settings.biomes && this.biomes.length > 0) {
      const attempts = 3;
      for (let a = 0; a < attempts; a++) {
        const biome = this.biomes[this.rng.int(0, this.biomes.length - 1)];
        if (biome.foodRate > 1) {
          x = biome.cx + this.rng.range(-biome.radius * 0.8, biome.radius * 0.8);
          y = biome.cy + this.rng.range(-biome.radius * 0.8, biome.radius * 0.8);
          x = Math.max(0, Math.min(this.settings.worldWidth, x));
          y = Math.max(0, Math.min(this.settings.worldHeight, y));
          break;
        }
      }
    }
    let energy = 35;
    const biome = this.biomeAt(x, y);
    if (biome) energy = Math.round(energy * biome.foodRate);
    this.food.push({ id: this.nextFoodId++, x, y, energy: Math.max(15, energy) });
  }

  private biomeAt(x: number, y: number): Biome | undefined {
    for (const b of this.biomes) {
      const dx = x - b.cx;
      const dy = y - b.cy;
      if (dx * dx + dy * dy < b.radius * b.radius) return b;
    }
    return undefined;
  }

  private assignSpecies(genome: Genome): number {
    let best: Species | null = null;
    let bestDist = Infinity;
    for (const sp of this.species) {
      const d = geneticDistance(genome, sp.representative);
      if (d < bestDist) { bestDist = d; best = sp; }
    }
    if (best && bestDist < SPECIATION_THRESHOLD) {
      best.count++;
      return best.id;
    }
    const sp: Species = {
      id: this.nextSpeciesId++, hue: genome.hue, count: 1,
      representative: { ...genome }, generations: 0,
      avgIntelligence: genome.intelligence, rank: 0, totalKills: 0,
      structuresBuilt: 0, civilizationLevel: 0,
      knowledgeDiscovered: 0, evolutionLeaps: 0,
    };
    this.species.push(sp);
    return sp.id;
  }

  step() {
    this.tick++;

    const foodToAdd = Math.min(this.settings.foodGrowthRate, this.settings.maxFood - this.food.length);
    for (let i = 0; i < foodToAdd; i++) this.spawnFood();

    for (const org of this.organisms) {
      if (!org.alive) continue;
      this.updateOrganism(org);
    }

    const newborns: Organism[] = [];
    if (this.settings.sexualReproduction) this.trySexualReproduction(newborns);
    for (const org of this.organisms) {
      if (!org.alive) continue;
      if (org.energy > this.settings.reproductionThreshold && org.reproductionCooldown <= 0
          && this.organisms.length + newborns.length < this.settings.maxPopulation) {
        newborns.push(this.reproduceAsexual(org));
      }
    }
    this.organisms.push(...newborns);

    this.organisms = this.organisms.filter((o) => o.alive);
    this.food = this.food.filter((f) => f.energy > 0);

    this.updateRemains();
    this.updateStructures();
    if (this.settings.knowledgeNodes) this.updateKnowledgeNodes();

    this.recountSpecies();
    this.updateHierarchy();

    if (this.settings.colonyFormation) {
      this.updateColonies();
    } else {
      for (const org of this.organisms) { org.colonyId = null; org.colonyRole = 'solitary'; }
      this.colonies = [];
      this.stats.colonies = 0;
    }

    if (this.settings.structureBuilding) {
      this.tryBuildStructures();
    }

    this.updateGlobalStats();
  }

  // ─── Intelligence-driven AI ───────────────────────────────────────────

  private updateOrganism(org: Organism) {
    org.age++;
    if (org.reproductionCooldown > 0) org.reproductionCooldown--;
    if (org.buildCooldown > 0) org.buildCooldown--;
    if (org.knowledgeBoost > 0) org.knowledgeBoost -= 0.001;

    // Track biome
    const biome = this.biomeAt(org.x, org.y);
    org.biomeId = biome?.id ?? 0;

    const intel = Math.min(1, org.genome.intelligence + org.knowledgeBoost);
    const sense = org.genome.senseRadius;
    const isCarnivore = org.genome.diet >= 0.5;

    let targetX: number | null = null;
    let targetY: number | null = null;
    let nearestDist = sense * sense;

    org.threatLevel = 0;

    // Watchtower bonus + Observatory bonus
    let effectiveSense = sense;
    if (org.colonyId !== null) {
      let hasObservatory = false;
      for (const s of this.structures) {
        if (s.colonyId !== org.colonyId) continue;
        if (s.type === 'watchtower') {
          const dx = s.x - org.x;
          const dy = s.y - org.y;
          if (dx * dx + dy * dy < s.radius * s.radius) {
            effectiveSense = Math.max(effectiveSense, sense * 1.5);
          }
        }
        if (s.type === 'observatory') hasObservatory = true;
      }
      if (hasObservatory) effectiveSense *= OBSERVATORY_SENSE_BOOST;
    }
    nearestDist = effectiveSense * effectiveSense;

    // Threat detection (intel >= 0.2)
    if (intel >= 0.2) {
      let threatX = 0, threatY = 0, threatFound = false;
      const fleeSense = effectiveSense * (0.6 + intel * 0.6);

      for (const other of this.organisms) {
        if (other === org || !other.alive) continue;
        if (other.genome.diet < 0.5 && other.genome.aggression < 0.6) continue;
        if (other.genome.size < org.genome.size * 0.9) continue;
        if (org.colonyId !== null && org.colonyId === other.colonyId) continue;

        const dx = other.x - org.x;
        const dy = other.y - org.y;
        const d2 = dx * dx + dy * dy;
        const fleeRange = fleeSense * fleeSense;

        if (d2 < fleeRange) {
          org.threatLevel = Math.max(org.threatLevel, 1 - d2 / fleeRange);
          threatX -= dx / (Math.sqrt(d2) + 1);
          threatY -= dy / (Math.sqrt(d2) + 1);
          threatFound = true;
        }
      }

      if (threatFound && (org.energy > 40 || intel >= 0.5)) {
        const fleeAngle = Math.atan2(threatY, threatX);
        const juke = intel >= 0.6 ? this.rng.range(-0.5, 0.5) : 0;
        org.angle = fleeAngle + juke;
        this.moveOrganism(org, biome);
        this.applyEnergyCost(org, biome);
        return;
      }
    }

    // Hunting
    if (isCarnivore || org.genome.aggression > 0.6) {
      let bestPreyScore = -Infinity;

      for (const other of this.organisms) {
        if (other === org || !other.alive) continue;
        if (org.colonyId !== null && org.colonyId === other.colonyId && org.genome.cooperation > 0.5) continue;
        if (other.genome.size > org.genome.size * 1.15) continue;

        // Sanctuary protection: skip prey inside a sanctuary
        if (other.colonyId !== null) {
          let protected_ = false;
          for (const s of this.structures) {
            if (s.type !== 'sanctuary' || s.colonyId !== other.colonyId) continue;
            const sdx = s.x - other.x;
            const sdy = s.y - other.y;
            if (sdx * sdx + sdy * sdy < s.radius * s.radius) { protected_ = true; break; }
          }
          if (protected_) continue;
        }

        const dx = other.x - org.x;
        const dy = other.y - org.y;
        const d2 = dx * dx + dy * dy;
        if (d2 >= nearestDist) continue;

        let score = -d2;
        if (intel >= 0.4) {
          score += (1 - other.energy / 200) * 500;
          if (other.threatLevel > 0.5) score += 300;
        }
        if (intel >= 0.6 && other.colonyId !== null) score -= 200;

        if (score > bestPreyScore) {
          bestPreyScore = score;
          targetX = other.x;
          targetY = other.y;
        }

        if (d2 < (org.genome.size + other.genome.size) * (org.genome.size + other.genome.size) * 0.25) {
          const gained = Math.min(other.energy, 60);
          org.energy += gained;
          other.alive = false;
          this.stats.kills++;
          this.stats.deaths++;
          if (other.genome.diet >= 0.5) this.stats.carnivores--;
          else this.stats.herbivores--;
          const sp = this.species.find((s) => s.id === org.speciesId);
          if (sp) sp.totalKills++;
          // Dead organism leaves remains
          this.spawnRemains(other.x, other.y, other.genome.size * 2, other.genome.hue);
        }
      }
    }

    // Foraging
    if (!isCarnivore || targetX === null) {
      let bestFoodScore = -Infinity;

      for (const f of this.food) {
        const dx = f.x - org.x;
        const dy = f.y - org.y;
        const d2 = dx * dx + dy * dy;
        if (d2 >= nearestDist && targetX === null) continue;

        let score = -d2;
        if (intel >= 0.4) {
          score += f.energy * 2;
          let clusterBonus = 0;
          for (const f2 of this.food) {
            if (f2 === f) continue;
            const fdx = f2.x - f.x;
            const fdy = f2.y - f.y;
            if (fdx * fdx + fdy * fdy < 1600) clusterBonus += 50;
          }
          score += clusterBonus;
        }

        if (score > bestFoodScore) {
          bestFoodScore = score;
          if (targetX === null) { targetX = f.x; targetY = f.y; }
        }

        if (d2 < (org.genome.size + 4) * (org.genome.size + 4)) {
          org.energy += f.energy;
          f.energy = 0;
        }
      }
    }

    // Gather remains as building material (intel >= 0.5, build skill >= 0.3)
    if (org.genome.intelligence >= 0.5 && org.genome.buildSkill >= 0.3 && org.carrying < MAX_CARRYING) {
      for (const r of this.remains) {
        const dx = r.x - org.x;
        const dy = r.y - org.y;
        const d2 = dx * dx + dy * dy;
        if (d2 < nearestDist && targetX === null) {
          targetX = r.x;
          targetY = r.y;
        }
        if (d2 < (org.genome.size + 5) * (org.genome.size + 5)) {
          const gathered = Math.min(r.energy, 3);
          org.carrying += gathered;
          r.energy -= gathered;
          break;
        }
      }
    }

    // Knowledge node discovery (curiosity-driven)
    if (this.settings.knowledgeNodes && org.genome.curiosity > 0.3) {
      for (const kn of this.knowledgeNodes) {
        if (kn.discovered) continue;
        const dx = kn.x - org.x;
        const dy = kn.y - org.y;
        const d2 = dx * dx + dy * dy;
        const curiosityRange = effectiveSense * (0.5 + org.genome.curiosity);
        if (d2 < curiosityRange * curiosityRange && targetX === null) {
          targetX = kn.x;
          targetY = kn.y;
        }
        if (d2 < (org.genome.size + 6) * (org.genome.size + 6)) {
          kn.discovered = true;
          kn.discoveredBy = org.speciesId;
          org.knowledgeBoost += kn.intelBoost;
          this.stats.knowledgeDiscovered++;
          const sp = this.species.find((s) => s.id === org.speciesId);
          if (sp) sp.knowledgeDiscovered++;
        }
      }
    }

    // Sanctuary protection: cannot be attacked while inside
    if (org.colonyId !== null) {
      for (const s of this.structures) {
        if (s.type !== 'sanctuary' || s.colonyId !== org.colonyId) continue;
        const dx = s.x - org.x;
        const dy = s.y - org.y;
        if (dx * dx + dy * dy < s.radius * s.radius) {
          org.energy += 0.8;
          org.threatLevel = 0;
          break;
        }
      }
    }

    // Monument boost: intelligence and cooperation aura
    if (org.colonyId !== null) {
      for (const s of this.structures) {
        if (s.type !== 'monument' || s.colonyId !== org.colonyId) continue;
        const dx = s.x - org.x;
        const dy = s.y - org.y;
        if (dx * dx + dy * dy < s.radius * s.radius) {
          org.knowledgeBoost = Math.max(org.knowledgeBoost, 0.03);
          break;
        }
      }
    }

    // Nest rest
    if (org.colonyId !== null && org.energy < 100) {
      for (const s of this.structures) {
        if (s.type !== 'nest' || s.colonyId !== org.colonyId) continue;
        const dx = s.x - org.x;
        const dy = s.y - org.y;
        if (dx * dx + dy * dy < s.radius * s.radius) {
          org.energy += 0.5;
          if (targetX === null) { targetX = s.x; targetY = s.y; }
          break;
        }
      }
    }

    // Food cache
    if (org.colonyId !== null && org.energy < 50) {
      for (const s of this.structures) {
        if (s.type !== 'cache' || s.colonyId !== org.colonyId) continue;
        const dx = s.x - org.x;
        const dy = s.y - org.y;
        if (dx * dx + dy * dy < s.radius * s.radius && s.hp > 0) {
          org.energy += 20;
          s.hp -= 1;
          break;
        }
      }
    }

    // Colony cohesion
    if (org.colonyId !== null && org.colonyRole === 'member' && org.genome.cooperation > 0.5) {
      const colony = this.colonies.find((c) => c.id === org.colonyId);
      if (colony && targetX === null) { targetX = colony.centerX; targetY = colony.centerY; }
    }

    // Movement
    if (targetX !== null && targetY !== null) {
      const dx = (targetX as number) - org.x;
      const dy = (targetY as number) - org.y;
      org.angle = Math.atan2(dy, dx);
    } else {
      const wander = 0.3 - intel * 0.2;
      org.angle += this.rng.range(-wander, wander);
    }

    this.moveOrganism(org, biome);
    this.applyEnergyCost(org, biome);

    // Death from old age / starvation leaves remains
    if (!org.alive && org.energy <= 0) {
      this.spawnRemains(org.x, org.y, org.genome.size * 1.5, org.genome.hue);
    }
  }

  private moveOrganism(org: Organism, biome: Biome | undefined) {
    const speedMod = biome ? biome.speedMod : 1;
    const speed = org.genome.speed * speedMod;

    if (org.genome.diet >= 0.5 || org.genome.aggression > 0.6) {
      for (const s of this.structures) {
        if (s.type !== 'wall') continue;
        const dx = s.x - org.x;
        const dy = s.y - org.y;
        if (dx * dx + dy * dy < s.radius * s.radius) {
          org.vx = Math.cos(org.angle) * speed * 0.4;
          org.vy = Math.sin(org.angle) * speed * 0.4;
          org.x += org.vx;
          org.y += org.vy;
          this.wrap(org);
          return;
        }
      }
    }
    org.vx = Math.cos(org.angle) * speed;
    org.vy = Math.sin(org.angle) * speed;
    org.x += org.vx;
    org.y += org.vy;
    this.wrap(org);
  }

  private wrap(org: Organism) {
    const w = this.settings.worldWidth;
    const h = this.settings.worldHeight;
    const mode = this.settings.boundaryMode;
    if (mode === 'wrap') {
      if (org.x < 0) org.x += w;
      if (org.x >= w) org.x -= w;
      if (org.y < 0) org.y += h;
      if (org.y >= h) org.y -= h;
    } else if (mode === 'reflect') {
      if (org.x < 0) { org.x = -org.x; org.angle = Math.PI - org.angle; }
      if (org.x >= w) { org.x = 2 * w - org.x; org.angle = Math.PI - org.angle; }
      if (org.y < 0) { org.y = -org.y; org.angle = -org.angle; }
      if (org.y >= h) { org.y = 2 * h - org.y; org.angle = -org.angle; }
    } else {
      // open: clamp to bounds with a small bounce
      if (org.x < 0) { org.x = 0; org.angle = Math.PI - org.angle; }
      if (org.x >= w) { org.x = w - 1; org.angle = Math.PI - org.angle; }
      if (org.y < 0) { org.y = 0; org.angle = -org.angle; }
      if (org.y >= h) { org.y = h - 1; org.angle = -org.angle; }
    }
  }

  private applyEnergyCost(org: Organism, biome: Biome | undefined) {
    let cost =
      (org.genome.size * 0.01 + org.genome.speed * 0.04 + org.genome.senseRadius * 0.002) *
      org.genome.metabolism;

    // Biome energy drain, mitigated by adaptability
    if (biome) {
      const drainPenalty = Math.max(0, biome.energyDrain - 1);
      cost *= 1 + drainPenalty * (1 - org.genome.adaptability);
    }

    org.energy -= cost;

    const maxAge = org.genome.lifespan;
    if (org.age > maxAge) org.energy -= (org.age - maxAge) * 0.02;

    if (org.energy <= 0) {
      org.alive = false;
      this.stats.deaths++;
      if (org.genome.diet >= 0.5) this.stats.carnivores--;
      else this.stats.herbivores--;
    }
  }

  // ─── Remains (dead organic matter) ──────────────────────────────────

  private spawnRemains(x: number, y: number, energy: number, hue: number) {
    this.remains.push({
      id: this.nextRemainsId++,
      x, y, energy, age: 0, hue,
    });
  }

  private updateRemains() {
    for (const r of this.remains) {
      r.age++;
      // Remains decay into food after some time (nutrient recycling)
      if (r.age > REMAINS_DECAY_TICKS && r.energy > 5) {
        this.food.push({
          id: this.nextFoodId++,
          x: r.x + this.rng.range(-5, 5),
          y: r.y + this.rng.range(-5, 5),
          energy: Math.min(30, r.energy * 0.5),
        });
        r.energy = 0;
      }
    }
    this.remains = this.remains.filter((r) => r.energy > 0);
    this.stats.remains = this.remains.length;
  }

  // ─── Knowledge nodes ────────────────────────────────────────────────

  private updateKnowledgeNodes() {
    for (const kn of this.knowledgeNodes) {
      kn.age++;
    }
  }

  // ─── Structure building ──────────────────────────────────────────────

  private tryBuildStructures() {
    for (const org of this.organisms) {
      if (!org.alive || org.buildCooldown > 0) continue;
      if (org.colonyId === null || org.colonyRole !== 'leader') continue;

      const intel = org.genome.intelligence;
      const build = org.genome.buildSkill;
      if (intel < 0.4 || build < 0.3) continue;

      const colony = this.colonies.find((c) => c.id === org.colonyId);
      if (!colony) continue;
      const colonyStructures = this.structures.filter((s) => s.colonyId === org.colonyId);
      if (colonyStructures.length >= MAX_STRUCTURES_PER_COLONY) continue;
      if (org.energy < 60) continue;

      const sp = this.species.find((s) => s.id === org.speciesId);
      const has = new Set(colonyStructures.map((s) => s.type));

      // Try basic structures first
      let toBuild: StructureType | null = null;
      for (const t of BASIC_STRUCTURES) {
        if (!has.has(t)) {
          const info = STRUCTURE_INFO[t];
          if (intel >= info.minIntel && build >= info.minBuild && colony.members >= info.minColonySize) {
            if (org.carrying >= info.materialCost || t === 'nest') {
              toBuild = t;
              break;
            }
          }
        }
      }

      // Try advanced blueprints if enabled
      if (toBuild === null && this.settings.blueprints) {
        for (const t of ADVANCED_STRUCTURES) {
          if (has.has(t)) continue;
          const info = STRUCTURE_INFO[t];
          if (intel < info.minIntel || build < info.minBuild) continue;
          if (colony.members < info.minColonySize) continue;
          if (sp && sp.evolutionLeaps < info.minEvolutionLeaps) continue;
          if (org.carrying < info.materialCost) continue;

          if (info.requiresMultipleColonies) {
            // Check for a nearby allied colony of the same species
            const allied = this.colonies.find((c) =>
              c.id !== org.colonyId && c.speciesId === org.speciesId &&
              Math.hypot(c.centerX - colony.centerX, c.centerY - colony.centerY) < BRIDGE_CONNECT_RANGE,
            );
            if (!allied) continue;
            // Build the blueprint between the two colonies
            this.buildBlueprint(t, org, colony, [colony.id, allied.id]);
            continue;
          }

          toBuild = t;
          break;
        }
      }

      if (toBuild !== null) {
        this.buildBlueprint(toBuild, org, colony, [colony.id]);
      }
    }
  }

  private buildBlueprint(type: StructureType, org: Organism, colony: Colony, contributing: number[]) {
    const info = STRUCTURE_INFO[type];
    let x = colony.centerX + this.rng.range(-30, 30);
    let y = colony.centerY + this.rng.range(-30, 30);

    // For multi-colony blueprints, place between the two colonies
    if (contributing.length > 1) {
      const other = this.colonies.find((c) => c.id === contributing[1]);
      if (other) {
        x = (colony.centerX + other.centerX) / 2;
        y = (colony.centerY + other.centerY) / 2;
      }
    }

    const radius = type === 'wall' ? 50 : type === 'monument' ? MONUMENT_RADIUS : type === 'sanctuary' ? SANCTUARY_RADIUS : 45;
    const struct: Structure = {
      id: this.nextStructureId++,
      type,
      x, y,
      speciesId: org.speciesId,
      colonyId: org.colonyId,
      hp: type === 'cache' ? 20 : 150,
      maxHp: type === 'cache' ? 20 : 150,
      radius,
      hue: org.genome.hue,
      age: 0,
      materials: type === 'nest' ? 0 : info.materialCost,
      tier: info.tier,
      contributingColonies: contributing,
    };
    this.structures.push(struct);

    if (type === 'nest' && colony) colony.nestId = struct.id;

    org.energy -= 40;
    org.buildCooldown = BUILD_COOLDOWN;
    if (type !== 'nest') org.carrying -= info.materialCost;

    const sp = this.species.find((s) => s.id === org.speciesId);
    if (sp) sp.structuresBuilt++;
  }

  private updateStructures() {
    for (const s of this.structures) {
      s.age++;
      if (s.type === 'wall' || s.type === 'watchtower') {
        s.hp -= 0.02;
      }
      // Advanced structures are more durable
      if (s.tier === 'advanced') {
        s.hp -= 0.005;
      }
    }
    const before = this.structures.length;
    this.structures = this.structures.filter((s) => s.hp > 0);
    if (this.structures.length !== before) {
      for (const c of this.colonies) {
        if (c.nestId !== null && !this.structures.find((s) => s.id === c.nestId)) {
          c.nestId = null;
        }
      }
    }
  }

  // ─── Reproduction ────────────────────────────────────────────────────

  private trySexualReproduction(newborns: Organism[]) {
    const candidates = this.organisms.filter(
      (o) => o.alive && o.energy > this.settings.reproductionThreshold * 0.7
        && o.reproductionCooldown <= 0 && o.genome.fertility > 0.3,
    );

    for (const a of candidates) {
      if (a.reproductionCooldown > 0) continue;
      if (this.organisms.length + newborns.length >= this.settings.maxPopulation) break;

      let bestMate: Organism | null = null;
      let bestDist = Infinity;
      for (const b of this.organisms) {
        if (b === a || !b.alive || b.reproductionCooldown > 0) continue;
        if (b.speciesId !== a.speciesId) continue;
        if (b.energy < this.settings.reproductionThreshold * 0.5) continue;
        const dx = b.x - a.x;
        const dy = b.y - a.y;
        const d2 = dx * dx + dy * dy;
        const mateRange = (a.genome.senseRadius * 0.5) ** 2;
        if (d2 < mateRange && d2 < bestDist) { bestDist = d2; bestMate = b; }
      }

      if (bestMate) {
        let nestBonus = 1;
        if (a.colonyId !== null) {
          for (const s of this.structures) {
            if (s.type !== 'nest' || s.colonyId !== a.colonyId) continue;
            const dx = s.x - a.x;
            const dy = s.y - a.y;
            if (dx * dx + dy * dy < s.radius * s.radius) { nestBonus = 0.6; break; }
          }
        }
        const child = this.reproduceSexual(a, bestMate, nestBonus);
        if (child) newborns.push(child);
      }
    }
  }

  private reproduceAsexual(parent: Organism): Organism {
    parent.energy /= 2;
    parent.reproductionCooldown = 60;
    const childGenome = mutateGenome(parent.genome, this.settings.mutationRate, this.rng);
    if (hadEvolutionLeap(parent.genome, childGenome)) {
      this.stats.evolutionLeaps++;
      const sp = this.species.find((s) => s.id === parent.speciesId);
      if (sp) sp.evolutionLeaps++;
    }
    const child: Organism = {
      id: this.nextId,
      x: parent.x + this.rng.range(-10, 10),
      y: parent.y + this.rng.range(-10, 10),
      vx: 0, vy: 0,
      angle: this.rng.range(0, Math.PI * 2),
      energy: parent.energy, age: 0,
      generation: parent.generation + 1,
      genome: childGenome,
      speciesId: this.assignSpecies(childGenome),
      alive: true, reproductionCooldown: 30,
      colonyId: null, colonyRole: 'solitary',
      threatLevel: 0, buildCooldown: 0,
      carrying: 0, knowledgeBoost: 0,
      biomeId: parent.biomeId,
    };
    this.stats.births++;
    this.stats.asexualReproductions++;
    return child;
  }

  private reproduceSexual(parentA: Organism, parentB: Organism, nestBonus: number): Organism | null {
    if (this.organisms.length >= this.settings.maxPopulation) return null;

    const cost = 40 * nestBonus * (1 / Math.max(0.2, (parentA.genome.fertility + parentB.genome.fertility) / 2));
    parentA.energy -= cost;
    parentB.energy -= cost;
    parentA.reproductionCooldown = 60;
    parentB.reproductionCooldown = 60;

    let childGenome = crossoverGenome(parentA.genome, parentB.genome, this.rng);
    const parentAvg: Genome = {
      ...parentA.genome,
      intelligence: (parentA.genome.intelligence + parentB.genome.intelligence) / 2,
      buildSkill: (parentA.genome.buildSkill + parentB.genome.buildSkill) / 2,
    };
    childGenome = mutateGenome(childGenome, this.settings.mutationRate, this.rng);
    if (hadEvolutionLeap(parentAvg, childGenome)) {
      this.stats.evolutionLeaps++;
      const sp = this.species.find((s) => s.id === parentA.speciesId);
      if (sp) sp.evolutionLeaps++;
    }

    const midX = (parentA.x + parentB.x) / 2;
    const midY = (parentA.y + parentB.y) / 2;
    const child: Organism = {
      id: this.nextId++,
      x: midX + this.rng.range(-10, 10),
      y: midY + this.rng.range(-10, 10),
      vx: 0, vy: 0,
      angle: this.rng.range(0, Math.PI * 2),
      energy: 60, age: 0,
      generation: Math.max(parentA.generation, parentB.generation) + 1,
      genome: childGenome,
      speciesId: this.assignSpecies(childGenome),
      alive: true, reproductionCooldown: 30,
      colonyId: null, colonyRole: 'solitary',
      threatLevel: 0, buildCooldown: 0,
      carrying: 0, knowledgeBoost: 0,
      biomeId: this.biomeAt(midX, midY)?.id ?? 0,
    };
    this.stats.births++;
    this.stats.sexualReproductions++;
    return child;
  }

  // ─── Colony formation ────────────────────────────────────────────────

  private updateColonies() {
    for (const org of this.organisms) { org.colonyId = null; org.colonyRole = 'solitary'; }
    this.colonies = [];

    const radius = this.settings.colonyRadius;
    const r2 = radius * radius;

    const visited = new Set<number>();
    for (const org of this.organisms) {
      if (!org.alive || visited.has(org.id)) continue;
      const colonyTendency = (org.genome.socialGene + org.genome.intelligence * 0.3) * org.genome.cooperation;
      if (colonyTendency < 0.3) { visited.add(org.id); continue; }

      const group: Organism[] = [org];
      visited.add(org.id);

      let changed = true;
      while (changed) {
        changed = false;
        for (const member of group) {
          for (const other of this.organisms) {
            if (visited.has(other.id) || !other.alive) continue;
            if (other.speciesId !== org.speciesId) continue;
            if (other.genome.socialGene < 0.25) continue;
            const dx = other.x - member.x;
            const dy = other.y - member.y;
            if (dx * dx + dy * dy < r2) {
              group.push(other);
              visited.add(other.id);
              changed = true;
            }
          }
        }
      }

      if (group.length >= COLONY_MIN_MEMBERS) {
        const colonyId = this.nextColonyId++;
        let cx = 0, cy = 0;
        for (const m of group) { cx += m.x; cy += m.y; }
        cx /= group.length; cy /= group.length;

        let leader = group[0];
        for (const m of group) {
          const ls = leader.genome.intelligence + leader.genome.cooperation * 0.3;
          const ms = m.genome.intelligence + m.genome.cooperation * 0.3;
          if (ms > ls) leader = m;
        }

        let nestId: number | null = null;
        for (const s of this.structures) {
          if (s.type === 'nest' && s.colonyId === colonyId) { nestId = s.id; break; }
        }

        for (const m of group) {
          m.colonyId = colonyId;
          m.colonyRole = m === leader ? 'leader' : 'member';
        }

        this.colonies.push({
          id: colonyId, speciesId: org.speciesId, members: group.length,
          centerX: cx, centerY: cy, hue: org.genome.hue, nestId,
        });
      }
    }
    this.stats.colonies = this.colonies.length;
  }

  // ─── Species hierarchy & stats ────────────────────────────────────────

  private recountSpecies() {
    for (const sp of this.species) sp.count = 0;
    for (const org of this.organisms) {
      const sp = this.species.find((s) => s.id === org.speciesId);
      if (sp) sp.count++;
    }
    this.species = this.species.filter((s) => s.count > 0);
  }

  private updateHierarchy() {
    for (const sp of this.species) {
      let totalIntel = 0, totalSize = 0, count = 0;
      for (const org of this.organisms) {
        if (org.speciesId === sp.id) {
          totalIntel += org.genome.intelligence;
          totalSize += org.genome.size;
          count++;
        }
      }
      sp.avgIntelligence = count > 0 ? totalIntel / count : 0;
      const avgSize = count > 0 ? totalSize / count : 0;

      const structCount = this.structures.filter((s) => s.speciesId === sp.id).length;
      sp.civilizationLevel = structCount;

      sp.generations = count;
      (sp as Species & { _domScore: number })._domScore =
        sp.count * 1.0 + sp.totalKills * 0.5 + sp.avgIntelligence * 10 + avgSize * 0.5
        + structCount * 3 + sp.knowledgeDiscovered * 4 + sp.evolutionLeaps * 2;
    }

    const sorted = [...this.species].sort(
      (a, b) => (b as Species & { _domScore: number })._domScore - (a as Species & { _domScore: number })._domScore,
    );
    sorted.forEach((sp, i) => { sp.rank = i + 1; });
  }

  private updateGlobalStats() {
    if (this.organisms.length === 0) {
      this.stats.avgIntelligence = 0;
      this.stats.dominantSpeciesId = null;
      this.stats.apexKills = 0;
      this.stats.structures = 0;
      this.stats.avgBuildSkill = 0;
      this.stats.avgCuriosity = 0;
      this.stats.avgAdaptability = 0;
      return;
    }
    let totalIntel = 0, totalBuild = 0, totalCurio = 0, totalAdapt = 0;
    for (const org of this.organisms) {
      totalIntel += org.genome.intelligence;
      totalBuild += org.genome.buildSkill;
      totalCurio += org.genome.curiosity;
      totalAdapt += org.genome.adaptability;
    }
    const n = this.organisms.length;
    this.stats.avgIntelligence = totalIntel / n;
    this.stats.avgBuildSkill = totalBuild / n;
    this.stats.avgCuriosity = totalCurio / n;
    this.stats.avgAdaptability = totalAdapt / n;
    this.stats.structures = this.structures.length;
    this.stats.remains = this.remains.length;

    const dominant = [...this.species].sort((a, b) => a.rank - b.rank)[0] ?? null;
    this.stats.dominantSpeciesId = dominant?.id ?? null;
    this.stats.apexKills = dominant?.totalKills ?? 0;
  }

  get population() { return this.organisms.length; }
  get speciesCount() { return this.species.length; }
  get generations() { return this.stats.maxGeneration; }

  seedLife(count: number) {
    for (let i = 0; i < count; i++) {
      const genome = randomGenome(this.rng);
      const org: Organism = {
        id: this.nextId++,
        x: this.rng.range(0, this.settings.worldWidth),
        y: this.rng.range(0, this.settings.worldHeight),
        vx: 0, vy: 0,
        angle: this.rng.range(0, Math.PI * 2),
        energy: 80, age: 0, generation: 0,
        genome,
        speciesId: this.assignSpecies(genome),
        alive: true, reproductionCooldown: 30,
        colonyId: null, colonyRole: 'solitary',
        threatLevel: 0, buildCooldown: 0,
        carrying: 0, knowledgeBoost: 0,
        biomeId: 0,
      };
      this.organisms.push(org);
      if (org.genome.diet >= 0.5) this.stats.carnivores++;
      else this.stats.herbivores++;
    }
  }
}
