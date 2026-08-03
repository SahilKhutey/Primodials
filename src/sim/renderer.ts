import type { Simulation } from './simulation';
import type { Organism, Structure, Biome, Remains, KnowledgeNode, StructureType, Camera } from './types';
import { BIOME_INFO } from './types';
import type { BiofilmCluster } from './microbial';

function hsl(h: number, s: number, l: number, a = 1): string {
  return `hsla(${h}, ${s}%, ${l}%, ${a})`;
}

function drawPolygon(
  ctx: CanvasRenderingContext2D,
  x: number,
  y: number,
  radius: number,
  sides: number,
  rotation: number,
) {
  ctx.beginPath();
  for (let i = 0; i < sides; i++) {
    const a = rotation + (i / sides) * Math.PI * 2;
    const px = x + Math.cos(a) * radius;
    const py = y + Math.sin(a) * radius;
    if (i === 0) ctx.moveTo(px, py);
    else ctx.lineTo(px, py);
  }
  ctx.closePath();
}

export function render(
  ctx: CanvasRenderingContext2D,
  sim: Simulation,
  width: number,
  height: number,
  showSense = false,
  selectedId: number | null = null,
  showColonies = true,
  phase = 0,
  camera: Camera | null = null,
  wallpaperMode = false,
  trails: Map<number, { x: number; y: number; hue: number }[]> | null = null,
) {
  // Base scale that fits the whole world in the viewport (zoom === 1)
  const baseScale = Math.min(width / sim.settings.worldWidth, height / sim.settings.worldHeight);
  const cam = camera ?? { x: sim.settings.worldWidth / 2, y: sim.settings.worldHeight / 2, zoom: 1 };
  const scale = baseScale * cam.zoom;

  // ── Atmospheric background ──────────────────────────────────────────
  if (wallpaperMode) {
    // Deeper, more cinematic background for wallpaper mode
    const grad = ctx.createRadialGradient(width * 0.5, height * 0.45, 0, width * 0.5, height * 0.6, width * 0.8);
    grad.addColorStop(0, '#0d0d14');
    grad.addColorStop(0.5, '#08080d');
    grad.addColorStop(1, '#040406');
    ctx.fillStyle = grad;
    ctx.fillRect(0, 0, width, height);
  } else {
    const grad = ctx.createRadialGradient(width * 0.5, height * 0.45, 0, width * 0.5, height * 0.5, width * 0.75);
    grad.addColorStop(0, '#141418');
    grad.addColorStop(0.6, '#0e0e12');
    grad.addColorStop(1, '#08080a');
    ctx.fillStyle = grad;
    ctx.fillRect(0, 0, width, height);
  }

  ctx.save();
  // Map: screen = (world - cam) * scale + screenCenter
  ctx.translate(width / 2, height / 2);
  ctx.scale(scale, scale);
  ctx.translate(-cam.x, -cam.y);

  // ── World boundary ───────────────────────────────────────────────────
  drawBoundary(ctx, sim, phase);

  // ── Biomes ───────────────────────────────────────────────────────────
  for (const b of sim.biomes) {
    drawBiome(ctx, b, phase);
  }

  // ── Chemical field (chemotaxis gradients) ────────────────────────────
  if (sim.chemicalField && sim.settings.chemicalField) {
    drawChemicalField(ctx, sim, phase);
  }

  // Subtle grid
  ctx.strokeStyle = 'rgba(255,255,255,0.018)';
  ctx.lineWidth = 1;
  const gridSize = 80;
  for (let x = 0; x <= sim.settings.worldWidth; x += gridSize) {
    ctx.beginPath();
    ctx.moveTo(x, 0);
    ctx.lineTo(x, sim.settings.worldHeight);
    ctx.stroke();
  }
  for (let y = 0; y <= sim.settings.worldHeight; y += gridSize) {
    ctx.beginPath();
    ctx.moveTo(0, y);
    ctx.lineTo(sim.settings.worldWidth, y);
    ctx.stroke();
  }

  // ── Colonies ─────────────────────────────────────────────────────────
  if (showColonies && sim.colonies.length > 0) {
    for (const colony of sim.colonies) {
      const radius = sim.settings.colonyRadius;
      const pulse = 0.04 + Math.sin(phase * 0.5 + colony.id) * 0.015;
      const g = ctx.createRadialGradient(colony.centerX, colony.centerY, 0, colony.centerX, colony.centerY, radius);
      g.addColorStop(0, hsl(colony.hue, 45, 35, pulse * 1.5));
      g.addColorStop(1, hsl(colony.hue, 45, 35, 0));
      ctx.fillStyle = g;
      ctx.beginPath();
      ctx.arc(colony.centerX, colony.centerY, radius, 0, Math.PI * 2);
      ctx.fill();

      ctx.strokeStyle = hsl(colony.hue, 35, 45, 0.08);
      ctx.lineWidth = 1;
      ctx.setLineDash([6, 4]);
      ctx.beginPath();
      ctx.arc(colony.centerX, colony.centerY, radius, 0, Math.PI * 2);
      ctx.stroke();
      ctx.setLineDash([]);
    }

    const colonyLeaders = new Map<number, Organism>();
    for (const org of sim.organisms) {
      if (org.alive && org.colonyRole === 'leader') colonyLeaders.set(org.colonyId!, org);
    }
    for (const org of sim.organisms) {
      if (!org.alive || org.colonyId === null || org.colonyRole !== 'member') continue;
      const leader = colonyLeaders.get(org.colonyId);
      if (!leader) continue;
      ctx.strokeStyle = hsl(org.genome.hue, 30, 50, 0.1);
      ctx.lineWidth = 0.8;
      ctx.beginPath();
      ctx.moveTo(org.x, org.y);
      ctx.lineTo(leader.x, leader.y);
      ctx.stroke();
    }
  }

  // ── Knowledge nodes ──────────────────────────────────────────────────
  for (const kn of sim.knowledgeNodes) {
    drawKnowledgeNode(ctx, kn, phase);
  }

  // ── Biofilm clusters ──────────────────────────────────────────────────
  if (sim.settings.microbialBehavior && sim.biofilms.length > 0) {
    for (const bf of sim.biofilms) {
      drawBiofilm(ctx, bf, phase);
    }
  }

  // ── Structures ──────────────────────────────────────────────────────
  for (const s of sim.structures) {
    drawStructure(ctx, sim, s, phase);
  }

  // ── Remains (dead organic matter) ────────────────────────────────────
  for (const r of sim.remains) {
    drawRemains(ctx, r, phase);
  }

  // ── Food ────────────────────────────────────────────────────────────
  for (const f of sim.food) {
    if (f.energy <= 0) continue;
    const sparkle = 0.5 + Math.sin(phase * 2 + f.id * 0.3) * 0.15;
    ctx.shadowBlur = 4;
    ctx.shadowColor = `rgba(160, 220, 160, ${sparkle * 0.4})`;
    ctx.fillStyle = `rgba(170, 230, 170, ${0.45 + sparkle * 0.15})`;
    ctx.beginPath();
    ctx.arc(f.x, f.y, 2.5, 0, Math.PI * 2);
    ctx.fill();
    ctx.shadowBlur = 0;
  }

  // ── Organism trails (wallpaper mode) ────────────────────────────────
  if (wallpaperMode && trails) {
    for (const [, points] of trails) {
      if (points.length < 2) continue;
      for (let i = 1; i < points.length; i++) {
        const p = points[i];
        const prev = points[i - 1];
        const alpha = (i / points.length) * 0.12;
        ctx.strokeStyle = `hsla(${p.hue}, 60%, 55%, ${alpha})`;
        ctx.lineWidth = 0.8;
        ctx.beginPath();
        ctx.moveTo(prev.x, prev.y);
        ctx.lineTo(p.x, p.y);
        ctx.stroke();
      }
    }
  }

  // ── Organisms ───────────────────────────────────────────────────────
  for (const org of sim.organisms) {
    if (!org.alive) continue;
    drawOrganism(ctx, org, showSense, org.id === selectedId, phase);
  }

  // ── Neural brain glow for selected organism ──────────────────────────
  if (selectedId !== null) {
    const sel = sim.organisms.find((o) => o.id === selectedId && o.alive);
    if (sel && sel.brain && sel.lastOutputs) {
      const activity = Math.hypot(sel.lastOutputs[0], sel.lastOutputs[1]);
      const pulse = 0.5 + Math.sin(phase * 3) * 0.5;
      const r = sel.genome.size + 8 + pulse * 4;
      ctx.strokeStyle = `rgba(217, 70, 239, ${0.3 + activity * 0.3})`;
      ctx.lineWidth = 1.5;
      ctx.setLineDash([3, 3]);
      ctx.beginPath();
      ctx.arc(sel.x, sel.y, r, 0, Math.PI * 2);
      ctx.stroke();
      ctx.setLineDash([]);
    }
  }

  ctx.restore();

  // ── Ambient floating particles (wallpaper mode) ─────────────────────
  if (wallpaperMode) {
    drawAmbientParticles(ctx, width, height, phase);
  }

  // ── Vignette (wallpaper mode) ────────────────────────────────────────
  if (wallpaperMode) {
    const vg = ctx.createRadialGradient(width / 2, height / 2, width * 0.3, width / 2, height / 2, width * 0.75);
    vg.addColorStop(0, 'rgba(0,0,0,0)');
    vg.addColorStop(1, 'rgba(0,0,0,0.45)');
    ctx.fillStyle = vg;
    ctx.fillRect(0, 0, width, height);
  }
}

