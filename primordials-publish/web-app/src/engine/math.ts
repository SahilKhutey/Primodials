/**
 * Math utilities for the simulation
 */

export const TAU = Math.PI * 2;

export function clamp(value: number, min: number, max: number): number {
  return Math.max(min, Math.min(max, value));
}

export function lerp(a: number, b: number, t: number): number {
  return a + (b - a) * t;
}

export function distance(
  a: { x: number; y: number },
  b: { x: number; y: number }
): number {
  const dx = a.x - b.x;
  const dy = a.y - b.y;
  return Math.sqrt(dx * dx + dy * dy);
}

export function normalizeAngle(angle: number): number {
  while (angle < 0) angle += TAU;
  while (angle >= TAU) angle -= TAU;
  return angle;
}

export function shortestAngleDiff(from: number, to: number): number {
  let diff = to - from;
  while (diff > Math.PI) diff -= TAU;
  while (diff < -Math.PI) diff += TAU;
  return diff;
}
