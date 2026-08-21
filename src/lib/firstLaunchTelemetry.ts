import { getSafeStorage } from "./safeStorage";

const SESSION_KEY = "polygonal-primordials.session.v1";

export type FirstLaunchEvent =
  | "launch"
  | "onboarding-start"
  | "onboarding-complete"
  | "simulation-start"
  | "wallpaper-start"
  | "share-world"
  | "local-save"
  | "local-recovery"
  | "crash-recovery"
  | "release-build";

type Session = {
  sessionId: string;
  firstSeenAt: string;
  eventCounts: Partial<Record<FirstLaunchEvent, number>>;
};

function createSessionId(): string {
  if (typeof crypto !== "undefined" && "randomUUID" in crypto) return crypto.randomUUID();
  return `${Date.now()}-${Math.random().toString(16).slice(2)}`;
}

function readSession(): Session {
  const storage = getSafeStorage();
  if (storage) {
    try {
      const raw = storage.getItem(SESSION_KEY);
      if (raw) return JSON.parse(raw) as Session;
    } catch {
      // Ignore storage/parsing errors
    }
  }
  return { sessionId: createSessionId(), firstSeenAt: new Date().toISOString(), eventCounts: {} };
}

const session = readSession();

function persist() {
  const storage = getSafeStorage();
  if (!storage) return;
  try {
    storage.setItem(SESSION_KEY, JSON.stringify(session));
  } catch {
    // Ignore storage write errors
  }
}

export function recordFirstLaunchEvent(event: FirstLaunchEvent): void {
  session.eventCounts[event] = (session.eventCounts[event] ?? 0) + 1;
  persist();
}

export function getLocalSessionSummary(): Session {
  return { ...session, eventCounts: { ...session.eventCounts } };
}
