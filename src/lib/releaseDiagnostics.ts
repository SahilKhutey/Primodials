/**
 * releaseDiagnostics — pre-flight environment checks
 *
 * Run in dev mode or on demand. Catches runtime environment limitations.
 *
 * License: MIT
 */

import type { SimSettings } from "@/sim/types";

export interface DiagnosticResult {
  name: string;
  status: "pass" | "warn" | "fail";
  ok: boolean;
  message: string;
}

export type ReleaseDiagnostic = DiagnosticResult;

export function runClientDiagnostics(settings: SimSettings): DiagnosticResult[] {
  const results: DiagnosticResult[] = [];

  // ─── Canvas ─────────────────────────────────────────
  try {
    const canvas = document.createElement("canvas");
    const ctx = canvas.getContext("2d");
    results.push({
      name: "Canvas 2D",
      status: ctx ? "pass" : "fail",
      ok: !!ctx,
      message: ctx ? "Available" : "Not supported — wallpaper will not render",
    });
  } catch {
    results.push({
      name: "Canvas 2D",
      status: "fail",
      ok: false,
      message: "Canvas API threw on creation",
    });
  }

  // ─── LocalStorage ──────────────────────────────────
  try {
    const probe = "__primordials_probe__";
    window.localStorage.setItem(probe, "1");
    window.localStorage.removeItem(probe);
    results.push({
      name: "LocalStorage",
      status: "pass",
      ok: true,
      message: "Available — settings will persist",
    });
  } catch {
    results.push({
      name: "LocalStorage",
      status: "fail",
      ok: false,
      message: "Blocked — settings will not persist (private mode?)",
    });
  }

  // ─── World dimensions ─────────────────────────────
  const area = settings.worldWidth * settings.worldHeight;
  if (area > 8_000_000) {
    results.push({
      name: "World size",
      status: "warn",
      ok: true,
      message: `Large world (${area.toLocaleString()} units²) may hurt FPS`,
    });
  } else {
    results.push({
      name: "World size",
      status: "pass",
      ok: true,
      message: `${settings.worldWidth}×${settings.worldHeight} (${area.toLocaleString()} units²)`,
    });
  }

  // ─── Population limit ────────────────────────────
  if (settings.maxPopulation > 0 && settings.initialPopulation <= settings.maxPopulation) {
    results.push({
      name: "Population limits",
      status: "pass",
      ok: true,
      message: `${settings.initialPopulation}/${settings.maxPopulation}`,
    });
  } else {
    results.push({
      name: "Population limits",
      status: "warn",
      ok: false,
      message: `Initial (${settings.initialPopulation}) exceeds max (${settings.maxPopulation})`,
    });
  }

  return results;
}

export function diagnosticsToTable(results: DiagnosticResult[]): string {
  const rows = results
    .map((r) => `${r.status.toUpperCase().padEnd(4)} | ${r.name.padEnd(22)} | ${r.message}`)
    .join("\n");
  return `STATUS | NAME                   | MESSAGE\n-------+------------------------+---------\n${rows}`;
}