function drawBoundary(ctx: CanvasRenderingContext2D, sim: Simulation, phase: number) {
  const w = sim.settings.worldWidth;
  const h = sim.settings.worldHeight;
  const mode = sim.settings.boundaryMode;
  const pulse = 0.5 + Math.sin(phase * 0.5) * 0.5;

  if (mode === 'wrap') {
    // Subtle dashed line indicating wrap-around
    ctx.strokeStyle = `rgba(120, 140, 180, ${0.08 + pulse * 0.03})`;
    ctx.lineWidth = 1.5;
    ctx.setLineDash([8, 6]);
    ctx.strokeRect(0, 0, w, h);
    ctx.setLineDash([]);
  } else if (mode === 'reflect') {
    // Glowing solid border
    ctx.shadowBlur = 8;
    ctx.shadowColor = `rgba(100, 180, 255, ${0.3 + pulse * 0.15})`;
    ctx.strokeStyle = `rgba(100, 180, 255, ${0.25 + pulse * 0.1})`;
    ctx.lineWidth = 2.5;
    ctx.strokeRect(0, 0, w, h);
    ctx.shadowBlur = 0;
  } else {
    // open: strong border with corner markers
    ctx.strokeStyle = `rgba(255, 150, 80, ${0.3 + pulse * 0.1})`;
    ctx.lineWidth = 3;
    ctx.strokeRect(0, 0, w, h);
    // Corner brackets
    const cl = 20;
    ctx.strokeStyle = `rgba(255, 150, 80, ${0.5 + pulse * 0.2})`;
    ctx.lineWidth = 3;
    ctx.beginPath();
    // top-left
    ctx.moveTo(0, cl); ctx.lineTo(0, 0); ctx.lineTo(cl, 0);
    // top-right
    ctx.moveTo(w - cl, 0); ctx.lineTo(w, 0); ctx.lineTo(w, cl);
    // bottom-right
    ctx.moveTo(w, h - cl); ctx.lineTo(w, h); ctx.lineTo(w - cl, h);
    // bottom-left
    ctx.moveTo(cl, h); ctx.lineTo(0, h); ctx.lineTo(0, h - cl);
    ctx.stroke();
  }
}

