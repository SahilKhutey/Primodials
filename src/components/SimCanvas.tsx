import { useCallback, useEffect, useRef, useState } from 'react';
import { ZoomIn, ZoomOut, Maximize, Locate, Crosshair } from 'lucide-react';
import type { Simulation } from '@/sim/simulation';
import { render } from '@/sim/renderer';
import { defaultCamera, type Camera } from '@/sim/types';
import type { CinematicCamera } from '@/sim/cinematicCamera';

type Props = {
  sim: Simulation;
  running: boolean;
  showSense: boolean;
  selectedId: number | null;
  onSelect: (id: number | null) => void;
  showColonies: boolean;
  wallpaperMode?: boolean;
  cinematic?: CinematicCamera;
};

const MIN_ZOOM = 0.4;
const MAX_ZOOM = 8;

export function SimCanvas({ sim, running, showSense, selectedId, onSelect, showColonies, wallpaperMode = false, cinematic }: Props) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const minimapRef = useRef<HTMLCanvasElement>(null);
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
  const wallpaperRef = useRef(wallpaperMode);
  wallpaperRef.current = wallpaperMode;
  const cinematicRef = useRef(cinematic);
  cinematicRef.current = cinematic;

  const [camera, setCamera] = useState<Camera>(() => defaultCamera(sim.settings.worldWidth, sim.settings.worldHeight));
  const cameraRef = useRef(camera);
  cameraRef.current = camera;
  const [followSelected, setFollowSelected] = useState(false);
  const followRef = useRef(followSelected);
  followRef.current = followSelected;

  // Trail storage for wallpaper mode
  const trailsRef = useRef<Map<number, { x: number; y: number; hue: number }[]>>(new Map());

  // ── Coordinate helpers ────────────────────────────────────────────
  const screenToWorld = useCallback((sx: number, sy: number) => {
    const canvas = canvasRef.current;
    if (!canvas) return { x: 0, y: 0 };
    const baseScale = Math.min(canvas.clientWidth / sim.settings.worldWidth, canvas.clientHeight / sim.settings.worldHeight);
    const scale = baseScale * cameraRef.current.zoom;
    const wx = (sx - canvas.clientWidth / 2) / scale + cameraRef.current.x;
    const wy = (sy - canvas.clientHeight / 2) / scale + cameraRef.current.y;
    return { x: wx, y: wy };
  }, [sim.settings.worldWidth, sim.settings.worldHeight]);

  // ── Zoom helper ────────────────────────────────────────────────────
  const zoomAt = useCallback((factor: number, sx?: number, sy?: number) => {
    setCamera((cam) => {
      const canvas = canvasRef.current;
      if (!canvas) return cam;
      const cx = sx ?? canvas.clientWidth / 2;
      const cy = sy ?? canvas.clientHeight / 2;
      const baseScale = Math.min(canvas.clientWidth / sim.settings.worldWidth, canvas.clientHeight / sim.settings.worldHeight);
      const oldScale = baseScale * cam.zoom;
      const newZoom = Math.max(MIN_ZOOM, Math.min(MAX_ZOOM, cam.zoom * factor));
      const newScale = baseScale * newZoom;
      const wx = (cx - canvas.clientWidth / 2) / oldScale + cam.x;
      const wy = (cy - canvas.clientHeight / 2) / oldScale + cam.y;
      const nx = (cx - canvas.clientWidth / 2) / newScale + wx;
      const ny = (cy - canvas.clientHeight / 2) / newScale + wy;
      return { x: nx, y: ny, zoom: newZoom };
    });
  }, [sim.settings.worldWidth, sim.settings.worldHeight]);

  // ── Pan helper ─────────────────────────────────────────────────────
  const panBy = useCallback((dxScreen: number, dyScreen: number) => {
    setCamera((cam) => {
      const canvas = canvasRef.current;
      if (!canvas) return cam;
      const baseScale = Math.min(canvas.clientWidth / sim.settings.worldWidth, canvas.clientHeight / sim.settings.worldHeight);
      const scale = baseScale * cam.zoom;
      return { ...cam, x: cam.x - dxScreen / scale, y: cam.y - dyScreen / scale };
    });
  }, [sim.settings.worldWidth, sim.settings.worldHeight]);

  // ── Follow selected organism ───────────────────────────────────────
  useEffect(() => {
    if (!followSelected || selectedId === null) return;
    const id = setInterval(() => {
      const org = simRef.current.organisms.find((o) => o.id === selectedId && o.alive);
      if (org) {
        setCamera((cam) => ({ ...cam, x: org.x, y: org.y }));
      }
    }, 100);
    return () => clearInterval(id);
  }, [followSelected, selectedId]);

  // ── Keyboard controls (sandbox mode only) ──────────────────────────
  useEffect(() => {
    if (wallpaperMode) return;
    const onKey = (e: KeyboardEvent) => {
      const canvas = canvasRef.current;
      if (!canvas) return;
      const step = 80 / cameraRef.current.zoom;
      switch (e.key) {
        case 'ArrowUp': case 'w': case 'W': panBy(0, -step); break;
        case 'ArrowDown': case 's': case 'S': panBy(0, step); break;
        case 'ArrowLeft': case 'a': case 'A': panBy(-step, 0); break;
        case 'ArrowRight': case 'd': case 'D': panBy(step, 0); break;
        case '+': case '=': zoomAt(1.25); break;
        case '-': case '_': zoomAt(1 / 1.25); break;
        case '0': setCamera(defaultCamera(simRef.current.settings.worldWidth, simRef.current.settings.worldHeight)); break;
        case 'f': case 'F': setFollowSelected((v) => !v); break;
        default: return;
      }
      e.preventDefault();
    };
    window.addEventListener('keydown', onKey);
    return () => window.removeEventListener('keydown', onKey);
  }, [panBy, zoomAt, wallpaperMode]);

  // ── Main render loop ──────────────────────────────────────────────
  useEffect(() => {
    const canvas = canvasRef.current;
    const minimap = minimapRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;
    const mctx = minimap?.getContext('2d') ?? null;

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

      // Update cinematic camera
      if (wallpaperRef.current && cinematicRef.current) {
        cinematicRef.current.update(dt / 16.67);
      }

      // Update trails in wallpaper mode
      if (wallpaperRef.current) {
        const trails = trailsRef.current;
        for (const org of simRef.current.organisms) {
          if (!org.alive) continue;
          let trail = trails.get(org.id);
          if (!trail) {
            trail = [];
            trails.set(org.id, trail);
          }
          trail.push({ x: org.x, y: org.y, hue: org.genome.hue });
          if (trail.length > 12) trail.shift();
        }
        // Clean up dead organisms' trails
        for (const [id] of trails) {
          if (!simRef.current.organisms.find((o) => o.id === id && o.alive)) {
            trails.delete(id);
          }
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

      // Use cinematic camera in wallpaper mode, manual camera in sandbox
      const cam = wallpaperRef.current && cinematicRef.current
        ? cinematicRef.current.getCamera()
        : cameraRef.current;

      render(
        ctx, simRef.current, w, h,
        showSenseRef.current, selectedRef.current, showColoniesRef.current,
        phase, cam, wallpaperRef.current,
        wallpaperRef.current ? trailsRef.current : null,
      );

      // Minimap (sandbox mode only)
      if (mctx && minimap && !wallpaperRef.current) {
        drawMinimap(mctx, minimap, simRef.current, cameraRef.current);
      }

      raf = requestAnimationFrame(loop);
    };
    raf = requestAnimationFrame(loop);
    return () => cancelAnimationFrame(raf);
  }, []);

  // ── Mouse handlers ─────────────────────────────────────────────────
  const dragRef = useRef<{ x: number; y: number; moved: boolean } | null>(null);

  const handleMouseDown = (e: React.MouseEvent<HTMLCanvasElement>) => {
    dragRef.current = { x: e.clientX, y: e.clientY, moved: false };
  };

  const handleMouseMove = (e: React.MouseEvent<HTMLCanvasElement>) => {
    if (!dragRef.current) return;
    const dx = e.clientX - dragRef.current.x;
    const dy = e.clientY - dragRef.current.y;
    if (Math.abs(dx) > 3 || Math.abs(dy) > 3) dragRef.current.moved = true;
    if (dragRef.current.moved) {
      panBy(dx, dy);
      dragRef.current.x = e.clientX;
      dragRef.current.y = e.clientY;
    }
  };

  const handleMouseUp = (e: React.MouseEvent<HTMLCanvasElement>) => {
    const wasDrag = dragRef.current?.moved;
    dragRef.current = null;
    if (wasDrag) return;

    const rect = canvasRef.current!.getBoundingClientRect();
    const px = e.clientX - rect.left;
    const py = e.clientY - rect.top;
    const { x: wx, y: wy } = screenToWorld(px, py);

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

  const handleWheel = (e: React.WheelEvent<HTMLCanvasElement>) => {
    const rect = canvasRef.current!.getBoundingClientRect();
    const sx = e.clientX - rect.left;
    const sy = e.clientY - rect.top;
    const factor = e.deltaY < 0 ? 1.15 : 1 / 1.15;
    zoomAt(factor, sx, sy);
  };

  const resetView = () => setCamera(defaultCamera(sim.settings.worldWidth, sim.settings.worldHeight));

  // ── Minimap click ──────────────────────────────────────────────────
  const handleMinimapClick = (e: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = minimapRef.current;
    if (!canvas) return;
    const rect = canvas.getBoundingClientRect();
    const mx = (e.clientX - rect.left) / rect.width;
    const my = (e.clientY - rect.top) / rect.height;
    setCamera((cam) => ({
      ...cam,
      x: mx * sim.settings.worldWidth,
      y: my * sim.settings.worldHeight,
    }));
  };

  return (
    <div className="relative h-full w-full">
      <canvas
        ref={canvasRef}
        onMouseDown={handleMouseDown}
        onMouseMove={handleMouseMove}
        onMouseUp={handleMouseUp}
        onMouseLeave={() => { dragRef.current = null; }}
        onWheel={handleWheel}
        className={`h-full w-full ${
          wallpaperMode
            ? 'cursor-default'
            : 'cursor-grab rounded-2xl border border-neutral-800/60 shadow-2xl shadow-black/50 active:cursor-grabbing'
        }`}
      />

      {/* Zoom controls overlay (sandbox mode only) */}
      {!wallpaperMode && (
        <>
          <div className="absolute bottom-3 right-3 flex flex-col gap-1.5">
            <button
              onClick={() => zoomAt(1.3)}
              className="flex h-9 w-9 items-center justify-center rounded-lg bg-neutral-900/80 text-neutral-200 ring-1 ring-white/10 backdrop-blur-sm transition hover:bg-neutral-800 active:scale-90"
              title="Zoom in (+)"
            >
              <ZoomIn size={17} />
            </button>
            <button
              onClick={() => zoomAt(1 / 1.3)}
              className="flex h-9 w-9 items-center justify-center rounded-lg bg-neutral-900/80 text-neutral-200 ring-1 ring-white/10 backdrop-blur-sm transition hover:bg-neutral-800 active:scale-90"
              title="Zoom out (-)"
            >
              <ZoomOut size={17} />
            </button>
            <button
              onClick={resetView}
              className="flex h-9 w-9 items-center justify-center rounded-lg bg-neutral-900/80 text-neutral-200 ring-1 ring-white/10 backdrop-blur-sm transition hover:bg-neutral-800 active:scale-90"
              title="Reset view (0)"
            >
              <Maximize size={16} />
            </button>
            <button
              onClick={() => setFollowSelected((v) => !v)}
              className={`flex h-9 w-9 items-center justify-center rounded-lg ring-1 backdrop-blur-sm transition active:scale-90 ${
                followSelected
                  ? 'bg-cyan-500/30 text-cyan-200 ring-cyan-400/40'
                  : 'bg-neutral-900/80 text-neutral-200 ring-white/10 hover:bg-neutral-800'
              }`}
              title="Follow selected (F)"
            >
              <Locate size={16} />
            </button>
          </div>

          {/* Minimap overlay */}
          <div className="absolute bottom-3 left-3 overflow-hidden rounded-lg border border-neutral-700/60 bg-neutral-950/70 shadow-lg backdrop-blur-sm">
            <canvas
              ref={minimapRef}
              onClick={handleMinimapClick}
              className="block cursor-pointer"
              style={{ width: 140, height: 105 }}
            />
            <div className="absolute left-1.5 top-1 flex items-center gap-1 text-[10px] font-semibold uppercase tracking-wider text-neutral-400">
              <Crosshair size={9} />
              Map
            </div>
          </div>

          {/* Zoom indicator */}
          <div className="absolute right-3 top-3 rounded-lg bg-neutral-950/70 px-2.5 py-1 text-xs font-mono text-neutral-300 ring-1 ring-white/10 backdrop-blur-sm">
            {camera.zoom.toFixed(2)}x
          </div>

          {/* Keyboard hint */}
          <div className="absolute bottom-3 left-1/2 -translate-x-1/2 rounded-lg bg-neutral-950/60 px-3 py-1 text-[10px] text-neutral-500 ring-1 ring-white/5 backdrop-blur-sm">
            Scroll to zoom · Drag to pan · WASD/arrows to move · F to follow · 0 to reset
          </div>
        </>
      )}

      {/* Wallpaper mode subtle indicator */}
      {wallpaperMode && (
        <div className="pointer-events-none absolute right-4 top-4 flex items-center gap-2 rounded-full bg-neutral-950/40 px-3 py-1 text-[10px] font-medium text-neutral-500 ring-1 ring-white/5 backdrop-blur-sm">
          <span className="relative flex h-1.5 w-1.5">
            <span className="absolute inline-flex h-full w-full animate-ping rounded-full bg-cyan-400 opacity-75" />
            <span className="relative inline-flex h-1.5 w-1.5 rounded-full bg-cyan-400" />
          </span>
          Cinematic
        </div>
      )}
    </div>
  );
}

// ── Minimap renderer ──────────────────────────────────────────────────
function drawMinimap(
  ctx: CanvasRenderingContext2D,
  canvas: HTMLCanvasElement,
  sim: Simulation,
  camera: Camera,
) {
  const dpr = Math.min(window.devicePixelRatio || 1, 2);
  const w = canvas.clientWidth;
  const h = canvas.clientHeight;
  if (canvas.width !== w * dpr || canvas.height !== h * dpr) {
    canvas.width = w * dpr;
    canvas.height = h * dpr;
  }
  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);

  ctx.fillStyle = '#0a0a0e';
  ctx.fillRect(0, 0, w, h);

  const sx = w / sim.settings.worldWidth;
  const sy = h / sim.settings.worldHeight;

  // Biomes
  for (const b of sim.biomes) {
    ctx.fillStyle = `hsla(${b.hue}, 40%, 25%, 0.5)`;
    ctx.beginPath();
    ctx.arc(b.cx * sx, b.cy * sy, b.radius * sx, 0, Math.PI * 2);
    ctx.fill();
  }

  // Structures
  for (const s of sim.structures) {
    ctx.fillStyle = `hsla(${s.hue}, 60%, 60%, 0.8)`;
    ctx.fillRect(s.x * sx - 1, s.y * sy - 1, 2, 2);
  }

  // Organisms
  for (const org of sim.organisms) {
    if (!org.alive) continue;
    ctx.fillStyle = `hsl(${org.genome.hue}, 70%, 55%)`;
    ctx.fillRect(org.x * sx, org.y * sy, 1.2, 1.2);
  }

  // Viewport rectangle
  const baseScale = Math.min(w / sim.settings.worldWidth, h / sim.settings.worldHeight);
  const viewW = (w / (baseScale * camera.zoom)) * sx;
  const viewH = (h / (baseScale * camera.zoom)) * sy;
  const viewX = camera.x * sx - viewW / 2;
  const viewY = camera.y * sy - viewH / 2;
  ctx.strokeStyle = 'rgba(34, 211, 238, 0.8)';
  ctx.lineWidth = 1;
  ctx.strokeRect(viewX, viewY, viewW, viewH);

  // Camera center dot
  ctx.fillStyle = '#22d3ee';
  ctx.beginPath();
  ctx.arc(camera.x * sx, camera.y * sy, 1.5, 0, Math.PI * 2);
  ctx.fill();
}
