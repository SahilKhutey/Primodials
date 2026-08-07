import { useRef, useEffect, useState } from 'react';
import { TrendingUp, Users, Dna, Brain, Swords, Leaf, Beef, Network, Sun, Bug, Handshake, Crown } from 'lucide-react';
import type { Simulation } from '@/sim/simulation';
import type { HistoryPoint } from '@/sim/types';

type SeriesKey = keyof Omit<HistoryPoint, 'tick'>;

type SeriesDef = {
  key: SeriesKey;
  label: string;
  color: string;
  icon: React.ReactNode;
  group: string;
};

const ALL_SERIES: SeriesDef[] = [
  { key: 'population', label: 'Population', color: '#f43f5e', icon: <Users size={14} />, group: 'Core' },
  { key: 'speciesCount', label: 'Species Diversity', color: '#a78bfa', icon: <Dna size={14} />, group: 'Core' },
  { key: 'avgIntelligence', label: 'Avg Intelligence', color: '#e5e7eb', icon: <Brain size={14} />, group: 'Core' },
  { key: 'avgFitness', label: 'Avg Fitness', color: '#facc15', icon: <TrendingUp size={14} />, group: 'Core' },
  { key: 'herbivores', label: 'Herbivores', color: '#4ade80', icon: <Leaf size={14} />, group: 'Diet' },
  { key: 'carnivores', label: 'Carnivores', color: '#f87171', icon: <Beef size={14} />, group: 'Diet' },
  { key: 'colonies', label: 'Colonies', color: '#2dd4bf', icon: <Network size={14} />, group: 'Society' },
  { key: 'structures', label: 'Structures', color: '#38bdf8', icon: <Network size={14} />, group: 'Society' },
  { key: 'births', label: 'Births', color: '#f472b6', icon: <Users size={14} />, group: 'Vital' },
  { key: 'deaths', label: 'Deaths', color: '#9ca3af', icon: <Users size={14} />, group: 'Vital' },
  { key: 'kills', label: 'Kills', color: '#fb923c', icon: <Swords size={14} />, group: 'Vital' },
  { key: 'biofilmPopulation', label: 'Biofilm Pop', color: '#34d399', icon: <Network size={14} />, group: 'Microbial' },
  { key: 'spores', label: 'Spores', color: '#fbbf24', icon: <Bug size={14} />, group: 'Microbial' },
  { key: 'hgtEvents', label: 'HGT Events', color: '#22d3ee', icon: <Network size={14} />, group: 'Microbial' },
  { key: 'photosynthesizers', label: 'Photosynth', color: '#fde047', icon: <Sun size={14} />, group: 'Advanced' },
  { key: 'symbioticPairs', label: 'Symbiotic Pairs', color: '#6ee7b7', icon: <Handshake size={14} />, group: 'Advanced' },
  { key: 'infected', label: 'Infected', color: '#ef4444', icon: <Bug size={14} />, group: 'Advanced' },
  { key: 'spatialClusters', label: 'Spatial Clusters', color: '#818cf8', icon: <Network size={14} />, group: 'Social' },
  { key: 'altruismEvents', label: 'Altruism', color: '#4ade80', icon: <Handshake size={14} />, group: 'Social' },
  { key: 'competitionEvents', label: 'Competition', color: '#fb923c', icon: <Swords size={14} />, group: 'Social' },
  { key: 'neutralDriftEvents', label: 'Neutral Drift', color: '#94a3b8', icon: <TrendingUp size={14} />, group: 'Social' },
  { key: 'avgClustering', label: 'Avg Clustering', color: '#a78bfa', icon: <Network size={14} />, group: 'Social' },
  { key: 'avgAltruism', label: 'Avg Altruism', color: '#86efac', icon: <Handshake size={14} />, group: 'Social' },
  { key: 'avgDominance', label: 'Avg Dominance', color: '#fcd34d', icon: <Crown size={14} />, group: 'Social' },
  { key: 'avgNicheBreadth', label: 'Avg Niche Width', color: '#5eead4', icon: <Leaf size={14} />, group: 'Social' },
  { key: 'avgCompetitiveness', label: 'Avg Competitiveness', color: '#fdba74', icon: <Swords size={14} />, group: 'Social' },
];

const GROUPS = ['Core', 'Diet', 'Vital', 'Society', 'Microbial', 'Advanced', 'Social'];

