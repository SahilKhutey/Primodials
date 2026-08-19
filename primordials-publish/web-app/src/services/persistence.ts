/**
 * Local persistence using localStorage
 */
const STORAGE_KEY = 'primordials-state';

export interface StoredState {
  lastBiomeId: string;
  lastSeed: number;
  parameters: Record<string, unknown>;
}

export function saveState(state: StoredState): void {
  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
  } catch (e) {
    // Silently fail
  }
}

export function loadState(): StoredState | null {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) return null;
    return JSON.parse(raw) as StoredState;
  } catch {
    return null;
  }
}

export function clearState(): void {
  try {
    localStorage.removeItem(STORAGE_KEY);
  } catch {
    // ignore
  }
}
