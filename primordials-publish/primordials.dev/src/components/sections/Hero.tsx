'use client';

import { motion } from 'framer-motion';
import { Download, Play, Github } from 'lucide-react';
import { HeroVisual } from '@/components/sections/HeroVisual';

export function Hero() {
  return (
    <section className="relative overflow-hidden pt-12 sm:pt-20">
      {/* Background gradient */}
      <div aria-hidden="true" className="absolute inset-0 -z-10 bg-gradient-radial" />
      <div aria-hidden="true" className="absolute inset-0 -z-10 bg-dots opacity-50" />

      {/* Glow blobs */}
      <div aria-hidden="true" className="absolute -top-40 left-1/4 -z-10 h-96 w-96 rounded-full bg-accent-magenta/20 blur-[120px]" />
      <div aria-hidden="true" className="absolute -top-20 right-1/4 -z-10 h-96 w-96 rounded-full bg-biome-algae/15 blur-[120px]" />

      <div className="container-wide">
        <div className="grid gap-12 lg:grid-cols-2 lg:gap-20">
          {/* Copy */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.8 }}
            className="flex flex-col justify-center"
          >
            <div className="label-mono mb-6 inline-flex items-center gap-2 self-start rounded-full border border-bg-surface bg-bg-surface/50 px-3 py-1 backdrop-blur-sm">
              <span className="h-2 w-2 animate-pulse rounded-full bg-accent-magenta" />
              v2.1.0 · MIT licensed · Open source
            </div>

            <h1 className="h-display">
              A living{' '}
              <span className="text-gradient">ecosystem</span> <br />
              on your desktop.
            </h1>

            <p className="lead mt-6 max-w-xl">
              Real Lotka-Volterra predator-prey dynamics. Six biomes from algae bloom to volcanic vent. 60 FPS. Free forever.
            </p>

            <div className="mt-10 flex flex-col gap-3 sm:flex-row">
              <a
                href="https://gensoulslab.itch.io/polygonal-primordials"
                target="_blank"
                rel="noopener noreferrer"
                className="btn-primary"
              >
                <Play className="h-4 w-4" /> Play in Browser (itch.io)
              </a>
              <a
                href="https://github.com/SahilKhutey/Primodials/releases"
                target="_blank"
                rel="noopener noreferrer"
                className="btn-secondary"
              >
                <Download className="h-4 w-4" /> Standalone Desktop Installer
              </a>
            </div>

            <dl className="mt-12 grid grid-cols-3 gap-6 border-t border-bg-surface/40 pt-8 text-sm">
              <div>
                <dt className="text-text-muted">Entities</dt>
                <dd className="font-display text-2xl font-bold text-text-primary">
                  250K+
                </dd>
              </div>
              <div>
                <dt className="text-text-muted">Frame rate</dt>
                <dd className="font-display text-2xl font-bold text-text-primary">
                  60 FPS
                </dd>
              </div>
              <div>
                <dt className="text-text-muted">License</dt>
                <dd className="font-display text-2xl font-bold text-text-primary">
                  MIT
                </dd>
              </div>
            </dl>
          </motion.div>

          {/* Visual */}
          <motion.div
            initial={{ opacity: 0, scale: 0.95 }}
            animate={{ opacity: 1, scale: 1 }}
            transition={{ duration: 1, delay: 0.2 }}
            className="relative"
          >
            <HeroVisual />
          </motion.div>
        </div>
      </div>
    </section>
  );
}
