import { useEffect } from "react";
import type { Simulation } from "@/sim/simulation";
import { writeAutosave } from "@/lib/autosave";

export function useAutosave(
  sim: Simulation,
  enabled: boolean,
  intervalMs = 60_000,
): void {
  useEffect(() => {
    if (!enabled) return;

    const save = () => writeAutosave(sim);
    const id = window.setInterval(save, Math.max(10_000, intervalMs));

    const onBeforeUnload = () => save();
    window.addEventListener("beforeunload", onBeforeUnload);

    return () => {
      window.clearInterval(id);
      window.removeEventListener("beforeunload", onBeforeUnload);
    };
  }, [sim, enabled, intervalMs]);
}