function drawChart(
  canvas: HTMLCanvasElement,
  history: HistoryPoint[],
  activeSeries: SeriesKey[],
) {
  const ctx = canvas.getContext('2d');
  if (!ctx) return;

  const dpr = window.devicePixelRatio || 1;
  const rect = canvas.getBoundingClientRect();
  const w = rect.width;
  const h = rect.height;
  canvas.width = w * dpr;
  canvas.height = h * dpr;
  ctx.scale(dpr, dpr);

  ctx.clearRect(0, 0, w, h);

  // Background
  ctx.fillStyle = '#0a0a0a';
  ctx.fillRect(0, 0, w, h);

  const padL = 44;
  const padR = 12;
  const padT = 12;
  const padB = 24;
  const chartW = w - padL - padR;
  const chartH = h - padT - padB;

  // Grid lines
  ctx.strokeStyle = 'rgba(255,255,255,0.05)';
  ctx.lineWidth = 1;
  for (let i = 0; i <= 4; i++) {
    const y = padT + (chartH / 4) * i;
    ctx.beginPath();
    ctx.moveTo(padL, y);
    ctx.lineTo(w - padR, y);
    ctx.stroke();
  }
  for (let i = 0; i <= 6; i++) {
    const x = padL + (chartW / 6) * i;
    ctx.beginPath();
    ctx.moveTo(x, padT);
    ctx.lineTo(x, h - padB);
    ctx.stroke();
  }

  if (history.length < 2 || activeSeries.length === 0) {
    ctx.fillStyle = 'rgba(255,255,255,0.3)';
    ctx.font = '13px system-ui, sans-serif';
    ctx.textAlign = 'center';
    ctx.fillText('Collecting data...', w / 2, h / 2);
    return;
  }

  const n = history.length;
  const xScale = (i: number) => padL + (i / (n - 1)) * chartW;

  // Compute global max for normalization (each series gets its own scale)
  const seriesMax = new Map<SeriesKey, number>();
  for (const key of activeSeries) {
    let mx = 0;
    for (const p of history) {
      const v = p[key];
      if (v > mx) mx = v;
    }
    seriesMax.set(key, mx || 1);
  }

  // Y-axis labels (normalized 0-100%)
  ctx.fillStyle = 'rgba(255,255,255,0.25)';
  ctx.font = '10px system-ui, sans-serif';
  ctx.textAlign = 'right';
  for (let i = 0; i <= 4; i++) {
    const y = padT + (chartH / 4) * i;
    const pct = 100 - i * 25;
    ctx.fillText(`${pct}%`, padL - 6, y + 3);
  }

  // X-axis labels (tick range)
  ctx.textAlign = 'center';
  const firstTick = history[0].tick;
  const lastTick = history[n - 1].tick;
  ctx.fillText(`t=${firstTick}`, padL, h - padB + 14);
  ctx.fillText(`t=${lastTick}`, w - padR, h - padB + 14);
  const midTick = Math.round((firstTick + lastTick) / 2);
  ctx.fillText(`t=${midTick}`, padL + chartW / 2, h - padB + 14);

  // Draw each series
  for (const key of activeSeries) {
    const def = ALL_SERIES.find((s) => s.key === key);
    if (!def) continue;
    const mx = seriesMax.get(key)!;
    const yScale = (v: number) => padT + chartH - (v / mx) * chartH;

    // Glow line
    ctx.strokeStyle = def.color;
    ctx.lineWidth = 2;
    ctx.lineJoin = 'round';
    ctx.lineCap = 'round';
    ctx.shadowColor = def.color;
    ctx.shadowBlur = 6;
    ctx.globalAlpha = 0.9;

    ctx.beginPath();
    for (let i = 0; i < n; i++) {
      const x = xScale(i);
      const y = yScale(history[i][key]);
      if (i === 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.stroke();

    // Fill under curve
    ctx.shadowBlur = 0;
    ctx.globalAlpha = 0.08;
    ctx.lineTo(xScale(n - 1), padT + chartH);
    ctx.lineTo(xScale(0), padT + chartH);
    ctx.closePath();
    ctx.fillStyle = def.color;
    ctx.fill();
  }

  ctx.globalAlpha = 1;
  ctx.shadowBlur = 0;
}

type Props = { sim: Simulation };

export function EvolutionChart({ sim }: Props) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [activeKeys, setActiveKeys] = useState<Set<SeriesKey>>(
    new Set<SeriesKey>(['population', 'speciesCount', 'avgIntelligence']),
  );
  const [, force] = useState(0);

  // Re-render chart ~4x/sec to reflect new data
  useEffect(() => {
    const id = setInterval(() => force((n) => n + 1), 250);
    return () => clearInterval(id);
  }, []);

  useEffect(() => {
    if (canvasRef.current) {
      drawChart(canvasRef.current, sim.history, Array.from(activeKeys));
    }
  });

  const toggleSeries = (key: SeriesKey) => {
    setActiveKeys((prev) => {
      const next = new Set(prev);
      if (next.has(key)) next.delete(key);
      else next.add(key);
      return next;
    });
  };

  const activeSeries = ALL_SERIES.filter((s) => activeKeys.has(s.key));

  return (
    <div className="space-y-3">
      {/* Chart canvas */}
      <div className="relative overflow-hidden rounded-xl border border-neutral-700/60 bg-neutral-950">
        <canvas ref={canvasRef} className="h-48 w-full" />
        {/* Legend overlay */}
        <div className="absolute right-3 top-3 flex flex-col items-end gap-1">
          {activeSeries.map((s) => (
            <div key={s.key} className="flex items-center gap-1.5 rounded-full bg-black/50 px-2 py-0.5 backdrop-blur-sm">
              <span className="h-2 w-2 rounded-full" style={{ backgroundColor: s.color }} />
              <span className="text-[10px] font-medium text-neutral-300">{s.label}</span>
            </div>
          ))}
        </div>
      </div>

      {/* Series selector grouped */}
      <div className="space-y-2">
        {GROUPS.map((group) => {
          const groupSeries = ALL_SERIES.filter((s) => s.group === group);
          return (
            <div key={group}>
              <div className="mb-1 text-[10px] font-semibold uppercase tracking-wider text-neutral-500">{group}</div>
              <div className="flex flex-wrap gap-1">
                {groupSeries.map((s) => {
                  const active = activeKeys.has(s.key);
                  return (
                    <button
                      key={s.key}
                      onClick={() => toggleSeries(s.key)}
                      className={`flex items-center gap-1 rounded-lg px-2 py-1 text-[11px] font-medium transition ${
                        active
                          ? 'bg-neutral-700/60 text-neutral-100 ring-1 ring-white/10'
                          : 'bg-neutral-800/40 text-neutral-500 hover:text-neutral-300'
                      }`}
                    >
                      <span style={{ color: active ? s.color : undefined }}>{s.icon}</span>
                      {s.label}
                    </button>
                  );
                })}
              </div>
            </div>
          );
        })}
      </div>

      <p className="text-xs text-neutral-500">
        Each series is auto-scaled to its own maximum. Toggle metrics to compare evolutionary trends over time.
      </p>
    </div>
  );
}
