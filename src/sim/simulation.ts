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
  type HistoryPoint,
  type Particle,
  type ParticleType,
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
  makeBrainForGenome,
  mutateBrainForGenome,
  crossoverBrainForGenome,
  SPECIATION_THRESHOLD,
} from './genetics';
import { evalBrain, N_INPUTS, N_OUTPUTS, type Brain } from './brain';
import { ChemicalField } from './chemicalField';
import {
  runAndTumble, quorumSense, updateBiofilms, trySporulation, tryHGT, applyHGT,
  type BiofilmCluster,
} from './microbial';

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
  particles: Particle[] = [];
  chemicalField: ChemicalField | null = null;
  biofilms: BiofilmCluster[] = [];
  nextBiofilmId = 1;
  stats: SimStats;
  tick = 0;
  history: HistoryPoint[] = [];
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
      generationCycle: 0, worldExpansions: 0,
      brainDrivenPopulation: 0, avgFitness: 0, peakFitness: 0,
      biofilmClusters: 0, biofilmPopulation: 0, spores: 0,
      hgtEvents: 0, avgChemotaxis: 0, avgQuorum: 0, avgBiofilm: 0,
      photosynthesizers: 0, symbioticPairs: 0, parasiticEvents: 0,
      infected: 0, diseaseEvents: 0,
      avgPhotosynthesis: 0, avgSymbiosis: 0, avgToxicity: 0,
      newSpeciesIntroduced: 0,
      spatialClusters: 0, clusteredPopulation: 0, alphaCount: 0,
      altruismEvents: 0, avgClustering: 0, avgAltruism: 0,
      avgDominance: 0, avgNicheBreadth: 0, avgCompetitiveness: 0,
      competitionEvents: 0, neutralDriftEvents: 0,
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
    this.particles = [];
    this.biofilms = [];
    this.tick = 0;
    this.history = [];
    this.nextId = 1;
    this.nextFoodId = 1;
    this.nextSpeciesId = 1;
    this.nextColonyId = 1;
    this.nextStructureId = 1;
    this.nextRemainsId = 1;
    this.nextKnowledgeId = 1;
    this.nextBiofilmId = 1;
    this.stats = this.makeStats();

    if (this.settings.chemicalField) {
      this.chemicalField = new ChemicalField(
        this.settings.worldWidth, this.settings.worldHeight, 20,
      );
    } else {
      this.chemicalField = null;
    }

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
    const types: BiomeType[] = ['plains', 'forest', 'desert', 'tundra', 'wetland', 'ocean', 'volcanic', 'toxic', 'crystal'];
    const count = this.rng.int(5, 8);
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
      brain: this.settings.neuralBrains ? makeBrainForGenome(genome, this.rng) : null,
      fitness: 0,
      lastInputs: null,
      lastOutputs: null,
      tumbleTimer: 0,
      inBiofilm: false,
      biofilmId: null,
      sporeMode: false,
      sporeTimer: 0,
      infected: false,
      infectionTimer: 0,
      symbiosisPartner: null,
      socialRank: 'solitary',
      clusterId: null,
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

    // Deposit attractant from food, repellent from remains
    if (this.chemicalField) {
      for (const f of this.food) {
        this.chemicalField.depositAttractant(f.x, f.y, 0.5);
      }
      for (const r of this.remains) {
        this.chemicalField.depositRepellent(r.x, r.y, 0.3);
      }
    }

    for (const org of this.organisms) {
      if (!org.alive) continue;
      this.updateOrganism(org);
    }
    if (this.settings.microbialBehavior) {
      for (const org of this.organisms) {
        if (!org.alive || org.sporeMode) continue;
        const hgt = tryHGT(org, this.organisms, this.rng);
        if (hgt) {
          applyHGT(org, hgt.donor, hgt.gene);
          this.stats.hgtEvents++;
        }
      }
    }

    // Chemical field diffusion
    if (this.chemicalField) {
      this.chemicalField.diffuse();
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

    // ── Endless generation: auto-spawn life when population drops ───────
    if (this.settings.endlessGeneration) {
      this.handleEndlessGeneration();
    }

    // ── Disease events: periodic outbreaks ─────────────────────────────
    if (this.settings.diseaseEvents) {
      this.handleDiseaseEvents();
    }

    // ── Advanced biology: photosynthesis, symbiosis, parasitism ───────
    if (this.settings.advancedBiology) {
      this.processAdvancedBiology();
    }

    // ── Social behavior: clustering, hierarchy, altruism, competition ─
    if (this.settings.socialBehavior) {
      this.processSocialBehavior();
    }

    // ── Neutral drift: random genetic drift (neutral theory) ───────────
    if (this.settings.neutralDrift) {
      this.processNeutralDrift();
    }

    // Biofilm formation
    if (this.settings.microbialBehavior) {
      const result = updateBiofilms(this.organisms, this.biofilms, this.rng, this.nextBiofilmId);
      this.biofilms = result.biofilms;
      this.nextBiofilmId = result.nextId;
    } else {
      this.biofilms = [];
      for (const org of this.organisms) {
        org.inBiofilm = false;
        org.biofilmId = null;
      }
    }

    this.updateGlobalStats();

    // Generation cycle boundary
    if (this.settings.generationCycleTicks > 0 && this.tick % this.settings.generationCycleTicks === 0) {
      this.onGenerationCycle();
    }

    // World expansion
    if (this.settings.worldExpansion && this.settings.worldExpansionInterval > 0
        && this.tick % this.settings.worldExpansionInterval === 0 && this.tick > 0) {
      this.expandWorld();
    }

    // Record history point every 10 ticks
    if (this.tick % 10 === 0) {
      this.recordHistory();
    }

    this.updateParticles();
  }

  // ─── Particle system ─────────────────────────────────────────────────
  spawnParticle(x: number, y: number, type: ParticleType, hue: number) {
    if (this.particles.length > 300) return;
    const count = type === 'kill' ? 8 : type === 'death' ? 6 : type === 'birth' ? 5 : 4;
    for (let i = 0; i < count; i++) {
      const angle = (i / count) * Math.PI * 2 + this.rng.range(0, 0.5);
      const speed = type === 'kill' ? this.rng.range(1.5, 3) : this.rng.range(0.5, 1.5);
      this.particles.push({
        x,
        y,
        vx: Math.cos(angle) * speed,
        vy: Math.sin(angle) * speed,
        life: type === 'kill' ? 40 : type === 'birth' ? 30 : 25,
        maxLife: type === 'kill' ? 40 : type === 'birth' ? 30 : 25,
        hue,
        type,
        size: type === 'kill' ? 3 : 2,
      });
    }
  }

  private updateParticles() {
    for (const p of this.particles) {
      p.x += p.vx;
      p.y += p.vy;
      p.vx *= 0.92;
      p.vy *= 0.92;
      p.life--;
    }
    this.particles = this.particles.filter((p) => p.life > 0);
  }

  private recordHistory() {
    const s = this.stats;
    const point: HistoryPoint = {
      tick: this.tick,
      population: this.population,
      speciesCount: this.speciesCount,
      avgIntelligence: s.avgIntelligence,
      avgFitness: s.avgFitness,
      herbivores: s.herbivores,
      carnivores: s.carnivores,
      colonies: s.colonies,
      structures: s.structures,
      births: s.births,
      deaths: s.deaths,
      kills: s.kills,
      biofilmPopulation: s.biofilmPopulation,
      spores: s.spores,
      hgtEvents: s.hgtEvents,
      photosynthesizers: s.photosynthesizers,
      symbioticPairs: s.symbioticPairs,
      infected: s.infected,
      spatialClusters: s.spatialClusters,
      altruismEvents: s.altruismEvents,
      competitionEvents: s.competitionEvents,
      neutralDriftEvents: s.neutralDriftEvents,
      avgClustering: s.avgClustering,
      avgAltruism: s.avgAltruism,
      avgDominance: s.avgDominance,
      avgNicheBreadth: s.avgNicheBreadth,
      avgCompetitiveness: s.avgCompetitiveness,
    };
    this.history.push(point);
    if (this.history.length > 600) this.history.shift();
  }

  // ─── Generation cycle ────────────────────────────────────────────────

  private onGenerationCycle() {
    this.stats.generationCycle++;
    // Fitness is evaluated over the cycle; reset for next epoch
    for (const org of this.organisms) {
      if (!org.alive) continue;
      org.fitness = 0;
    }
    // Fire auto-checkpoint callback if registered
    if (this.onAutoCheckpoint) {
      this.onAutoCheckpoint(this.stats.generationCycle, this.tick);
    }
  }

  onAutoCheckpoint: ((cycle: number, tick: number) => void) | null = null;

  // ─── World expansion ─────────────────────────────────────────────────

  private expandWorld() {
    this.stats.worldExpansions++;
    const growW = 200;
    const growH = 150;
    this.settings.worldWidth += growW;
    this.settings.worldHeight += growH;
    this.settings.maxPopulation = Math.min(1200, this.settings.maxPopulation + 100);
    this.settings.maxFood = Math.min(800, this.settings.maxFood + 80);

    // Add a new biome in the expanded region
    if (this.settings.biomes) {
      const types: BiomeType[] = ['plains', 'forest', 'desert', 'tundra', 'wetland', 'ocean', 'volcanic', 'toxic', 'crystal'];
      const type = types[this.rng.int(0, types.length - 1)];
      const info = BIOME_INFO[type];
      this.biomes.push({
        id: this.biomes.length,
        type,
        cx: this.rng.range(this.settings.worldWidth * 0.6, this.settings.worldWidth),
        cy: this.rng.range(this.settings.worldHeight * 0.6, this.settings.worldHeight),
        radius: this.rng.range(120, 220),
        foodRate: info.foodRate,
        energyDrain: info.energyDrain,
        speedMod: info.speedMod,
        hue: info.hue,
      });
    }

    // Seed some food in the new territory
    for (let i = 0; i < 30; i++) this.spawnFood();

    // Resize chemical field to match new world dimensions
    if (this.chemicalField) {
      this.chemicalField.resize(this.settings.worldWidth, this.settings.worldHeight);
    }
  }

  // ─── Endless generation ──────────────────────────────────────────────
  private endlessTimer = 0;
  private handleEndlessGeneration() {
    this.endlessTimer++;
    // If population is very low, spawn a few organisms from random genomes
    if (this.population < 15 && this.tick % 60 === 0) {
      for (let i = 0; i < 3; i++) {
        const x = this.rng.range(50, this.settings.worldWidth - 50);
        const y = this.rng.range(50, this.settings.worldHeight - 50);
        const genome = randomGenome(this.rng);
        this.spawnOrganism(x, y, genome, 1);
        this.stats.newSpeciesIntroduced++;
      }
    }
    // Every ~20 seconds, introduce a new species with a fresh random genome
    if (this.endlessTimer % 600 === 0 && this.population < this.settings.maxPopulation * 0.7) {
      const x = this.rng.range(50, this.settings.worldWidth - 50);
      const y = this.rng.range(50, this.settings.worldHeight - 50);
      const genome = randomGenome(this.rng);
      // Give newcomers a slight random advantage to help them establish
      genome.intelligence = Math.min(1, genome.intelligence + this.rng.range(0, 0.2));
      this.spawnOrganism(x, y, genome, 1);
      this.stats.newSpeciesIntroduced++;
    }
  }

  // ─── Disease events ──────────────────────────────────────────────────
  private diseaseTimer = 0;
  private handleDiseaseEvents() {
    this.diseaseTimer++;
    // Disease outbreak every ~30 seconds
    if (this.diseaseTimer % 900 === 0 && this.diseaseTimer > 0 && this.population > 20) {
      this.stats.diseaseEvents++;
      // Pick a random location and infect nearby organisms
      const epicenterX = this.rng.range(0, this.settings.worldWidth);
      const epicenterY = this.rng.range(0, this.settings.worldHeight);
      const infectionRadius = 150;
      for (const org of this.organisms) {
        if (!org.alive || org.infected) continue;
        const dx = org.x - epicenterX;
        const dy = org.y - epicenterY;
        if (dx * dx + dy * dy < infectionRadius * infectionRadius) {
          // Disease resistance determines whether infection takes hold
          if (this.rng.next() > org.genome.diseaseResistance) {
            org.infected = true;
            org.infectionTimer = 200 + this.rng.int(0, 100);
          }
        }
      }
    }
    // Process active infections
    for (const org of this.organisms) {
      if (!org.alive || !org.infected) continue;
      org.infectionTimer--;
      // Infected organisms drain energy faster
      org.energy -= 0.15;
      // Spread to nearby organisms
      if (this.rng.next() < 0.02) {
        for (const other of this.organisms) {
          if (other === org || !other.alive || other.infected) continue;
          const dx = other.x - org.x;
          const dy = other.y - org.y;
          if (dx * dx + dy * dy < 400 && this.rng.next() > other.genome.diseaseResistance) {
            other.infected = true;
            other.infectionTimer = 200;
          }
        }
      }
      if (org.infectionTimer <= 0) {
        org.infected = false;
      }
    }
  }

  // ─── Advanced biology ─────────────────────────────────────────────────
  private processAdvancedBiology() {
    for (const org of this.organisms) {
      if (!org.alive || org.sporeMode) continue;
      const biome = this.biomeAt(org.x, org.y);
      const light = biome ? (BIOME_INFO[biome.type].light) : 1;

      // Photosynthesis: gain energy from light in lit biomes
      if (org.genome.photosynthesis > 0.2 && light > 0.3) {
        const gain = org.genome.photosynthesis * light * 0.3;
        org.energy += gain;
      }

      // Toxicity: deposit repellent that deters predators
      if (org.genome.toxicity > 0.3 && this.chemicalField) {
        this.chemicalField.depositRepellent(org.x, org.y, org.genome.toxicity * 0.2);
      }

      // Parasitism: drain energy from nearby organisms
      if (org.genome.parasitism > 0.3 && org.energy < 120) {
        for (const other of this.organisms) {
          if (other === org || !other.alive) continue;
          if (other.speciesId === org.speciesId) continue;
          const dx = other.x - org.x;
          const dy = other.y - org.y;
          if (dx * dx + dy * dy < 100) {
            const drain = Math.min(other.energy, org.genome.parasitism * 0.5);
            other.energy -= drain;
            org.energy += drain * 0.7; // parasite gets 70% of drained energy
            this.stats.parasiticEvents++;
            break;
          }
        }
      }

      // Symbiosis: form mutualistic pairs that benefit both organisms
      if (org.genome.symbiosis > 0.3 && org.symbiosisPartner === null) {
        for (const other of this.organisms) {
          if (other === org || !other.alive) continue;
          if (other.symbiosisPartner !== null) continue;
          if (other.genome.symbiosis < 0.3) continue;
          const dx = other.x - org.x;
          const dy = other.y - org.y;
          if (dx * dx + dy * dy < 2500) {
            org.symbiosisPartner = other.id;
            other.symbiosisPartner = org.id;
            break;
          }
        }
      }
      // Symbiosis benefit: both partners gain energy when close
      if (org.symbiosisPartner !== null) {
        const partner = this.organisms.find((o) => o.id === org.symbiosisPartner && o.alive);
        if (partner) {
          const dx = partner.x - org.x;
          const dy = partner.y - org.y;
          if (dx * dx + dy * dy < 6400) {
            org.energy += 0.1;
          } else {
            // Too far apart — break the symbiosis
            org.symbiosisPartner = null;
            partner.symbiosisPartner = null;
          }
        } else {
          org.symbiosisPartner = null;
        }
      }
    }
  }

  // ─── Social behavior: clustering, hierarchy, altruism, competition ─────
  private socialTimer = 0;
  private processSocialBehavior() {
    this.socialTimer++;

    // ── Spatial clustering: group organisms by proximity ──────────────
    // Run every ~3 seconds to reduce CPU cost
    if (this.socialTimer % 90 === 0) {
      this.detectSpatialClusters();
      this.assignHierarchy();
    }

    // ── Altruism: share energy with nearby same-species organisms in need
    for (const org of this.organisms) {
      if (!org.alive || org.sporeMode) continue;
      if (org.genome.altruism < 0.2 || org.energy < 80) continue;

      for (const other of this.organisms) {
        if (other === org || !other.alive) continue;
        if (other.speciesId !== org.speciesId) continue;
        if (other.energy > 40) continue; // only help those in need
        const dx = other.x - org.x;
        const dy = other.y - org.y;
        if (dx * dx + dy * dy < 900) {
          const share = Math.min(org.energy - 60, 15) * org.genome.altruism;
          if (share > 1) {
            org.energy -= share;
            other.energy += share;
            this.stats.altruismEvents++;
          }
        }
      }
    }

    // ── Competition: organisms compete for food when resources are scarce
    for (const org of this.organisms) {
      if (!org.alive || org.sporeMode) continue;
      if (org.genome.competitiveness < 0.3) continue;

      for (const other of this.organisms) {
        if (other === org || !other.alive) continue;
        if (other.speciesId === org.speciesId) continue;
        const dx = other.x - org.x;
        const dy = other.y - org.y;
        if (dx * dx + dy * dy < 400) {
          // Competitive organisms push rivals away and steal a small energy tax
          if (org.genome.competitiveness > other.genome.competitiveness) {
            const tax = (org.genome.competitiveness - other.genome.competitiveness) * 0.3;
            other.energy -= tax;
            this.stats.competitionEvents++;
          }
        }
      }
    }

    // ── Niche partitioning: organisms with broad niches tolerate more biomes
    // This is handled implicitly via biome energy drain modifiers
    for (const org of this.organisms) {
      if (!org.alive || org.sporeMode) continue;
      const biome = this.biomeAt(org.x, org.y);
      if (!biome) continue;
      // Broad-niche organisms suffer less energy drain in non-native biomes
      const drainMod = 1 - org.genome.nicheBreadth * 0.3;
      // This is applied as a small energy bonus to offset the drain
      if (drainMod < 1) {
        org.energy += (biome.energyDrain - 1) * 0.05 * (1 - drainMod);
      }
    }
  }

  // ── Spatial cluster detection using simple grid-based grouping ──────
  private nextClusterId = 1;
  private detectSpatialClusters() {
    // Reset cluster assignments
    for (const org of this.organisms) {
      org.clusterId = null;
    }

    const cellSize = 60;
    const grid = new Map<string, number[]>();
    for (const org of this.organisms) {
      if (!org.alive) continue;
      const key = `${Math.floor(org.x / cellSize)},${Math.floor(org.y / cellSize)}`;
      if (!grid.has(key)) grid.set(key, []);
      grid.get(key)!.push(org.id);
    }

    // Flood-fill adjacent cells to form clusters
    const visited = new Set<string>();
    let clusterCount = 0;
    for (const [key, ids] of grid) {
      if (visited.has(key)) continue;
      // BFS to find all connected cells
      const queue = [key];
      const clusterIds: number[] = [];
      while (queue.length > 0) {
        const cell = queue.shift()!;
        if (visited.has(cell)) continue;
        visited.add(cell);
        const cellIds = grid.get(cell);
        if (cellIds) clusterIds.push(...cellIds);
        const [cx, cy] = cell.split(',').map(Number);
        for (const [dx, dy] of [[1, 0], [-1, 0], [0, 1], [0, -1]]) {
          const nkey = `${cx + dx},${cy + dy}`;
          if (!visited.has(nkey) && grid.has(nkey)) queue.push(nkey);
        }
      }
      // Only count clusters with 3+ organisms
      if (clusterIds.length >= 3) {
        clusterCount++;
        const cid = this.nextClusterId++;
        for (const id of clusterIds) {
          const org = this.organisms.find((o) => o.id === id);
          if (org) org.clusterId = cid;
        }
      }
    }
    this.stats.spatialClusters = clusterCount;
  }

  // ── Social hierarchy: assign alpha/beta/omega ranks within colonies ─
  private assignHierarchy() {
    // Group organisms by colony
    const colonyMembers = new Map<number, Organism[]>();
    for (const org of this.organisms) {
      if (!org.alive || org.colonyId === null) {
        org.socialRank = 'solitary';
        continue;
      }
      if (!colonyMembers.has(org.colonyId)) colonyMembers.set(org.colonyId, []);
      colonyMembers.get(org.colonyId)!.push(org);
    }

    for (const [colonyId, members] of colonyMembers) {
      if (members.length < 2) {
        for (const m of members) m.socialRank = 'solitary';
        continue;
      }
      // Rank by dominance * size * energy (fitness proxy)
      const ranked = members.sort((a, b) => {
        const aScore = a.genome.dominance * a.genome.size * (a.energy / 200);
        const bScore = b.genome.dominance * b.genome.size * (b.energy / 200);
        return bScore - aScore;
      });
      const alphaCount = Math.max(1, Math.floor(ranked.length * 0.15));
      const betaCount = Math.floor(ranked.length * 0.35);
      for (let i = 0; i < ranked.length; i++) {
        if (i < alphaCount) ranked[i].socialRank = 'alpha';
        else if (i < alphaCount + betaCount) ranked[i].socialRank = 'beta';
        else ranked[i].socialRank = 'omega';
      }
    }
  }

  // ─── Neutral drift: random genetic drift (neutral theory) ──────────────
  private driftTimer = 0;
  private processNeutralDrift() {
    this.driftTimer++;
    // Every ~5 seconds, pick a random organism and apply a small random
    // genetic drift — a neutral mutation that doesn't affect fitness.
    // This models the neutral theory of molecular evolution: most mutations
    // are neutral and fix/die by chance, not selection.
    if (this.driftTimer % 150 === 0 && this.organisms.length > 0) {
      const idx = this.rng.int(0, this.organisms.length - 1);
      const org = this.organisms[idx];
      if (org && org.alive) {
        // Apply a tiny random shift to a random gene
        const genes: (keyof typeof org.genome)[] = [
          'hue', 'speed', 'senseRadius', 'metabolism', 'aggression',
          'cooperation', 'socialGene', 'curiosity', 'adaptability',
          'clustering', 'altruism', 'dominance', 'nicheBreadth',
        ];
        const gene = genes[this.rng.int(0, genes.length - 1)];
        const drift = this.rng.range(-0.02, 0.02);
        (org.genome as any)[gene] += drift;
        // Clamp to valid range
        if (typeof (org.genome as any)[gene] === 'number') {
          (org.genome as any)[gene] = Math.max(0, Math.min(1, (org.genome as any)[gene]));
        }
        this.stats.neutralDriftEvents++;
      }
    }
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

    // ─── Spore mode: dormant survival state ────────────────────────────
    if (org.sporeMode) {
      const biomeDrain = biome ? biome.energyDrain : 1;
      const stillSpore = trySporulation(org, biomeDrain, 0);
      if (stillSpore) {
        // Spores use almost no energy, don't move or reproduce
        org.energy -= 0.02;
        if (org.energy <= 0) {
          org.alive = false;
          this.stats.deaths++;
          this.spawnRemains(org.x, org.y, org.genome.size * 2, org.genome.hue);
        }
        return;
      }
    }

    // ─── Microbial behavior: run-and-tumble + chemotaxis ───────────────
    // For low-intelligence organisms (no brain yet), movement is driven
    // by bacterial-style chemotaxis instead of the hardcoded AI.
    if (this.settings.microbialBehavior && intel < 0.2 && !org.brain) {
      const tumbleResult = runAndTumble(org, this.chemicalField, this.rng);
      org.angle = tumbleResult.angle;

      // Quorum sensing: adjust behavior based on local density
      if (org.genome.quorumSensing > 0.3) {
        const quorum = quorumSense(org, this.organisms, sense);
        // At high density, slow down (save energy) and increase reproduction readiness
        if (quorum.density > 0.6) {
          tumbleResult.speedMod *= 0.6;
        }
        // Deposit attractant to help cluster (quorum sensing signal)
        if (this.chemicalField && org.genome.biofilmFormation > 0.3) {
          this.chemicalField.depositAttractant(org.x, org.y, 0.1 * quorum.density);
        }
      }

      // Biofilm membership reduces movement and energy drain
      let biofilmSpeedMod = 1;
      if (org.inBiofilm) {
        biofilmSpeedMod = 0.3;
        // Biofilm protection: reduced energy drain
        org.energy += 0.05;
      }

      // Try sporulation under harsh conditions
      const biomeDrain = biome ? biome.energyDrain : 1;
      if (trySporulation(org, biomeDrain, org.threatLevel)) {
        return; // entered spore mode, skip rest
      }

      this.moveOrganism(org, biome, biofilmSpeedMod);
      this.applyEnergyCost(org, biome);

      // Eat food if overlapping
      for (const f of this.food) {
        if (f.energy <= 0) continue;
        const dx = f.x - org.x;
        const dy = f.y - org.y;
        if (dx * dx + dy * dy < (org.genome.size + 4) * (org.genome.size + 4)) {
          org.energy += f.energy;
          f.energy = 0;
        }
      }

      return; // microbial behavior fully handles this organism
    }

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

    // ─── Neural brain evaluation ──────────────────────────────────────
    // The brain produces behavioral modifiers that tune instinct-driven
    // decisions. Smarter creatures get better at surviving through evolution.
    let brainSpeedMod = 1;
    if (org.brain) {
      const inputs = this.collectBrainInputs(org, intel, effectiveSense, isCarnivore);
      const outputs = new Float32Array(N_OUTPUTS);
      evalBrain(org.brain, inputs, outputs);
      org.lastInputs = inputs;
      org.lastOutputs = outputs;
      brainSpeedMod = 0.5 + outputs[2] * 0.5 + 0.5; // 0.5..1.5
    }

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
        this.moveOrganism(org, biome, brainSpeedMod);
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
          this.spawnParticle(other.x, other.y, 'kill', other.genome.hue);
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

    // Neural brain can override or blend with instinct steering
    if (org.brain && org.lastOutputs) {
      const o = org.lastOutputs;
      // If the brain produces strong steering, blend it with instinct angle
      const steerStrength = Math.hypot(o[0], o[1]);
      if (steerStrength > 0.15) {
        const brainAngle = Math.atan2(o[1], o[0]);
        // Blend factor depends on intelligence — smarter creatures trust their brain more
        const blend = Math.min(0.7, intel * 0.8);
        org.angle = this.lerpAngle(org.angle, brainAngle, blend);
      }
    }

    this.moveOrganism(org, biome, brainSpeedMod);
    this.applyEnergyCost(org, biome);

    // Death from old age / starvation leaves remains
    if (!org.alive && org.energy <= 0) {
      this.spawnRemains(org.x, org.y, org.genome.size * 1.5, org.genome.hue);
    }
  }

  // ─── Neural brain input collection ─────────────────────────────────
  private collectBrainInputs(
    org: Organism, intel: number, senseRadius: number, isCarnivore: boolean,
  ): Float32Array {
    const inputs = new Float32Array(N_INPUTS);
    // [0] energy level (0..1)
    inputs[0] = Math.min(1, org.energy / 200);
    // [1] health proxy: 1 - age/lifespan
    inputs[1] = 1 - Math.min(1, org.age / org.genome.lifespan);
    // [2] intelligence
    inputs[2] = intel;
    // [3] aggression
    inputs[3] = org.genome.aggression;
    // [4] is carnivore
    inputs[4] = isCarnivore ? 1 : 0;
    // [5] reproduction readiness
    inputs[5] = Math.min(1, org.energy / this.settings.reproductionThreshold);
    // [6] carrying materials
    inputs[6] = Math.min(1, org.carrying / 20);
    // [7] knowledge boost
    inputs[7] = Math.min(1, org.knowledgeBoost / 10);
    // [8] threat level
    inputs[8] = Math.min(1, org.threatLevel);
    // [9] colony member
    inputs[9] = org.colonyId !== null ? 1 : 0;
    // [10] nearest food direction (cos)
    // [11] nearest food direction (sin)
    // [12] nearest threat direction (cos)
    // [13] nearest threat direction (sin)
    let nearestFoodDist = Infinity;
    let nearestFoodX = 0, nearestFoodY = 0;
    for (const f of this.food) {
      const dx = f.x - org.x;
      const dy = f.y - org.y;
      const d2 = dx * dx + dy * dy;
      if (d2 < senseRadius * senseRadius && d2 < nearestFoodDist) {
        nearestFoodDist = d2;
        nearestFoodX = dx; nearestFoodY = dy;
      }
    }
    if (nearestFoodDist < Infinity) {
      const len = Math.hypot(nearestFoodX, nearestFoodY) || 1;
      inputs[10] = nearestFoodX / len;
      inputs[11] = nearestFoodY / len;
    }
    let nearestThreatDist = Infinity;
    let nearestThreatX = 0, nearestThreatY = 0;
    for (const other of this.organisms) {
      if (other.id === org.id || !other.alive) continue;
      if (other.genome.diet < 0.5) continue; // only carnivores are threats
      if (other.genome.size <= org.genome.size * 1.2) continue;
      const dx = other.x - org.x;
      const dy = other.y - org.y;
      const d2 = dx * dx + dy * dy;
      if (d2 < senseRadius * senseRadius && d2 < nearestThreatDist) {
        nearestThreatDist = d2;
        nearestThreatX = dx; nearestThreatY = dy;
      }
    }
    if (nearestThreatDist < Infinity) {
      const len = Math.hypot(nearestThreatX, nearestThreatY) || 1;
      inputs[12] = nearestThreatX / len;
      inputs[13] = nearestThreatY / len;
    }
    return inputs;
  }

  private lerpAngle(a: number, b: number, t: number): number {
    let diff = b - a;
    while (diff > Math.PI) diff -= Math.PI * 2;
    while (diff < -Math.PI) diff += Math.PI * 2;
    return a + diff * t;
  }

  private moveOrganism(org: Organism, biome: Biome | undefined, speedMod = 1) {
    const speed = org.genome.speed * (biome ? biome.speedMod : 1) * speedMod;

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
    // Aging rate gene: lower agingRate = slower senescence
    if (org.age > maxAge) org.energy -= (org.age - maxAge) * 0.02 * org.genome.agingRate;

    if (org.energy <= 0) {
      org.alive = false;
      this.stats.deaths++;
      this.spawnParticle(org.x, org.y, 'death', org.genome.hue);
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
    const childBrain = this.settings.neuralBrains
      ? mutateBrainForGenome(childGenome, parent.brain, this.settings.mutationRate, this.rng)
      : null;
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
      brain: childBrain,
      fitness: 0,
      lastInputs: null, lastOutputs: null,
      tumbleTimer: 0,
      inBiofilm: false,
      biofilmId: null,
      sporeMode: false,
      sporeTimer: 0,
      infected: false,
      infectionTimer: 0,
      symbiosisPartner: null,
      socialRank: 'solitary',
      clusterId: null,
    };
    this.stats.births++;
    this.stats.asexualReproductions++;
    this.spawnParticle(child.x, child.y, 'birth', childGenome.hue);
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

    const childBrain = this.settings.neuralBrains
      ? crossoverBrainForGenome(parentA.genome, parentB.genome, parentA.brain, parentB.brain, this.rng)
      : null;

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
      brain: childBrain,
      fitness: 0,
      lastInputs: null, lastOutputs: null,
      tumbleTimer: 0,
      inBiofilm: false,
      biofilmId: null,
      sporeMode: false,
      sporeTimer: 0,
      infected: false,
      infectionTimer: 0,
      symbiosisPartner: null,
      socialRank: 'solitary',
      clusterId: null,
    };
    this.stats.births++;
    this.stats.sexualReproductions++;
    this.spawnParticle(child.x, child.y, 'birth', child.genome.hue);
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

    // Brain-driven population + fitness tracking
    let brainPop = 0;
    let totalFitness = 0;
    let peakFitness = 0;
    for (const org of this.organisms) {
      if (org.brain) brainPop++;
      // Fitness accumulates: survival + energy + knowledge + kills
      const f = org.age * 0.01 + org.energy * 0.1 + org.knowledgeBoost * 2;
      org.fitness += f * 0.001; // slow accrual
      totalFitness += org.fitness;
      if (org.fitness > peakFitness) peakFitness = org.fitness;
    }
    this.stats.brainDrivenPopulation = brainPop;
    this.stats.avgFitness = n > 0 ? totalFitness / n : 0;
    this.stats.peakFitness = peakFitness;

    // Microbial stats
    let biofilmPop = 0;
    let sporeCount = 0;
    let totalChemotaxis = 0;
    let totalQuorum = 0;
    let totalBiofilm = 0;
    for (const org of this.organisms) {
      if (org.inBiofilm) biofilmPop++;
      if (org.sporeMode) sporeCount++;
      totalChemotaxis += org.genome.chemotaxis;
      totalQuorum += org.genome.quorumSensing;
      totalBiofilm += org.genome.biofilmFormation;
    }
    this.stats.biofilmClusters = this.biofilms.length;
    this.stats.biofilmPopulation = biofilmPop;
    this.stats.spores = sporeCount;
    this.stats.avgChemotaxis = n > 0 ? totalChemotaxis / n : 0;
    this.stats.avgQuorum = n > 0 ? totalQuorum / n : 0;
    this.stats.avgBiofilm = n > 0 ? totalBiofilm / n : 0;

    // Advanced biological stats
    let photoCount = 0;
    let symbPairs = 0;
    let infectedCount = 0;
    let totalPhoto = 0;
    let totalSymb = 0;
    let totalTox = 0;
    const symbiosisSeen = new Set<number>();
    for (const org of this.organisms) {
      if (org.genome.photosynthesis > 0.3) photoCount++;
      if (org.symbiosisPartner !== null && !symbiosisSeen.has(org.id)) {
        symbPairs++;
        symbiosisSeen.add(org.id);
        const partner = this.organisms.find((o) => o.id === org.symbiosisPartner);
        if (partner) symbiosisSeen.add(partner.id);
      }
      if (org.infected) infectedCount++;
      totalPhoto += org.genome.photosynthesis;
      totalSymb += org.genome.symbiosis;
      totalTox += org.genome.toxicity;
    }
    this.stats.photosynthesizers = photoCount;
    this.stats.symbioticPairs = symbPairs;
    this.stats.infected = infectedCount;
    this.stats.avgPhotosynthesis = n > 0 ? totalPhoto / n : 0;
    this.stats.avgSymbiosis = n > 0 ? totalSymb / n : 0;
    this.stats.avgToxicity = n > 0 ? totalTox / n : 0;

    // Social & ecological stats
    let totalCluster = 0, totalAltruism = 0, totalDominance = 0;
    let totalNiche = 0, totalCompete = 0;
    let alphaCount = 0;
    let clusteredPop = 0;
    for (const org of this.organisms) {
      totalCluster += org.genome.clustering;
      totalAltruism += org.genome.altruism;
      totalDominance += org.genome.dominance;
      totalNiche += org.genome.nicheBreadth;
      totalCompete += org.genome.competitiveness;
      if (org.socialRank === 'alpha') alphaCount++;
      if (org.clusterId !== null) clusteredPop++;
    }
    this.stats.alphaCount = alphaCount;
    this.stats.clusteredPopulation = clusteredPop;
    this.stats.avgClustering = n > 0 ? totalCluster / n : 0;
    this.stats.avgAltruism = n > 0 ? totalAltruism / n : 0;
    this.stats.avgDominance = n > 0 ? totalDominance / n : 0;
    this.stats.avgNicheBreadth = n > 0 ? totalNiche / n : 0;
    this.stats.avgCompetitiveness = n > 0 ? totalCompete / n : 0;
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
        brain: this.settings.neuralBrains ? makeBrainForGenome(genome, this.rng) : null,
        fitness: 0,
        lastInputs: null, lastOutputs: null,
        tumbleTimer: 0,
        inBiofilm: false,
        biofilmId: null,
        sporeMode: false,
        sporeTimer: 0,
        infected: false,
        infectionTimer: 0,
        symbiosisPartner: null,
        socialRank: 'solitary',
        clusterId: null,
      };
      this.organisms.push(org);
      if (org.genome.diet >= 0.5) this.stats.carnivores++;
      else this.stats.herbivores++;
    }
  }
}
