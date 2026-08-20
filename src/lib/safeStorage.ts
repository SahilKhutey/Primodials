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

export function safeGetString(key: string): string | null {
  const storage = getSafeStorage();
  if (!storage) return null;
  try {
    return storage.getItem(key);
  } catch {
    return null;
  }
}

export function safeSetString(key: string, value: string): boolean {
  const storage = getSafeStorage();
  if (!storage) return false;
  try {
    storage.setItem(key, value);
    return true;
  } catch {
    return false;
  }
}

export function safeGetJSON<T = unknown>(key: string): T | null {
  const raw = safeGetString(key);
  if (raw === null) return null;
  try {
    return JSON.parse(raw) as T;
  } catch {
    return null;
  }
}

export function safeSetJSON<T = unknown>(key: string, value: T): boolean {
  try {
    const raw = JSON.stringify(value);
    return safeSetString(key, raw);
  } catch {
    return false;
  }
}

export function safeRemoveItem(key: string): boolean {
  const storage = getSafeStorage();
  if (!storage) return false;
  try {
    storage.removeItem(key);
    return true;
  } catch {
    return false;
  }
}
