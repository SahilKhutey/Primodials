import type { SimSettings } from "@/sim/types";

export type ReleaseDiagnostic = {
  name: string;
  ok: boolean;
  message: string;
};

export function runClientDiagnostics(settings: SimSettings): ReleaseDiagnostic[] {
  const diagnostics: ReleaseDiagnostic[] = [];

  diagnostics.push({
    name: "Canvas",
    ok: typeof document !== "undefined" && !!document.createElement("canvas").getContext("2d"),
    message: "2D canvas rendering context",
  });

  diagnostics.push({
    name: "Local storage",
    ok: (() => {
      try {
        const key = "__pp_diag__";
        window.localStorage.setItem(key, "1");
        window.localStorage.removeItem(key);
        return true;
      } catch {
        return false;
      }
    })(),
    message: "Local persistence available",
  });

  diagnostics.push({
    name: "World bounds",
    ok: Number.isFinite(settings.worldWidth) && Number.isFinite(settings.worldHeight),
    message: `${settings.worldWidth} × ${settings.worldHeight}`,
  });

  diagnostics.push({
    name: "Population limit",
    ok: settings.maxPopulation > 0 && settings.initialPopulation <= settings.maxPopulation,
    message: `${settings.initialPopulation}/${settings.maxPopulation}`,
  });

  return diagnostics;
}
