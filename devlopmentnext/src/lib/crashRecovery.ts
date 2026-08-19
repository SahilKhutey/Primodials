import type { Simulation } from "@/sim/simulation";
import { saveLocalSnapshot, loadLocalSnapshot } from "@/sim/localSnapshot";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";

const CRASH_FLAG = "polygonal-primordials.runtime.flag.v1";

export function markRuntimeStarted(): void {
  try {
    localStorage.setItem(
      CRASH_FLAG,
      JSON.stringify({
        startedAt: new Date().toISOString(),
        clean: false,
      }),
    );
  } catch {}
}

export function markRuntimeClean(): void {
  try {
    localStorage.setItem(
      CRASH_FLAG,
      JSON.stringify({
        startedAt: new Date().toISOString(),
        clean: true,
      }),
    );
  } catch {}
}

export function previousRunWasUnclean(): boolean {
  try {
    const raw = localStorage.getItem(CRASH_FLAG);
    if (!raw) return false;
    const value = JSON.parse(raw) as { clean?: boolean };
    return value.clean === false;
  } catch {
    return false;
  }
}

export function installCrashRecovery(sim: Simulation): () => void {
  markRuntimeStarted();

  const save = () => {
    try {
      saveLocalSnapshot(sim);
    } catch {}
  };

  window.addEventListener("beforeunload", save);
  window.addEventListener("pagehide", save);

  const onError = () => save();
  const onRejection = () => save();

  window.addEventListener("error", onError);
  window.addEventListener("unhandledrejection", onRejection);

  return () => {
    window.removeEventListener("beforeunload", save);
    window.removeEventListener("pagehide", save);
    window.removeEventListener("error", onError);
    window.removeEventListener("unhandledrejection", onRejection);
    markRuntimeClean();
  };
}

export function recoverPreviousRun(): Simulation | null {
  const snapshot = loadLocalSnapshot();
  if (!snapshot) return null;
  try {
    return restoreSimulationSnapshot(snapshot);
  } catch {
    return null;
  }
}
