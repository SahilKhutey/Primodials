import { useEffect } from 'react';

export type KeyboardShortcutHandlers = {
  onToggleRun: () => void;
  onStep?: () => void;
  onReset?: () => void;
  onToggleFullscreen?: () => void;
  onToggleHelp?: () => void;
  onToggleDiary?: () => void;
};

export function useKeyboardShortcuts(
  handlers: KeyboardShortcutHandlers,
  enabled = true,
) {
  useEffect(() => {
    if (!enabled) return;

    const onKeyDown = (event: KeyboardEvent) => {
      const target = event.target;

      if (
        target instanceof HTMLInputElement ||
        target instanceof HTMLTextAreaElement ||
        target instanceof HTMLSelectElement ||
        (target instanceof HTMLElement && target.isContentEditable)
      ) {
        return;
      }

      const key = event.key.toLowerCase();

      if (event.ctrlKey || event.metaKey || event.altKey) {
        if (key === 'f') {
          event.preventDefault();
          handlers.onToggleFullscreen?.();
        }
        return;
      }

      if (key === ' ' || key === 'p') {
        event.preventDefault();
        handlers.onToggleRun();
        return;
      }

      if (key === 'n' || key === '.') {
        event.preventDefault();
        handlers.onStep?.();
        return;
      }

      if (key === 'r') {
        event.preventDefault();
        handlers.onReset?.();
        return;
      }

      if (key === 'h' || key === '?') {
        event.preventDefault();
        handlers.onToggleHelp?.();
        return;
      }

      if (key === 'd') {
        event.preventDefault();
        handlers.onToggleDiary?.();
      }
    };

    window.addEventListener('keydown', onKeyDown);
    return () => window.removeEventListener('keydown', onKeyDown);
  }, [enabled, handlers]);
}
