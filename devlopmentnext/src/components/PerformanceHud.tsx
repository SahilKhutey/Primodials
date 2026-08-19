import type { PerformanceSample } from "@/lib/performanceMonitor";

type Props = {
  sample: PerformanceSample | null;
  visible?: boolean;
};

export function PerformanceHud({ sample, visible = true }: Props) {
  if (!visible || !sample) return null;

  const fps = Math.round(sample.fps);
  const frame = sample.frameMs.toFixed(1);
  const steps = sample.simulationSteps.toFixed(1);
  const population = Math.round(sample.population);

  return (
    <div className="pointer-events-none absolute right-3 top-3 z-10 rounded-lg border border-white/10 bg-black/45 px-3 py-2 font-mono text-[10px] leading-4 text-white/75 backdrop-blur-md">
      <div>FPS&nbsp;&nbsp; {fps}</div>
      <div>FRAME {frame} ms</div>
      <div>SIM&nbsp;&nbsp; {steps}/s</div>
      <div>POP&nbsp;&nbsp; {population}</div>
    </div>
  );
}
