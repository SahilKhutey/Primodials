type Props = {
  running: boolean;
  population: number;
  species: number;
  generation: number;
  quality: string;
};

export function WallpaperStatusBadge({
  running,
  population,
  species,
  generation,
  quality,
}: Props) {
  return (
    <div className="pointer-events-none fixed left-4 top-4 z-30 rounded-xl border border-white/10 bg-black/45 px-3 py-2 font-mono text-[10px] text-white/70 shadow-xl backdrop-blur-md">
      <div className="mb-1 flex items-center gap-2 font-sans text-[10px] font-semibold uppercase tracking-wider text-white/50">
        <span
          className={`h-1.5 w-1.5 rounded-full ${
            running ? "bg-emerald-400" : "bg-white/30"
          }`}
        />
        Primordials
      </div>
      <div>{population} organisms · {species} species</div>
      <div>generation {generation} · {quality}</div>
    </div>
  );
}
