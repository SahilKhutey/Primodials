import { useCallback, useEffect, useState } from "react";
import { getSafeStorage } from "@/lib/safeStorage";
import type { SimSettings } from "@/sim/types";

const STORAGE_KEY = "polygonal-primordials.settings.v1";

export function loadPersistedSettings(defaults: SimSettings): SimSettings {
  const storage = getSafeStorage();
  if (!storage) return defaults;

  try {
    const raw = storage.getItem(STORAGE_KEY);
    if (!raw) return defaults;
    const parsed = JSON.parse(raw) as Partial<SimSettings>;
    return { ...defaults, ...parsed };
  } catch {
    return defaults;
  }
}

export function usePersistentSettings(defaults: SimSettings) {
  const [settings, setSettingsState] = useState<SimSettings>(() =>
    loadPersistedSettings(defaults),
  );

  useEffect(() => {
    const storage = getSafeStorage();
    if (!storage) return;
    try {
      storage.setItem(STORAGE_KEY, JSON.stringify(settings));
    } catch {
      // Quota/privacy mode failures are intentionally non-fatal.
    }
  }, [settings]);

  const setSettings = useCallback(
    (next: SimSettings | ((current: SimSettings) => SimSettings)) => {
      setSettingsState((current) =>
        typeof next === "function" ? next(current) : next,
      );
    },
    [],
  );

  const resetSettings = useCallback(() => {
    setSettingsState(defaults);
  }, [defaults]);

  return { settings, setSettings, resetSettings };
}