function drawBiome(ctx: CanvasRenderingContext2D, b: Biome, phase: number) {
  const info = BIOME_INFO[b.type];
  const pulse = 0.5 + Math.sin(phase * 0.3 + b.id) * 0.1;

  // Soft radial fill
  const g = ctx.createRadialGradient(b.cx, b.cy, 0, b.cx, b.cy, b.radius);
  g.addColorStop(0, hsl(b.hue, 35, 18, 0.35 + pulse * 0.05));
  g.addColorStop(0.7, hsl(b.hue, 30, 15, 0.15));
  g.addColorStop(1, hsl(b.hue, 30, 12, 0));
  ctx.fillStyle = g;
  ctx.beginPath();
  ctx.arc(b.cx, b.cy, b.radius, 0, Math.PI * 2);
  ctx.fill();

  // Dashed border
  ctx.strokeStyle = hsl(b.hue, 25, 30, 0.12);
  ctx.lineWidth = 1.5;
  ctx.setLineDash([12, 8]);
  ctx.beginPath();
  ctx.arc(b.cx, b.cy, b.radius, 0, Math.PI * 2);
  ctx.stroke();
  ctx.setLineDash([]);

  // Biome icon / texture dots
  const dotCount = Math.floor(b.radius / 30);
  for (let i = 0; i < dotCount; i++) {
    const a = (i / dotCount) * Math.PI * 2 + phase * 0.05;
    const r = b.radius * (0.3 + (i % 3) * 0.2);
    const dx = b.cx + Math.cos(a) * r;
    const dy = b.cy + Math.sin(a) * r;
    ctx.fillStyle = hsl(b.hue, 20, 35, 0.08);
    ctx.beginPath();
    ctx.arc(dx, dy, 2, 0, Math.PI * 2);
    ctx.fill();
  }

  // Label
  ctx.fillStyle = hsl(b.hue, 20, 55, 0.25);
  ctx.font = '10px Inter, sans-serif';
  ctx.textAlign = 'center';
  ctx.fillText(info.label, b.cx, b.cy - b.radius - 4);
}

function drawKnowledgeNode(ctx: CanvasRenderingContext2D, kn: KnowledgeNode, phase: number) {
  if (kn.discovered) {
    // Faint remnant showing it was found
    ctx.strokeStyle = `rgba(180, 200, 255, 0.08)`;
    ctx.lineWidth = 1;
    ctx.setLineDash([2, 4]);
    ctx.beginPath();
    ctx.arc(kn.x, kn.y, 6, 0, Math.PI * 2);
    ctx.stroke();
    ctx.setLineDash([]);
    return;
  }

  const pulse = 0.5 + Math.sin(phase * 1.5 + kn.id) * 0.5;
  const float = Math.sin(phase + kn.id) * 2;

  // Outer glow
  const g = ctx.createRadialGradient(kn.x, kn.y + float, 0, kn.x, kn.y + float, 20);
  g.addColorStop(0, `rgba(180, 200, 255, ${0.15 + pulse * 0.1})`);
  g.addColorStop(1, `rgba(180, 200, 255, 0)`);
  ctx.fillStyle = g;
  ctx.beginPath();
  ctx.arc(kn.x, kn.y + float, 20, 0, Math.PI * 2);
  ctx.fill();

  // Diamond shape
  ctx.shadowBlur = 8 + pulse * 4;
  ctx.shadowColor = `rgba(180, 200, 255, ${0.5 + pulse * 0.3})`;
  ctx.fillStyle = `rgba(200, 220, 255, ${0.6 + pulse * 0.2})`;
  drawPolygon(ctx, kn.x, kn.y + float, 5, 4, Math.PI / 4 + phase * 0.1);
  ctx.fill();
  ctx.strokeStyle = `rgba(220, 230, 255, ${0.7 + pulse * 0.2})`;
  ctx.lineWidth = 1;
  ctx.stroke();
  ctx.shadowBlur = 0;

  // Inner spark
  ctx.fillStyle = `rgba(255, 255, 255, ${0.4 + pulse * 0.3})`;
  ctx.beginPath();
  ctx.arc(kn.x, kn.y + float, 1.5, 0, Math.PI * 2);
  ctx.fill();
}

