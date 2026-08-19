'use client';

import { motion } from 'framer-motion';
import { useEffect, useState } from 'react';

export function HeroVisual() {
  const [preys, setPreys] = useState<Array<{ x: number; y: number; delay: number }>>([]);
  const [predators, setPredators] = useState<Array<{ x: number; y: number; delay: number }>>([]);

  useEffect(() => {
    setPreys(
      Array.from({ length: 40 }, () => ({
        x: Math.random() * 90 + 5,
        y: Math.random() * 90 + 5,
        delay: Math.random() * 2,
      }))
    );
    setPredators(
      Array.from({ length: 6 }, () => ({
        x: Math.random() * 90 + 5,
        y: Math.random() * 90 + 5,
        delay: Math.random() * 2,
      }))
    );
  }, []);

  return (
    <div className="relative aspect-square w-full overflow-hidden rounded-2xl border border-bg-surface/60 bg-bg-deep shadow-glow-magenta">
      {/* Animated ecosystem */}
      <div className="absolute inset-0 bg-gradient-to-br from-bg-deep via-bg-surface to-bg-void opacity-90" />

      {/* Prey (circles) */}
      {preys.map((p, i) => (
        <motion.div
          key={`prey-${i}`}
          className="absolute h-2.5 w-2.5 rounded-full bg-biome-algae shadow-[0_0_8px_#4ECDC4]"
          style={{ left: `${p.x}%`, top: `${p.y}%` }}
          animate={{
            x: [0, 20, -10, 0],
            y: [0, -15, 10, 0],
          }}
          transition={{
            duration: 8 + p.delay,
            repeat: Infinity,
            ease: 'easeInOut',
            delay: p.delay,
          }}
        />
      ))}

      {/* Predators (triangles) */}
      {predators.map((p, i) => (
        <motion.div
          key={`pred-${i}`}
          className="absolute h-3.5 w-3.5 rotate-45 bg-accent-magenta shadow-[0_0_10px_#FF4FA3]"
          style={{
            left: `${p.x}%`,
            top: `${p.y}%`,
            clipPath: 'polygon(50% 0%, 0% 100%, 100% 100%)',
          }}
          animate={{
            x: [0, 30, -20, 0],
            y: [0, -25, 15, 0],
          }}
          transition={{
            duration: 12 + p.delay,
            repeat: Infinity,
            ease: 'easeInOut',
            delay: p.delay,
          }}
        />
      ))}

      {/* Overlay UI hint */}
      <div className="absolute bottom-4 left-4 right-4 flex items-center justify-between rounded-lg border border-bg-surface/40 bg-bg-void/60 px-4 py-2 backdrop-blur-md">
        <div className="flex items-center gap-2 font-mono text-xs text-text-muted">
          <span className="h-2 w-2 rounded-full bg-biome-algae" />
          <span>prey: 800</span>
          <span className="ml-2 h-2 w-2 rounded-full bg-accent-magenta" />
          <span>predators: 25</span>
        </div>
        <div className="font-mono text-xs font-bold text-biome-algae">60 FPS LOCKED</div>
      </div>

      {/* Caption */}
      <div className="absolute right-4 top-4 rounded-md bg-bg-void/60 px-2.5 py-1 font-mono text-xs text-text-muted backdrop-blur-md">
        algae-bloom.json
      </div>
    </div>
  );
}
