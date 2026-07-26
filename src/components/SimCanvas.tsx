import { useEffect, useRef } from 'react';
import type { Simulation } from '@/sim/simulation';
import { render } from '@/sim/renderer';

type Props = {
  sim: Simulation;
  running: boolean;
  showSense: boolean;
  selectedId: number | null;
  onSelect: (id: number | null) => void;
  showColonies: boolean;
};

export function SimCanvas({ sim, running, showSense, selectedId, onSelect, showColonies }: Props) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const simRef = useRef(sim);
  simRef.current = sim;
  const selectedRef = useRef(selectedId);
  selectedRef.current = selectedId;
  const showColoniesRef = useRef(showColonies);
  showColoniesRef.current = showColonies;
  const showSenseRef = useRef(showSense);
  showSenseRef.current = showSense;
  const runningRef = useRef(running);
  runningRef.current = running;

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    let raf = 0;
    let acc = 0;
    let last = performance.now();
    let phase = 0;
    const STEP_MS = 1000 / 30;

    const loop = (now: number) => {
      const dt = now - last;
      last = now;
      phase += dt * 0.002;

      if (runningRef.current) {
        acc += dt;
        while (acc >= STEP_MS) {
          simRef.current.step();
          acc -= STEP_MS;
        }
      }

      const dpr = Math.min(window.devicePixelRatio || 1, 2);
      const w = canvas.clientWidth;
      const h = canvas.clientHeight;
      if (canvas.width !== w * dpr || canvas.height !== h * dpr) {
        canvas.width = w * dpr;
        canvas.height = h * dpr;
      }
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      render(ctx, simRef.current, w, h, showSenseRef.current, selectedRef.current, showColoniesRef.current, phase);
      raf = requestAnimationFrame(loop);
    };
    raf = requestAnimationFrame(loop);
    return () => cancelAnimationFrame(raf);
  }, []);

  const handleClick = (e: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const rect = canvas.getBoundingClientRect();
    const px = e.clientX - rect.left;
    const py = e.clientY - rect.top;
    const scaleX = sim.settings.worldWidth / canvas.clientWidth;
    const scaleY = sim.settings.worldHeight / canvas.clientHeight;
    const wx = px * scaleX;
    const wy = py * scaleY;

    let nearest: number | null = null;
    let nearestDist = Infinity;
    for (const org of sim.organisms) {
      if (!org.alive) continue;
      const dx = org.x - wx;
      const dy = org.y - wy;
      const d2 = dx * dx + dy * dy;
      const radius = (org.genome.size + 6) * (org.genome.size + 6);
      if (d2 < radius && d2 < nearestDist) {
        nearestDist = d2;
        nearest = org.id;
      }
    }
    onSelect(nearest);
  };

  return (
    <canvas
      ref={canvasRef}
      onClick={handleClick}
      className="w-full h-full cursor-crosshair rounded-2xl border border-neutral-800/60 shadow-2xl shadow-black/50"
    />
  );
}