function drawRemains(ctx: CanvasRenderingContext2D, r: Remains, phase: number) {
  const decayRatio = Math.max(0, 1 - r.age / 300);
  const alpha = 0.3 + decayRatio * 0.3;

  // Dark organic blob
  ctx.fillStyle = hsl(r.hue, 10, 20, alpha * 0.6);
  ctx.beginPath();
  ctx.arc(r.x, r.y, 3 + decayRatio * 2, 0, Math.PI * 2);
  ctx.fill();

  // Faint outline
  ctx.strokeStyle = hsl(r.hue, 15, 35, alpha * 0.4);
  ctx.lineWidth = 0.8;
  ctx.beginPath();
  ctx.arc(r.x, r.y, 3 + decayRatio * 2, 0, Math.PI * 2);
  ctx.stroke();
}

function drawStructure(ctx: CanvasRenderingContext2D, sim: Simulation, s: Structure, phase: number) {
  const hpRatio = Math.max(0, s.hp / s.maxHp);
  const pulse = 0.5 + Math.sin(phase * 0.8 + s.id) * 0.5;
  const baseAlpha = 0.15 + hpRatio * 0.25;

  // Advanced structures get extra glow
  if (s.tier === 'advanced') {
    const ag = ctx.createRadialGradient(s.x, s.y, 0, s.x, s.y, s.radius * 1.5);
    ag.addColorStop(0, hsl(s.hue, 40, 50, 0.08 + pulse * 0.04));
    ag.addColorStop(1, hsl(s.hue, 40, 50, 0));
    ctx.fillStyle = ag;
    ctx.beginPath();
    ctx.arc(s.x, s.y, s.radius * 1.5, 0, Math.PI * 2);
    ctx.fill();
  }

  switch (s.type) {
    case 'nest': {
      const g = ctx.createRadialGradient(s.x, s.y, 0, s.x, s.y, s.radius);
      g.addColorStop(0, hsl(s.hue, 30, 40, baseAlpha * 0.5));
      g.addColorStop(1, hsl(s.hue, 30, 40, 0));
      ctx.fillStyle = g;
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius, 0, Math.PI * 2);
      ctx.fill();

      ctx.shadowBlur = 8 + pulse * 4;
      ctx.shadowColor = hsl(s.hue, 30, 55, 0.4);
      ctx.strokeStyle = hsl(s.hue, 28, 50, baseAlpha + 0.2);
      ctx.lineWidth = 2.5;
      drawPolygon(ctx, s.x, s.y, 12, 6, s.age * 0.001);
      ctx.stroke();
      ctx.shadowBlur = 0;

      ctx.fillStyle = hsl(s.hue, 18, 58, baseAlpha * 0.8);
      drawPolygon(ctx, s.x, s.y, 7, 6, s.age * 0.001);
      ctx.fill();

      ctx.fillStyle = hsl(s.hue, 15, 65, baseAlpha + 0.15);
      ctx.beginPath();
      ctx.arc(s.x, s.y, 2, 0, Math.PI * 2);
      ctx.fill();
      break;
    }
    case 'cache': {
      const g = ctx.createRadialGradient(s.x, s.y, 0, s.x, s.y, s.radius);
      g.addColorStop(0, hsl(s.hue, 25, 40, baseAlpha * 0.4));
      g.addColorStop(1, hsl(s.hue, 25, 40, 0));
      ctx.fillStyle = g;
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius, 0, Math.PI * 2);
      ctx.fill();

      ctx.shadowBlur = 5;
      ctx.shadowColor = hsl(s.hue, 25, 55, 0.3);
      ctx.fillStyle = hsl(s.hue, 20, 52, baseAlpha + 0.1);
      drawPolygon(ctx, s.x, s.y, 8, 4, Math.PI / 4);
      ctx.fill();
      ctx.shadowBlur = 0;

      ctx.strokeStyle = hsl(s.hue, 25, 60, baseAlpha + 0.2);
      ctx.lineWidth = 1.5;
      drawPolygon(ctx, s.x, s.y, 8, 4, Math.PI / 4);
      ctx.stroke();

      ctx.fillStyle = hsl(s.hue, 15, 62, baseAlpha * 0.7);
      drawPolygon(ctx, s.x, s.y, 4, 4, Math.PI / 4);
      ctx.fill();

      if (hpRatio < 1) {
        ctx.strokeStyle = `rgba(255, 200, 100, ${0.3 + pulse * 0.2})`;
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.arc(s.x, s.y, 10, -Math.PI / 2, -Math.PI / 2 + hpRatio * Math.PI * 2);
        ctx.stroke();
      }
      break;
    }
    case 'watchtower': {
      const coneAngle = phase * 0.3 + s.id;
      const coneLen = s.radius * 1.2;
      const coneSpread = 0.6;
      const cg = ctx.createRadialGradient(s.x, s.y - 14, 0, s.x, s.y - 14, coneLen);
      cg.addColorStop(0, hsl(s.hue, 30, 55, 0.12 + pulse * 0.05));
      cg.addColorStop(1, hsl(s.hue, 30, 55, 0));
      ctx.fillStyle = cg;
      ctx.beginPath();
      ctx.moveTo(s.x, s.y - 14);
      ctx.arc(s.x, s.y - 14, coneLen, coneAngle - coneSpread, coneAngle + coneSpread);
      ctx.closePath();
      ctx.fill();

      ctx.fillStyle = hsl(s.hue, 20, 38, baseAlpha * 0.3);
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius, 0, Math.PI * 2);
      ctx.fill();

      ctx.strokeStyle = hsl(s.hue, 22, 50, baseAlpha + 0.25);
      ctx.lineWidth = 2.5;
      ctx.beginPath();
      ctx.moveTo(s.x, s.y);
      ctx.lineTo(s.x, s.y - 14);
      ctx.stroke();

      ctx.shadowBlur = 6 + pulse * 3;
      ctx.shadowColor = hsl(s.hue, 30, 60, 0.5);
      ctx.fillStyle = hsl(s.hue, 28, 58, baseAlpha + 0.2);
      drawPolygon(ctx, s.x, s.y - 16, 8, 3, coneAngle);
      ctx.fill();
      ctx.strokeStyle = hsl(s.hue, 30, 65, baseAlpha + 0.3);
      ctx.lineWidth = 1.5;
      ctx.stroke();
      ctx.shadowBlur = 0;

      ctx.fillStyle = hsl(s.hue, 40, 70, 0.6 + pulse * 0.3);
      ctx.beginPath();
      ctx.arc(s.x, s.y - 16, 2 + pulse, 0, Math.PI * 2);
      ctx.fill();
      break;
    }
    case 'wall': {
      ctx.shadowBlur = 6;
      ctx.shadowColor = hsl(s.hue, 20, 50, 0.3);
      ctx.strokeStyle = hsl(s.hue, 22, 48, baseAlpha + 0.25);
      ctx.lineWidth = 3.5;
      ctx.setLineDash([10, 5]);
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius * 0.7, 0, Math.PI * 2);
      ctx.stroke();
      ctx.setLineDash([]);
      ctx.shadowBlur = 0;

      ctx.strokeStyle = hsl(s.hue, 18, 55, baseAlpha * 0.6);
      ctx.lineWidth = 1.5;
      ctx.setLineDash([4, 6]);
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius * 0.5, 0, Math.PI * 2);
      ctx.stroke();
      ctx.setLineDash([]);

      ctx.fillStyle = hsl(s.hue, 15, 52, baseAlpha * 0.5);
      drawPolygon(ctx, s.x, s.y, 5, 4, phase * 0.2);
      ctx.fill();
      break;
    }
    case 'monument': {
      // Multi-tiered glowing spire
      ctx.shadowBlur = 12 + pulse * 6;
      ctx.shadowColor = hsl(s.hue, 40, 60, 0.5);

      // Base platform
      ctx.fillStyle = hsl(s.hue, 25, 40, baseAlpha * 0.6);
      drawPolygon(ctx, s.x, s.y, 18, 8, phase * 0.05);
      ctx.fill();

      // Mid tier
      ctx.fillStyle = hsl(s.hue, 35, 50, baseAlpha + 0.15);
      drawPolygon(ctx, s.x, s.y, 12, 8, phase * 0.05);
      ctx.fill();
      ctx.strokeStyle = hsl(s.hue, 40, 60, baseAlpha + 0.25);
      ctx.lineWidth = 1.5;
      ctx.stroke();

      // Spire
      ctx.fillStyle = hsl(s.hue, 45, 60, baseAlpha + 0.2);
      drawPolygon(ctx, s.x, s.y, 7, 8, phase * 0.05);
      ctx.fill();

      ctx.shadowBlur = 0;

      // Crown light
      ctx.fillStyle = hsl(s.hue, 50, 75, 0.6 + pulse * 0.3);
      ctx.beginPath();
      ctx.arc(s.x, s.y, 3 + pulse * 1.5, 0, Math.PI * 2);
      ctx.fill();

      // Radiating lines
      ctx.strokeStyle = hsl(s.hue, 40, 65, 0.1 + pulse * 0.05);
      ctx.lineWidth = 1;
      for (let i = 0; i < 8; i++) {
        const a = (i / 8) * Math.PI * 2 + phase * 0.1;
        ctx.beginPath();
        ctx.moveTo(s.x + Math.cos(a) * 8, s.y + Math.sin(a) * 8);
        ctx.lineTo(s.x + Math.cos(a) * (s.radius * 0.8), s.y + Math.sin(a) * (s.radius * 0.8));
        ctx.stroke();
      }
      break;
    }
    case 'sanctuary': {
      // Peaceful protective dome
      const g = ctx.createRadialGradient(s.x, s.y, 0, s.x, s.y, s.radius);
      g.addColorStop(0, hsl(s.hue, 30, 50, 0.08 + pulse * 0.04));
      g.addColorStop(0.7, hsl(s.hue, 35, 55, 0.04));
      g.addColorStop(1, hsl(s.hue, 35, 55, 0));
      ctx.fillStyle = g;
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius, 0, Math.PI * 2);
      ctx.fill();

      // Dome rings
      ctx.strokeStyle = hsl(s.hue, 35, 60, 0.15 + pulse * 0.08);
      ctx.lineWidth = 1.5;
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius * 0.8, 0, Math.PI * 2);
      ctx.stroke();
      ctx.strokeStyle = hsl(s.hue, 35, 60, 0.08 + pulse * 0.04);
      ctx.beginPath();
      ctx.arc(s.x, s.y, s.radius * 0.5, 0, Math.PI * 2);
      ctx.stroke();

      // Central flower
      ctx.shadowBlur = 8;
      ctx.shadowColor = hsl(s.hue, 40, 65, 0.4);
      ctx.fillStyle = hsl(s.hue, 35, 55, baseAlpha + 0.15);
      for (let i = 0; i < 6; i++) {
        const a = (i / 6) * Math.PI * 2 + phase * 0.08;
        const px = s.x + Math.cos(a) * 6;
        const py = s.y + Math.sin(a) * 6;
        drawPolygon(ctx, px, py, 4, 5, a);
        ctx.fill();
      }
      ctx.shadowBlur = 0;

      ctx.fillStyle = hsl(s.hue, 45, 70, 0.5 + pulse * 0.2);
      ctx.beginPath();
      ctx.arc(s.x, s.y, 3, 0, Math.PI * 2);
      ctx.fill();
      break;
    }
    case 'observatory': {
      // Tall tower with rotating ring
      ctx.shadowBlur = 10 + pulse * 4;
      ctx.shadowColor = hsl(s.hue, 35, 60, 0.4);

      // Base
      ctx.fillStyle = hsl(s.hue, 25, 42, baseAlpha * 0.6);
      drawPolygon(ctx, s.x, s.y, 14, 6, 0);
      ctx.fill();

      // Tower
      ctx.fillStyle = hsl(s.hue, 30, 50, baseAlpha + 0.15);
      drawPolygon(ctx, s.x, s.y, 9, 6, 0);
      ctx.fill();
      ctx.strokeStyle = hsl(s.hue, 35, 58, baseAlpha + 0.2);
      ctx.lineWidth = 1.5;
      ctx.stroke();

      ctx.shadowBlur = 0;

      // Rotating ring
      ctx.strokeStyle = hsl(s.hue, 40, 65, 0.3 + pulse * 0.15);
      ctx.lineWidth = 2;
      ctx.setLineDash([4, 3]);
      ctx.beginPath();
      ctx.arc(s.x, s.y, 16, phase * 0.5, phase * 0.5 + Math.PI * 1.5);
      ctx.stroke();
      ctx.setLineDash([]);

      // Lens / eye
      ctx.fillStyle = hsl(s.hue, 45, 70, 0.6 + pulse * 0.3);
      ctx.beginPath();
      ctx.arc(s.x, s.y, 4, 0, Math.PI * 2);
      ctx.fill();
      ctx.fillStyle = `rgba(255, 255, 255, ${0.4 + pulse * 0.3})`;
      ctx.beginPath();
      ctx.arc(s.x, s.y, 1.5, 0, Math.PI * 2);
      ctx.fill();
      break;
    }
    case 'bridge': {
      // Connecting pathway
      const colonies = s.contributingColonies
        .map((cid) => sim.colonies.find((c) => c.id === cid))
        .filter((c): c is NonNullable<typeof c> => c !== undefined);
      if (colonies.length >= 2) {
        ctx.strokeStyle = hsl(s.hue, 30, 50, 0.2 + pulse * 0.1);
        ctx.lineWidth = 4;
        ctx.setLineDash([6, 4]);
        ctx.beginPath();
        ctx.moveTo(colonies[0].centerX, colonies[0].centerY);
        ctx.lineTo(colonies[1].centerX, colonies[1].centerY);
        ctx.stroke();
        ctx.setLineDash([]);
      }

      // Bridge node marker
      ctx.shadowBlur = 6;
      ctx.shadowColor = hsl(s.hue, 30, 55, 0.4);
      ctx.fillStyle = hsl(s.hue, 30, 50, baseAlpha + 0.15);
      drawPolygon(ctx, s.x, s.y, 8, 4, phase * 0.15);
      ctx.fill();
      ctx.strokeStyle = hsl(s.hue, 35, 60, baseAlpha + 0.25);
      ctx.lineWidth = 1.5;
      ctx.stroke();
      ctx.shadowBlur = 0;

      ctx.fillStyle = hsl(s.hue, 40, 68, 0.5 + pulse * 0.2);
      ctx.beginPath();
      ctx.arc(s.x, s.y, 2.5, 0, Math.PI * 2);
      ctx.fill();
      break;
    }
  }
}

