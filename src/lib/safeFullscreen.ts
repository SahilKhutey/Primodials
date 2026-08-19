export async function enterFullscreen(element: HTMLElement = document.documentElement): Promise<boolean> {
  if (typeof document === "undefined") return false;
  if (document.fullscreenElement) return true;
  try {
    await element.requestFullscreen();
    return true;
  } catch {
    return false;
  }
}

export async function exitFullscreen(): Promise<boolean> {
  if (typeof document === "undefined") return false;
  if (!document.fullscreenElement) return true;
  try {
    await document.exitFullscreen();
    return true;
  } catch {
    return false;
  }
}
