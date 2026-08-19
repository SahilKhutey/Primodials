import { useEffect, useState } from 'react';
import { SimulationCanvas } from '@/components/SimulationCanvas';
import { BiomeSelector } from '@/components/BiomeSelector';
import { ParameterPanel } from '@/components/ParameterPanel';
import { StatsOverlay } from '@/components/StatsOverlay';
import { LoadingScreen } from '@/components/LoadingScreen';
import { WallpaperFrame } from '@/components/WallpaperFrame';
import { useSimulationStore } from '@/store/simulationStore';
import { useKeyboard } from '@/hooks/useKeyboard';
import { cn } from '@/utils/cn';
import { initializeAnalytics } from '@/services/analytics';
import { isWallpaperMode, isEmbedMode } from '@/utils/mode';

export default function App() {
  const [isLoaded, setIsLoaded] = useState(false);
  const { biomes, loadBiome } = useSimulationStore();
  useKeyboard();

  const wallpaperMode = isWallpaperMode();
  const embedMode = isEmbedMode();

  useEffect(() => {
    initializeAnalytics();

    const defaultId =
      new URLSearchParams(window.location.search).get('biome') ??
      import.meta.env.VITE_DEFAULT_BIOME ??
      'algae-bloom';

    if (biomes.some((b) => b.id === defaultId)) {
      loadBiome(defaultId);
    }

    if (wallpaperMode) {
      const timer = setTimeout(() => setIsLoaded(true), 1000);
      return () => clearTimeout(timer);
    }
    setIsLoaded(true);
  }, [biomes, loadBiome, wallpaperMode]);

  if (!isLoaded) {
    return <LoadingScreen />;
  }

  if (wallpaperMode && !embedMode) {
    return (
      <WallpaperFrame>
        <SimulationCanvas />
        <StatsOverlay minimal />
      </WallpaperFrame>
    );
  }

  return (
    <div
      className={cn(
        'relative flex h-screen w-screen overflow-hidden',
        'bg-bg-void text-text-primary'
      )}
    >
      {/* Left sidebar: Biome selector */}
      <aside className="z-10 flex w-64 flex-col border-r border-bg-surface/40 bg-bg-deep/80 backdrop-blur-md">
        <header className="flex items-center gap-3 border-b border-bg-surface/40 p-4">
          <div className="h-8 w-8 rounded-md bg-bg-surface" />
          <div>
            <h1 className="font-bold tracking-tight">primordials.</h1>
            <p className="font-mono text-xs text-text-muted">
              v{import.meta.env.VITE_APP_VERSION ?? '2.1.0'}
            </p>
          </div>
        </header>
        <BiomeSelector />
        <footer className="border-t border-bg-surface/40 p-4 font-mono text-xs text-text-muted">
          <a
            href="https://github.com/SahilKhutey/Primodials"
            target="_blank"
            rel="noopener noreferrer"
            className="hover:text-accent-magenta"
          >
            GitHub ↗
          </a>
        </footer>
      </aside>

      {/* Main canvas */}
      <main className="relative flex-1">
        <SimulationCanvas />
        <StatsOverlay />
      </main>

      {/* Right panel: Parameters */}
      <aside className="z-10 flex w-72 flex-col border-l border-bg-surface/40 bg-bg-deep/80 backdrop-blur-md">
        <ParameterPanel />
      </aside>
    </div>
  );
}
