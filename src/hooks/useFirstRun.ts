import { useCallback, useEffect, useState } from 'react';

const STORAGE_KEY = 'polygonal-primordials.first-run.v1';

export type FirstRunState = {
  completed: boolean;
  completedAt: string | null;
  version: 1;
};

const DEFAULT_STATE: FirstRunState = {
  completed: false,
  completedAt: null,
  version: 1,
};

function readState(): FirstRunState {
  if (typeof window === 'undefined') return DEFAULT_STATE;

  try {
    const raw = window.localStorage.getItem(STORAGE_KEY);
    if (!raw) return DEFAULT_STATE;

    const parsed = JSON.parse(raw) as Partial<FirstRunState>;
    if (parsed.version !== 1) return DEFAULT_STATE;

    return {
      ...DEFAULT_STATE,
      ...parsed,
    };
  } catch {
    return DEFAULT_STATE;
  }
}

export function useFirstRun() {
  const [state, setState] = useState<FirstRunState>(readState);
  const [open, setOpen] = useState(false);

  useEffect(() => {
    if (!state.completed) setOpen(true);
  }, [state.completed]);

  const complete = useCallback(() => {
    const next: FirstRunState = {
      version: 1,
      completed: true,
      completedAt: new Date().toISOString(),
    };

    setState(next);
    setOpen(false);

    try {
      window.localStorage.setItem(STORAGE_KEY, JSON.stringify(next));
    } catch {
      // First-run UX must remain functional even when storage is unavailable.
    }
  }, []);

  const reopen = useCallback(() => setOpen(true), []);

  const reset = useCallback(() => {
    setState(DEFAULT_STATE);
    setOpen(true);

    try {
      window.localStorage.removeItem(STORAGE_KEY);
    } catch {
      // Intentionally ignored.
    }
  }, []);

  return {
    open,
    completed: state.completed,
    completedAt: state.completedAt,
    complete,
    reopen,
    reset,
  };
}
