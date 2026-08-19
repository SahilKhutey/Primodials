import { useEffect, useRef } from 'react';
import { useSimulation } from '@/hooks/useSimulation';
import { useSimulationStore } from '@/store/simulationStore';
import { useResize } from '@/hooks/useResize';

export function SimulationCanvas() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const containerRef = useRef<HTMLDivElement>(null);
  const { setIsRunning } = useSimulationStore();

  useSimulation(canvasRef);

  useEffect(() => {
    setIsRunning(true);
  }, [setIsRunning]);

  useResize(containerRef, () => {
    // Dynamic resize triggers re-render automatically
  });

  return (
    <div ref={containerRef} className="absolute inset-0">
      <canvas
        ref={canvasRef}
        className="block h-full w-full"
        style={{ imageRendering: 'auto' }}
      />
    </div>
  );
}
