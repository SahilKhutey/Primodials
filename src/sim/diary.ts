// Ecosystem diary — tracks interesting events in the simulation
// and surfaces them to the user as a timeline.

import type { Simulation } from './simulation';

export type DiaryEventType =
  | 'first_spawn'
  | 'extinction'
  | 'new_species'
  | 'population_boom'
  | 'population_crash'
  | 'long_lineage'
  | 'theme_changed'
  | 'daily_milestone';

export type DiaryEntry = {
  id: number;
  type: DiaryEventType;
  tick: number;
  wallTime: number; // Date.now()
  speciesId: number | null;
  countBefore: number;
  countAfter: number;
  generation: number;
  description: string;
};

const COOLDOWN_MS = 10_000; // Don't repeat same event type within 10s
const MAX_ENTRIES = 500;

const EVENT_LABELS: Record<DiaryEventType, string> = {
  first_spawn: 'First Life',
  extinction: 'Extinction',
  new_species: 'New Species',
  population_boom: 'Population Boom',
  population_crash: 'Population Crash',
  long_lineage: 'Long Lineage',
  theme_changed: 'Theme Changed',
  daily_milestone: 'Daily Milestone',
};

export class EcosystemDiary {
  entries: DiaryEntry[] = [];
  private nextId = 1;
  private lastEventTime: Partial<Record<DiaryEventType, number>> = {};
  private lastSpeciesCounts: Map<number, number> = new Map();
  private lastPopulation = 0;
  private lastPopCheck = 0;
  private installTime = Date.now();
  private lastDailyCheck = Date.now();
  private callbacks: ((entry: DiaryEntry) => void)[] = [];

  onEvent(cb: (entry: DiaryEntry) => void): void {
    this.callbacks.push(cb);
  }

  // Called each tick from the simulation loop
  update(sim: Simulation): void {
    const now = Date.now();

    // Track species changes
    for (const sp of sim.species) {
      const prev = this.lastSpeciesCounts.get(sp.id) ?? 0;
      if (prev > 0 && sp.count === 0) {
        this.record('extinction', sim.tick, {
          speciesId: sp.id,
          countBefore: prev,
          countAfter: 0,
          description: `Species #${sp.id} went extinct (had ${prev} members)`,
        });
      } else if (prev === 0 && sp.count > 0) {
        this.record('new_species', sim.tick, {
          speciesId: sp.id,
          countAfter: sp.count,
          description: `New species #${sp.id} emerged with ${sp.count} members`,
        });
      }
      this.lastSpeciesCounts.set(sp.id, sp.count);
    }

    // Track population booms/crashes (compare over ~10 seconds)
    if (now - this.lastPopCheck > 10_000) {
      const pop = sim.population;
      if (this.lastPopulation > 0) {
        const ratio = pop / this.lastPopulation;
        if (ratio >= 2) {
          this.record('population_boom', sim.tick, {
            countBefore: this.lastPopulation,
            countAfter: pop,
            description: `Population boom: ${this.lastPopulation} → ${pop}`,
          });
        } else if (ratio <= 0.5) {
          this.record('population_crash', sim.tick, {
            countBefore: this.lastPopulation,
            countAfter: pop,
            description: `Population crash: ${this.lastPopulation} → ${pop}`,
          });
        }
      }
      this.lastPopulation = pop;
      this.lastPopCheck = now;
    }

    // Track long lineages
    if (sim.stats.maxGeneration > 0 && sim.stats.maxGeneration % 50 === 0) {
      this.record('long_lineage', sim.tick, {
        generation: sim.stats.maxGeneration,
        description: `A creature reached generation ${sim.stats.maxGeneration}`,
      });
    }

    // Daily milestone
    if (now - this.lastDailyCheck > 86_400_000) {
      this.lastDailyCheck = now;
      const days = Math.floor((now - this.installTime) / 86_400_000);
      this.record('daily_milestone', sim.tick, {
        description: `Day ${days + 1} — your wallpaper has been running for ${days + 1} day(s)`,
      });
    }
  }

  notifyThemeChanged(from: string, to: string): void {
    this.record('theme_changed', 0, {
      description: `Theme changed from "${from}" to "${to}"`,
    });
  }

  getRecent(max = 100): DiaryEntry[] {
    return this.entries.slice(-max).reverse();
  }

  getEventLabel(type: DiaryEventType): string {
    return EVENT_LABELS[type] ?? type;
  }

  generateSummary(): string {
    const extinctions = this.entries.filter((e) => e.type === 'extinction').length;
    const emergences = this.entries.filter((e) => e.type === 'new_species').length;
    const booms = this.entries.filter((e) => e.type === 'population_boom').length;
    const crashes = this.entries.filter((e) => e.type === 'population_crash').length;
    const days = Math.floor((Date.now() - this.installTime) / 86_400_000);

    return [
      '=== Ecosystem Diary Summary ===',
      '',
      `Total events: ${this.entries.length}`,
      `Days running: ${days}`,
      `Extinctions: ${extinctions}`,
      `New species: ${emergences}`,
      `Population booms: ${booms}`,
      `Population crashes: ${crashes}`,
      '',
      'Recent events:',
      ...this.getRecent(10).map((e) => `  [${this.formatTime(e.wallTime)}] ${e.description}`),
    ].join('\n');
  }

  clear(): void {
    this.entries = [];
    this.nextId = 1;
  }

  getInstallTime(): number {
    return this.installTime;
  }

  private record(
    type: DiaryEventType,
    tick: number,
    data: {
      speciesId?: number;
      countBefore?: number;
      countAfter?: number;
      generation?: number;
      description: string;
    },
  ): void {
    const now = Date.now();
    const lastTime = this.lastEventTime[type];
    if (lastTime && now - lastTime < COOLDOWN_MS) return;
    this.lastEventTime[type] = now;

    const entry: DiaryEntry = {
      id: this.nextId++,
      type,
      tick,
      wallTime: now,
      speciesId: data.speciesId ?? null,
      countBefore: data.countBefore ?? 0,
      countAfter: data.countAfter ?? 0,
      generation: data.generation ?? 0,
      description: data.description,
    };

    this.entries.push(entry);
    if (this.entries.length > MAX_ENTRIES) {
      this.entries.shift();
    }

    for (const cb of this.callbacks) cb(entry);
  }

  private formatTime(ts: number): string {
    const diff = Date.now() - ts;
    if (diff < 60_000) return 'just now';
    if (diff < 3_600_000) return `${Math.floor(diff / 60_000)}m ago`;
    if (diff < 86_400_000) return `${Math.floor(diff / 3_600_000)}h ago`;
    return `${Math.floor(diff / 86_400_000)}d ago`;
  }
}
