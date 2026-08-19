/**
 * Detect app mode from URL params
 */
export function isWallpaperMode(): boolean {
  if (typeof window === 'undefined') return false;
  const params = new URLSearchParams(window.location.search);
  return params.get('wallpaper') === '1' || params.has('wallpaper');
}

export function isEmbedMode(): boolean {
  if (typeof window === 'undefined') return false;
  const params = new URLSearchParams(window.location.search);
  return params.get('embed') === '1';
}