function drawOrganism(ctx: CanvasRenderingContext2D, org: Organism, showSense: boolean, selected: boolean, phase: number) {
  const { size, sides, hue, diet, intelligence } = org.genome;
  const isCarnivore = diet >= 0.5;
  const effectiveIntel = Math.min(1, intelligence + org.knowledgeBoost);

  if (showSense) {
    const sg = ctx.createRadialGradient(org.x, org.y, 0, org.x, org.y, org.genome.senseRadius);
    sg.addColorStop(0, hsl(hue, 30, 40, 0.04));
    sg.addColorStop(1, hsl(hue, 30, 40, 0));
    ctx.fillStyle = sg;
    ctx.beginPath();
    ctx.arc(org.x, org.y, org.genome.senseRadius, 0, Math.PI * 2);
    ctx.fill();

    ctx.strokeStyle = hsl(hue, 30, 45, 0.06);
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.arc(org.x, org.y, org.genome.senseRadius, 0, Math.PI * 2);
    ctx.stroke();
  }

  const energyRatio = Math.max(0, Math.min(1, org.energy / 150));
  const lightness = 30 + energyRatio * 22 + effectiveIntel * 12;
  const sat = 25 + effectiveIntel * 15;

  // Body glow
  const glowSize = size + 4 + effectiveIntel * 6;
  const gg = ctx.createRadialGradient(org.x, org.y, 0, org.x, org.y, glowSize);
  gg.addColorStop(0, hsl(hue, sat, lightness, 0.35 + effectiveIntel * 0.2));
  gg.addColorStop(1, hsl(hue, sat, lightness, 0));
  ctx.fillStyle = gg;
  ctx.beginPath();
  ctx.arc(org.x, org.y, glowSize, 0, Math.PI * 2);
  ctx.fill();

  // Body
  ctx.shadowBlur = 6 + effectiveIntel * 8;
  ctx.shadowColor = hsl(hue, sat, lightness, 0.6 + effectiveIntel * 0.2);
  drawPolygon(ctx, org.x, org.y, size, sides, org.angle);
  ctx.fillStyle = hsl(hue, sat, lightness, 0.88);
  ctx.fill();
  ctx.shadowBlur = 0;

  ctx.strokeStyle = hsl(hue, sat, Math.min(72, lightness + 22), 0.85);
  ctx.lineWidth = 1.3;
  ctx.stroke();

  // Intelligence inner pattern
  if (effectiveIntel >= 0.4) {
    ctx.strokeStyle = `rgba(255, 255, 255, ${0.15 + effectiveIntel * 0.3})`;
    ctx.lineWidth = 0.9;
    drawPolygon(ctx, org.x, org.y, size * 0.5, sides, org.angle);
    ctx.stroke();
  }
  if (effectiveIntel >= 0.7) {
    ctx.fillStyle = `rgba(255, 255, 255, ${0.1 + effectiveIntel * 0.15})`;
    ctx.beginPath();
    ctx.arc(org.x, org.y, size * 0.22, 0, Math.PI * 2);
    ctx.fill();
  }

  // Direction indicator
  ctx.strokeStyle = hsl(hue, 12, 78, 0.55);
  ctx.lineWidth = 1.2;
  ctx.beginPath();
  ctx.moveTo(org.x, org.y);
  ctx.lineTo(org.x + Math.cos(org.angle) * size, org.y + Math.sin(org.angle) * size);
  ctx.stroke();

  // Carnivore marker
  if (isCarnivore) {
    ctx.shadowBlur = 4;
    ctx.shadowColor = 'rgba(255, 80, 80, 0.6)';
    ctx.fillStyle = 'rgba(255, 100, 100, 0.9)';
    ctx.beginPath();
    ctx.arc(org.x, org.y, 1.8, 0, Math.PI * 2);
    ctx.fill();
    ctx.shadowBlur = 0;
  }

  // Carrying material indicator
  if (org.carrying > 0) {
    ctx.strokeStyle = `rgba(180, 140, 80, ${0.3 + (org.carrying / 15) * 0.3})`;
    ctx.lineWidth = 1.2;
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 2.5, 0, Math.PI * 2);
    ctx.stroke();
  }

  // Knowledge boost aura
  if (org.knowledgeBoost > 0.02) {
    const kp = 0.4 + Math.sin(phase * 2 + org.id) * 0.2;
    ctx.strokeStyle = `rgba(180, 200, 255, ${org.knowledgeBoost * kp * 2})`;
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 8, 0, Math.PI * 2);
    ctx.stroke();
  }

  // Leader ring
  if (org.colonyRole === 'leader') {
    const lp = 0.6 + Math.sin(phase * 1.5 + org.id) * 0.2;
    ctx.strokeStyle = hsl(hue, 30, 68, lp * 0.7);
    ctx.lineWidth = 1.8;
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 4, 0, Math.PI * 2);
    ctx.stroke();
  }

  // Threat indicator
  if (org.threatLevel > 0.3) {
    const tp = 0.4 + Math.sin(phase * 4 + org.id) * 0.3;
    ctx.strokeStyle = `rgba(255, 70, 70, ${org.threatLevel * tp})`;
    ctx.lineWidth = 1.8;
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 6, 0, Math.PI * 2);
    ctx.stroke();
  }

  // Selection
  if (selected) {
    const sp = 0.7 + Math.sin(phase * 3) * 0.3;
    ctx.strokeStyle = `rgba(255, 255, 255, ${sp})`;
    ctx.lineWidth = 2.5;
    ctx.setLineDash([5, 4]);
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 7, 0, Math.PI * 2);
    ctx.stroke();
    ctx.setLineDash([]);

    ctx.strokeStyle = `rgba(255, 255, 255, ${sp * 0.4})`;
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(org.x - size - 12, org.y);
    ctx.lineTo(org.x - size - 4, org.y);
    ctx.moveTo(org.x + size + 4, org.y);
    ctx.lineTo(org.x + size + 12, org.y);
    ctx.moveTo(org.x, org.y - size - 12);
    ctx.lineTo(org.x, org.y - size - 4);
    ctx.moveTo(org.x, org.y + size + 4);
    ctx.lineTo(org.x, org.y + size + 12);
    ctx.stroke();
  }

  // Biofilm membership indicator
  if (org.inBiofilm) {
    ctx.strokeStyle = `rgba(100, 200, 150, ${0.3 + Math.sin(phase * 2 + org.id) * 0.1})`;
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 3, 0, Math.PI * 2);
    ctx.stroke();
  }

  // Spore mode indicator
  if (org.sporeMode) {
    ctx.strokeStyle = `rgba(200, 180, 120, 0.6)`;
    ctx.lineWidth = 1.5;
    ctx.setLineDash([2, 2]);
    ctx.beginPath();
    ctx.arc(org.x, org.y, size + 5, 0, Math.PI * 2);
    ctx.stroke();
    ctx.setLineDash([]);
  }
}

