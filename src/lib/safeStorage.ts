/**
 * safeStorage — defensive localStorage wrapper
 *
 * - Quota-exceeded safe
 * - Corrupt-JSON safe
 * - Disabled in private mode
 * - Returns null on any failure
 *
 * License: MIT
 */

export type StorageLike = Pick<Storage, "getItem" | "setItem" | "removeItem">;

export function getSafeStorage(): StorageLike | null {
  try {
    if (typeof window === "undefined" || !window.localStorage) return null;
    const probe = "__primordials_storage_probe__";
    window.localStorage.setItem(probe, "1");
    window.localStorage.removeItem(probe);
    return window.localStorage;
  } catch {
    return null;
  }
}

export function safeGetJSON<T>(key: string): T | null {
  try {
    if (typeof window === "undefined") return null;
    const raw = window.localStorage.getItem(key);
    if (raw === null) return null;
    return JSON.parse(raw) as T;
  } catch {
    return null;
  }
}

export function safeSetJSON(key: string, value: unknown): boolean {
  try {
    if (typeof window === "undefined") return false;
    const json = JSON.stringify(value);
    window.localStorage.setItem(key, json);
    return true;
  } catch {
    // QuotaExceededError, SecurityError, etc.
    return false;
  }
}

export function safeRemove(key: string): void {
  try {
    if (typeof window === "undefined") return;
    window.localStorage.removeItem(key);
  } catch {
    // Ignore
  }
}

export function safeGetString(key: string): string | null {
  try {
    if (typeof window === "undefined") return null;
    return window.localStorage.getItem(key);
  } catch {
    return null;
  }
}

export function safeSetString(key: string, value: string): boolean {
  try {
    if (typeof window === "undefined") return false;
    window.localStorage.setItem(key, value);
    return true;
  } catch {
    return false;
  }
}
