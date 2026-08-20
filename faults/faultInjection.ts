export type FaultName =
  | "storage"
  | "snapshot"
  | "world-share"
  | "simulation-step"
  | "render"
  | "wallpaper-bridge";

const SUPPORTED = new Set<FaultName>([
  "storage",
  "snapshot",
  "world-share",
  "simulation-step",
  "render",
  "wallpaper-bridge",
]);

function fromQuery(): FaultName | null {
  if (typeof window === "undefined") return null;
  const value = new URL(window.location.href).searchParams.get("fault");
  return SUPPORTED.has(value as FaultName) ? (value as FaultName) : null;
}

function fromEnv(): FaultName | null {
  const value = import.meta.env?.VITE_PP_FAULT;
  return SUPPORTED.has(value as FaultName) ? (value as FaultName) : null;
}

export function activeFault(): FaultName | null {
  if (!import.meta.env.DEV) return null;
  return fromQuery() ?? fromEnv();
}

export function shouldFail(name: FaultName): boolean {
  return activeFault() === name;
}

export function assertNoFault(name: FaultName): void {
  if (shouldFail(name)) throw new Error(`FAULT_INJECTION:${name}`);
}
