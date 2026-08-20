import { useCallback, useEffect, useState } from "react";

export type PresentationMode = "normal" | "cinematic" | "capture";
const STORAGE_KEY = "polygonal-primordials.presentation-mode.v1";

function readInitial(): PresentationMode {
  if (typeof window === "undefined") return "normal";
  const query = new URLSearchParams(window.location.search).get("presentation");
  if (query === "cinematic" || query === "capture") return query;
  try {
    const stored = window.localStorage.getItem(STORAGE_KEY);
    if (stored === "normal" || stored === "cinematic" || stored === "capture") return stored;
  } catch {
    // Ignore localStorage access failures in restricted environments
  }
  return "normal";
}

export function usePresentationMode() {
  const [mode, setMode] = useState<PresentationMode>(readInitial);

  useEffect(() => {
    try {
      window.localStorage.setItem(STORAGE_KEY, mode);
    } catch {
      // Ignore localStorage write failures
    }
    const url = new URL(window.location.href);
    if (mode === "normal") url.searchParams.delete("presentation");
    else url.searchParams.set("presentation", mode);
    window.history.replaceState({}, "", url.toString());
  }, [mode]);

  const cycle = useCallback(() => {
    setMode((current) =>
      current === "normal" ? "cinematic" : current === "cinematic" ? "capture" : "normal",
    );
  }, []);

  return {
    mode,
    setMode,
    cycle,
    isCinematic: mode === "cinematic",
    isCapture: mode === "capture",
    isPresentation: mode !== "normal",
  };
}
