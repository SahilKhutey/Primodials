export type VisibilityCallbacks = {
  onHidden: () => void;
  onVisible: () => void;
};

export function installVisibilityController(
  callbacks: VisibilityCallbacks,
): () => void {
  if (typeof document === "undefined") {
    return () => {};
  }

  const handler = () => {
    if (document.visibilityState === "hidden") callbacks.onHidden();
    else callbacks.onVisible();
  };

  document.addEventListener("visibilitychange", handler);
  return () => document.removeEventListener("visibilitychange", handler);
}
