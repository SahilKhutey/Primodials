(() => {
  const state = { startedAt: 0, last: 0, frames: 0, slowFrames: 0, samples: [] };
  let raf = 0;
  let windowStart = 0;
  let windowFrames = 0;

  function loop(now) {
    const frameMs = now - state.last;
    state.last = now;
    state.frames++;
    windowFrames++;
    if (frameMs > 34) state.slowFrames++;

    if (now - windowStart >= 1000) {
      state.samples.push({
        atMs: Math.round(now - state.startedAt),
        fps: windowFrames / ((now - windowStart) / 1000),
        frameMs,
      });
      windowFrames = 0;
      windowStart = now;
    }
    raf = requestAnimationFrame(loop);
  }

  window.__PP_BENCHMARK__ = {
    start() {
      cancelAnimationFrame(raf);
      state.startedAt = performance.now();
      state.last = state.startedAt;
      windowStart = state.startedAt;
      windowFrames = 0;
      state.frames = 0;
      state.slowFrames = 0;
      state.samples = [];
      raf = requestAnimationFrame(loop);
    },
    stop() {
      cancelAnimationFrame(raf);
      const averageFps = state.samples.length
        ? state.samples.reduce((s, x) => s + x.fps, 0) / state.samples.length
        : 0;
      return {
        durationSeconds: (performance.now() - state.startedAt) / 1000,
        frames: state.frames,
        averageFps,
        slowFrameRatio: state.frames ? state.slowFrames / state.frames : 0,
        samples: state.samples,
      };
    },
  };
})();
