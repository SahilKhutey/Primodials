import type { SimSettings } from "@/sim/types";

export type ReadinessItem = {
  id: string;
  label: string;
  ok: boolean;
  detail: string;
};

export function evaluateReleaseReadiness(settings: SimSettings): ReadinessItem[] {
  const secureContext = typeof window !== "undefined" && (window.isSecureContext || location.hostname === "localhost");

  return [
    {
      id: "canvas",
      label: "Canvas rendering",
      ok: typeof document !== "undefined" && !!document.createElement("canvas").getContext("2d"),
      detail: "2D canvas support",
    },
    {
      id: "storage",
      label: "Local persistence",
      ok: (() => {
        try {
          if (typeof window === "undefined") return false;
          const k = "__pp_release__";
          localStorage.setItem(k, "1");
          localStorage.removeItem(k);
          return true;
        } catch {
          return false;
        }
      })(),
      detail: "localStorage read/write",
    },
    {
      id: "secure",
      label: "Secure context",
      ok: !!secureContext,
      detail: secureContext ? "secure origin" : "non-secure origin",
    },
    {
      id: "settings",
      label: "Settings",
      ok: settings.maxPopulation >= settings.initialPopulation && settings.maxFood >= settings.initialFood,
      detail: `population ${settings.initialPopulation}/${settings.maxPopulation}`,
    },
    {
      id: "viewport",
      label: "Viewport",
      ok: typeof window !== "undefined" ? window.innerWidth >= 640 && window.innerHeight >= 360 : true,
      detail: typeof window !== "undefined" ? `${window.innerWidth}×${window.innerHeight}` : "SSR/test",
    },
  ];
}
