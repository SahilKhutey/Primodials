export type DisplayMetrics = {
  width: number;
  height: number;
  devicePixelRatio: number;
  aspect: number;
  refreshRateHint: number | null;
};

export function readDisplayMetrics(): DisplayMetrics {
  if (typeof window === "undefined") {
    return {
      width: 1920,
      height: 1080,
      devicePixelRatio: 1,
      aspect: 16 / 9,
      refreshRateHint: null,
    };
  }

  const width = Math.max(1, window.innerWidth);
  const height = Math.max(1, window.innerHeight);

  return {
    width,
    height,
    devicePixelRatio: Math.max(1, window.devicePixelRatio || 1),
    aspect: width / height,
    refreshRateHint: null,
  };
}

export function installDisplayMetricsListener(
  onChange: (metrics: DisplayMetrics) => void,
): () => void {
  if (typeof window === "undefined") {
    return () => {};
  }

  let frame = 0;

  const notify = () => {
    if (frame) cancelAnimationFrame(frame);
    frame = requestAnimationFrame(() => {
      frame = 0;
      onChange(readDisplayMetrics());
    });
  };

  window.addEventListener("resize", notify, { passive: true });
  window.addEventListener("orientationchange", notify, { passive: true });

  return () => {
    if (frame) cancelAnimationFrame(frame);
    window.removeEventListener("resize", notify);
    window.removeEventListener("orientationchange", notify);
  };
}
