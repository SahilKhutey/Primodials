export async function enterFullscreen(element: HTMLElement = document.documentElement) {
  if (document.fullscreenElement) return true;
  try {
    await element.requestFullscreen();
    return true;
  } catch {
    return false;
  }
}

export async function exitFullscreen() {
  if (!document.fullscreenElement) return true;
  try {
    await document.exitFullscreen();
    return true;
  } catch {
    return false;
  }
}
