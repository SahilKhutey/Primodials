import type { PerformanceSample } from "@/lib/performanceMonitor";
import type { AdaptivePerformanceState } from "@/lib/adaptivePerformance";

type Props = {
  sample: PerformanceSample | null;
  adaptive?: AdaptivePerformanceState;
  watchdogUnhealthy?: boolean;
  visible?: boolean;
};

export function RuntimeHealthHud({
  sample,
  adaptive,
  watchdogUnhealthy = false,
  visible = false,
}: Props) {
  if (!visible) return null;

  return (
    <div className="pointer-events-none fixed bottom-3 right-3 z-40 min-w-44 rounded-xl border border-white/10 bg-black/50 px-3 py-2 font-mono text-[10px] leading-4 text-white/70 backdrop-blur-md">
      <div className="mb-1 font-sans text-[9px] font-semibold uppercase tracking-[0.18em] text-white/40">
        Runtime
      </div>
      <div className={watchdogUnhealthy ? "text-red-300" : "text-emerald-300"}>
        {watchdogUnhealthy ? "DEGRADED" : "HEALTHY"}
      </div>
      {sample && (
        <>
          <div>FPS&nbsp;&nbsp; {Math.round(sample.fps)}</div>
          <div>FRAME {sample.frameMs.toFixed(1)} ms</div>
          <div>POP&nbsp;&nbsp; {Math.round(sample.population)}</div>
        </>
      )}
      {adaptive && (
        <div>QUALITY {adaptive.quality.toUpperCase()}</div>
      )}
    </div>
  );
}
