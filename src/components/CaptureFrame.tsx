import type { Simulation } from "@/sim/simulation";
import type { WallpaperTheme } from "@/sim/themes";

type Props = { sim: Simulation; theme: WallpaperTheme; visible: boolean };

export function CaptureFrame({ sim, theme, visible }: Props) {
  if (!visible) return null;

  return (
    <div className="pointer-events-none fixed inset-x-0 bottom-8 z-[60] flex justify-center">
      <div className="min-w-[320px] max-w-[min(80vw,720px)] rounded-2xl border border-white/10 bg-black/45 px-5 py-4 text-center shadow-2xl backdrop-blur-xl">
        <div className="text-[10px] font-semibold uppercase tracking-[0.28em] text-white/40">
          {theme.displayName}
        </div>
        <div className="mt-1 text-lg font-semibold text-white drop-shadow-lg">
          Generation {sim.stats.maxGeneration}
        </div>
        <div className="mt-2 flex justify-center gap-5 text-[10px] font-mono text-white/55">
          <span>{sim.population} organisms</span>
          <span>{sim.species.length} species</span>
          <span>cycle {sim.stats.generationCycle}</span>
        </div>
      </div>
    </div>
  );
}
