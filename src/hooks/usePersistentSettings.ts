/**
 * usePersistentSettings — localStorage-backed SimSettings
 *
 * Survives restarts. Validates on load. Reverts to defaults if corrupted.
 *
 * License: MIT
 */

import { useCallback, useEffect, useRef, useState } from "react";
import { safeGetJSON, safeSetJSON } from "@/lib/safeStorage";
import { validateSettings, type SimSettings } from "@/lib/settingsValidation";

const STORAGE_KEY = "primordials:settings:v1";

export interface UsePersistentSettingsResult {
  settings: SimSettings;
  setSettings: (next: SimSettings | ((prev: SimSettings) => SimSettings)) => void;
  resetSettings: () => void;
  isLoaded: boolean;
  lastSavedAt: number | null;
}

export function loadPersistedSettings(defaults: SimSettings): SimSettings {
  const stored = safeGetJSON<Partial<SimSettings>>(STORAGE_KEY);
  if (!stored) return defaults;
  return validateSettings(stored, defaults);
}

export function usePersistentSettings(
  defaults: SimSettings
): UsePersistentSettingsResult {
  const [settings, setSettingsInternal] = useState<SimSettings>(() =>
    loadPersistedSettings(defaults)
  );
  const [isLoaded, setIsLoaded] = useState(false);
  const [lastSavedAt, setLastSavedAt] = useState<number | null>(null);
  const saveTimerRef = useRef<number | null>(null);

  // ─── Load on mount ─────────────────────────────────
  useEffect(() => {
    const stored = safeGetJSON<Partial<SimSettings>>(STORAGE_KEY);
    if (stored) {
      const validated = validateSettings(stored, defaults);
      setSettingsInternal(validated);
      setLastSavedAt(Date.now());
    }
    setIsLoaded(true);
  }, [defaults]);

  // ─── Debounced save ───────────────────────────────
  const scheduleSave = useCallback((next: SimSettings) => {
    if (saveTimerRef.current !== null) {
      window.clearTimeout(saveTimerRef.current);
    }
    saveTimerRef.current = window.setTimeout(() => {
      safeSetJSON(STORAGE_KEY, next);
      setLastSavedAt(Date.now());
    }, 300);
  }, []);

  const setSettings = useCallback(
    (next: SimSettings | ((prev: SimSettings) => SimSettings)) => {
      setSettingsInternal((prev) => {
        const resolved =
          typeof next === "function"
            ? (next as (p: SimSettings) => SimSettings)(prev)
            : next;
        const validated = validateSettings(resolved, defaults);
        scheduleSave(validated);
        return validated;
      });
    },
    [defaults, scheduleSave]
  );

  const resetSettings = useCallback(() => {
    setSettingsInternal(defaults);
    safeSetJSON(STORAGE_KEY, defaults);
    setLastSavedAt(Date.now());
  }, [defaults]);

  // ─── Flush on unmount ─────────────────────────────
  useEffect(() => {
    return () => {
      if (saveTimerRef.current !== null) {
        window.clearTimeout(saveTimerRef.current);
      }
      safeSetJSON(STORAGE_KEY, settings);
    };
  }, [settings]);

  return { settings, setSettings, resetSettings, isLoaded, lastSavedAt };
}
