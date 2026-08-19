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
