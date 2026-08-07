import { useState } from 'react';
import { BookOpen, Skull, Sparkles, TrendingUp, TrendingDown, Dna, Palette, Calendar, X } from 'lucide-react';
import type { EcosystemDiary, DiaryEntry, DiaryEventType } from '@/sim/diary';

type Props = {
  diary: EcosystemDiary;
  visible: boolean;
  onClose: () => void;
};

const EVENT_ICONS: Record<DiaryEventType, React.ReactNode> = {
  first_spawn: <Sparkles size={14} className="text-emerald-300" />,
  extinction: <Skull size={14} className="text-rose-300" />,
  new_species: <Dna size={14} className="text-cyan-300" />,
  population_boom: <TrendingUp size={14} className="text-amber-300" />,
  population_crash: <TrendingDown size={14} className="text-orange-300" />,
  long_lineage: <Dna size={14} className="text-violet-300" />,
  theme_changed: <Palette size={14} className="text-pink-300" />,
  daily_milestone: <Calendar size={14} className="text-blue-300" />,
};

const EVENT_BG: Record<DiaryEventType, string> = {
  first_spawn: 'bg-emerald-500/10 ring-emerald-400/20',
  extinction: 'bg-rose-500/10 ring-rose-400/20',
  new_species: 'bg-cyan-500/10 ring-cyan-400/20',
  population_boom: 'bg-amber-500/10 ring-amber-400/20',
  population_crash: 'bg-orange-500/10 ring-orange-400/20',
  long_lineage: 'bg-violet-500/10 ring-violet-400/20',
  theme_changed: 'bg-pink-500/10 ring-pink-400/20',
  daily_milestone: 'bg-blue-500/10 ring-blue-400/20',
};

function formatTime(ts: number): string {
  const diff = Date.now() - ts;
  if (diff < 60_000) return 'just now';
  if (diff < 3_600_000) return `${Math.floor(diff / 60_000)}m ago`;
  if (diff < 86_400_000) return `${Math.floor(diff / 3_600_000)}h ago`;
  return `${Math.floor(diff / 86_400_000)}d ago`;
}

export function DiaryPanel({ diary, visible, onClose }: Props) {
  const [showSummary, setShowSummary] = useState(false);

  if (!visible) return null;

  const entries = diary.getRecent(200);
  const extinctions = diary.entries.filter((e) => e.type === 'extinction').length;
  const emergences = diary.entries.filter((e) => e.type === 'new_species').length;
  const days = Math.max(1, Math.floor((Date.now() - diary.getInstallTime()) / 86_400_000));

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-black/60 backdrop-blur-sm">
      <div className="flex h-[80vh] w-[90vw] max-w-2xl flex-col overflow-hidden rounded-2xl border border-white/10 bg-neutral-950/95 shadow-2xl shadow-black/60">
        {/* Header */}
        <div className="flex items-center justify-between border-b border-white/10 px-5 py-4">
          <div className="flex items-center gap-2.5">
            <div className="rounded-lg bg-neutral-900/60 p-1.5 ring-1 ring-white/10">
              <BookOpen size={18} className="text-amber-300" />
            </div>
            <div>
              <h2 className="text-sm font-bold text-neutral-100">Ecosystem Diary</h2>
              <p className="text-[10px] text-neutral-500">{diary.entries.length} events recorded over {days} day{days !== 1 ? 's' : ''}</p>
            </div>
          </div>
          <div className="flex items-center gap-2">
            <button
              onClick={() => setShowSummary((s) => !s)}
              className="rounded-lg bg-neutral-900/60 px-3 py-1.5 text-[11px] font-medium text-neutral-400 ring-1 ring-white/10 transition hover:bg-neutral-800 hover:text-neutral-200"
            >
              {showSummary ? 'Timeline' : 'Summary'}
            </button>
            <button
              onClick={() => {
                const report = diary.generateSummary();
                navigator.clipboard?.writeText(report);
              }}
              className="rounded-lg bg-neutral-900/60 px-3 py-1.5 text-[11px] font-medium text-neutral-400 ring-1 ring-white/10 transition hover:bg-neutral-800 hover:text-neutral-200"
            >
              Copy Report
            </button>
            <button
              onClick={onClose}
              className="rounded-lg bg-neutral-900/60 p-1.5 text-neutral-400 ring-1 ring-white/10 transition hover:bg-neutral-800 hover:text-neutral-200"
            >
              <X size={16} />
            </button>
          </div>
        </div>

        {/* Stats strip */}
        <div className="flex gap-2 border-b border-white/10 px-5 py-3">
          <div className="flex items-center gap-1.5 rounded-lg bg-rose-500/10 px-2.5 py-1.5 ring-1 ring-rose-400/20">
            <Skull size={12} className="text-rose-300" />
            <span className="text-[11px] font-semibold text-rose-300">{extinctions}</span>
            <span className="text-[10px] text-neutral-500">extinctions</span>
          </div>
          <div className="flex items-center gap-1.5 rounded-lg bg-cyan-500/10 px-2.5 py-1.5 ring-1 ring-cyan-400/20">
            <Dna size={12} className="text-cyan-300" />
            <span className="text-[11px] font-semibold text-cyan-300">{emergences}</span>
            <span className="text-[10px] text-neutral-500">new species</span>
          </div>
        </div>

        {/* Content */}
        <div className="flex-1 overflow-y-auto px-5 py-4">
          {showSummary ? (
            <div className="whitespace-pre-wrap text-[11px] leading-relaxed text-neutral-400">
              {diary.generateSummary()}
            </div>
          ) : entries.length === 0 ? (
            <div className="flex h-full flex-col items-center justify-center gap-2 text-center">
              <BookOpen size={32} className="text-neutral-700" />
              <p className="text-sm font-medium text-neutral-500">No events recorded yet</p>
              <p className="text-[11px] text-neutral-600">Watch your ecosystem — events appear as they happen</p>
            </div>
          ) : (
            <div className="flex flex-col gap-1.5">
              {entries.map((entry, i) => (
                <DiaryEntryCard key={entry.id} entry={entry} index={i} diary={diary} />
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

function DiaryEntryCard({ entry, index, diary }: { entry: DiaryEntry; index: number; diary: EcosystemDiary }) {
  const bg = EVENT_BG[entry.type] ?? 'bg-neutral-500/10 ring-white/10';
  const icon = EVENT_ICONS[entry.type] ?? <Sparkles size={14} className="text-neutral-400" />;

  return (
    <div className={`flex items-start gap-2.5 rounded-xl ${bg} p-2.5 ring-1`}>
      <div className="mt-0.5 shrink-0">{icon}</div>
      <div className="flex-1 min-w-0">
        <div className="flex items-center justify-between gap-2">
          <span className="text-[11px] font-semibold text-neutral-200">{diary.getEventLabel(entry.type)}</span>
          <span className="shrink-0 text-[9px] text-neutral-600">{formatTime(entry.wallTime)}</span>
        </div>
        <p className="mt-0.5 text-[11px] leading-snug text-neutral-400">{entry.description}</p>
        {entry.tick > 0 && (
          <span className="text-[9px] text-neutral-600">Tick {entry.tick}</span>
        )}
      </div>
    </div>
  );
}
