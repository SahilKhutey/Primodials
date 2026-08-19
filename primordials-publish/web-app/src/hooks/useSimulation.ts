import { useEffect, useRef } from 'react';
import { Simulation } from '@/engine/Simulation';
import { Renderer } from '@/engine/Renderer';
import { useSimulationStore } from '@/store/simulationStore';

export function useSimulation(canvasRef: React.RefObject<HTMLCanvasElement>) {
  const simRef = useRef<Simulation | null>(null);
  const rendererRef = useRef<Renderer | null>(null);
  const animationRef = useRef<number | null>(null);
  const lastTimeRef = useRef<number>(0);
  const { currentBiome, seed, parameters, setStats, isRunning } = useSimulationStore();

  useEffect(() => {
    if (!currentBiome || !canvasRef.current) return;
    const canvas = canvasRef.current;
    const renderer = new Renderer(canvas);
    const sim = new Simulation(currentBiome, parameters, seed);
    simRef.current = sim;
    rendererRef.current = renderer;

    return () => {
      simRef.current = null;
      rendererRef.current = null;
    };
  }, [currentBiome, seed, canvasRef]);

  useEffect(() => {
    if (simRef.current) {
      simRef.current.setParameters(parameters);
    }
  }, [parameters]);

  useEffect(() => {
    if (!isRunning) return;

    const animate = (timestamp: number) => {
      const sim = simRef.current;
      const renderer = rendererRef.current;

      if (!sim || !renderer) {
        animationRef.current = requestAnimationFrame(animate);
        return;
      }

      const dt =
        lastTimeRef.current === 0
          ? 1 / 60
          : Math.min((timestamp - lastTimeRef.current) / 1000, 1 / 30);
      lastTimeRef.current = timestamp;

      sim.step(dt);

      const entities = sim.getEntities();
      const biome = sim.getBiome();
      renderer.clear(biome.visuals.background, biome.visuals.gradient);
      renderer.drawEntities(entities);

      if (sim.getStats().ticks % 10 === 0) {
        setStats(sim.getStats());
      }

      animationRef.current = requestAnimationFrame(animate);
    };

    lastTimeRef.current = 0;
    animationRef.current = requestAnimationFrame(animate);

    return () => {
      if (animationRef.current !== null) {
        cancelAnimationFrame(animationRef.current);
      }
    };
  }, [isRunning, setStats]);
}