function drawChemicalField(ctx: CanvasRenderingContext2D, sim: Simulation, _phase: number) {
  const field = sim.chemicalField!;
  const { cols, rows, cellSize, attractant, repellent } = field;

  for (let cy = 0; cy < rows; cy++) {
    for (let cx = 0; cx < cols; cx++) {
      const i = cy * cols + cx;
      const att = attractant[i];
      const rep = repellent[i];
      const x = cx * cellSize;
      const y = cy * cellSize;

      if (att > 0.1) {
        const alpha = Math.min(0.15, att * 0.04);
        ctx.fillStyle = `rgba(100, 220, 150, ${alpha})`;
        ctx.fillRect(x, y, cellSize, cellSize);
      }

      if (rep > 0.1) {
        const alpha = Math.min(0.12, rep * 0.03);
        ctx.fillStyle = `rgba(220, 100, 100, ${alpha})`;
        ctx.fillRect(x, y, cellSize, cellSize);
      }
    }
  }
}

function drawBiofilm(ctx: CanvasRenderingContext2D, bf: BiofilmCluster, phase: number) {
  const pulse = 0.5 + Math.sin(phase * 1.5 + bf.id) * 0.5;
  const alpha = 0.08 + pulse * 0.05;

  // Biofilm blob — organic, irregular shape
  const grad = ctx.createRadialGradient(bf.cx, bf.cy, 0, bf.cx, bf.cy, bf.radius);
  grad.addColorStop(0, `rgba(100, 200, 150, ${alpha + 0.05})`);
  grad.addColorStop(0.7, `rgba(80, 180, 130, ${alpha * 0.5})`);
  grad.addColorStop(1, `rgba(60, 160, 110, 0)`);
  ctx.fillStyle = grad;
  ctx.beginPath();
  ctx.arc(bf.cx, bf.cy, bf.radius, 0, Math.PI * 2);
  ctx.fill();

  // Cluster boundary — dashed, organic
  ctx.strokeStyle = `rgba(120, 220, 160, ${0.15 + pulse * 0.1})`;
  ctx.lineWidth = 1.2;
  ctx.setLineDash([4, 3]);
  ctx.beginPath();
  ctx.arc(bf.cx, bf.cy, bf.radius, 0, Math.PI * 2);
  ctx.stroke();
  ctx.setLineDash([]);
}

// Pre-computed ambient particle positions (deterministic, no per-frame alloc)
const AMBIENT_PARTICLES = Array.from({ length: 40 }, (_, i) => ({
  baseX: Math.random(),
  baseY: Math.random(),
  speed: 0.3 + Math.random() * 0.7,
  size: 0.5 + Math.random() * 1.5,
  hue: 180 + Math.random() * 60,
  phaseOffset: Math.random() * Math.PI * 2,
}));

function drawAmbientParticles(ctx: CanvasRenderingContext2D, width: number, height: number, phase: number) {
  for (const p of AMBIENT_PARTICLES) {
    const x = p.baseX * width + Math.sin(phase * p.speed + p.phaseOffset) * 30;
    const y = p.baseY * height + Math.cos(phase * p.speed * 0.7 + p.phaseOffset) * 20;
    const alpha = 0.08 + Math.sin(phase * 0.5 + p.phaseOffset) * 0.04;
    ctx.fillStyle = `hsla(${p.hue}, 40%, 70%, ${alpha})`;
    ctx.beginPath();
    ctx.arc(x, y, p.size, 0, Math.PI * 2);
    ctx.fill();
  }
}
