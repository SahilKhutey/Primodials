/**
 * Primordials — Live Simulation Preview Canvas (React 18)
 *
 * Runs the active biome in real time during authoring.
 *
 * License: MIT
 */

import React, { useEffect, useRef } from 'react';
import type { Biome } from '../../web-app/src/types/biome';
import { Simulation } from '../../web-app/src/engine/Simulation';
import { Renderer } from '../../web-app/src/engine/Renderer';

interface LivePreviewProps {
  biome: Biome;
}

export function LivePreview({ biome }: LivePreviewProps) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const simRef = useRef<Simulation | null>(null);
  const rendererRef = useRef<Renderer | null>(null);
  const animRef = useRef<number | null>(null);

  useEffect(() => {
    if (!canvasRef.current) return;
    const canvas = canvasRef.current;
    const renderer = new Renderer(canvas);
    const sim = new Simulation(biome, {
      preyReproductionRate: 1.0,
      predatorHuntSuccess: 1.0,
      carryingCapacity: biome.rules.carryingCapacity,
      speedMultiplier: 1.0,
      paused: false,
    }, 1337);

    simRef.current = sim;
    rendererRef.current = renderer;

    let lastTime = performance.now();

    const loop = (time: number) => {
      const dt = Math.min((time - lastTime) / 1000, 1 / 30);
      lastTime = time;

      sim.step(dt);
      renderer.clear(biome.visuals.background, biome.visuals.gradient);
      renderer.drawEntities(sim.getEntities());

      animRef.current = requestAnimationFrame(loop);
    };

    animRef.current = requestAnimationFrame(loop);

    return () => {
      if (animRef.current) cancelAnimationFrame(animRef.current);
    };
  }, [biome]);

  return (
    <div className="absolute inset-0 bg-bg-void">
      <canvas ref={canvasRef} className="block h-full w-full" />
    </div>
  );
}
