/**
 * PerformanceHud — overlay showing FPS / frame / sim / population
 *
 * Toggleable via keyboard shortcut (default: backtick) or visibility prop.
 *
 * License: MIT
 */

import { useEffect, useState } from "react";
import type { PerformanceMonitor, PerformanceSnapshot } from "@/lib/performanceMonitor";

export interface PerformanceHudProps {
  monitor?: PerformanceMonitor;
  sample?: PerformanceSnapshot | null;
  visible?: boolean;
  enabled?: boolean;
  toggleKey?: string;
  position?: "top-right" | "top-left" | "bottom-right" | "bottom-left";
}

const positionStyles: Record<NonNullable<PerformanceHudProps["position"]>, string> = {
  "top-right": "top-3 right-3",
  "top-left": "top-3 left-3",
  "bottom-right": "bottom-3 right-3",
  "bottom-left": "bottom-3 left-3",
};

export function PerformanceHud({
  monitor,
  sample,
  visible = true,
  enabled = true,
  toggleKey = "`",
  position = "top-right",
}: PerformanceHudProps) {
  const [snapshot, setSnapshot] = useState<PerformanceSnapshot | null>(sample ?? null);
  const [isVisible, setIsVisible] = useState(visible && enabled);

  useEffect(() => {
    setIsVisible(visible && enabled);
  }, [visible, enabled]);

  useEffect(() => {
    if (sample) {
      setSnapshot(sample);
    }
  }, [sample]);

  useEffect(() => {
    if (!isVisible || !monitor) return;
    const id = window.setInterval(() => {
      setSnapshot(monitor.getSnapshot());
    }, 250);
    return () => window.clearInterval(id);
  }, [monitor, isVisible]);

  useEffect(() => {
    const handler = (e: KeyboardEvent) => {
      if (e.key === toggleKey && !(e.target instanceof HTMLInputElement) && !(e.target instanceof HTMLTextAreaElement)) {
        setIsVisible((v) => !v);
      }
    };
    window.addEventListener("keydown", handler);
    return () => window.removeEventListener("keydown", handler);
  }, [toggleKey]);

  if (!isVisible || !snapshot) return null;

  return (
    <div
      className={`fixed ${positionStyles[position]} z-50 pointer-events-none font-mono text-xs`}
      aria-hidden="true"
    >
      <div className="rounded-xl border border-white/10 bg-black/60 px-3 py-2 text-emerald-300 shadow-xl backdrop-blur-md">
        <div className="flex gap-3">
          <span>
            <span className="text-white/60">FPS</span>{" "}
            <span className="font-bold text-white">{snapshot.fps}</span>
          </span>
          <span>
            <span className="text-white/60">FRAME</span>{" "}
            <span>{snapshot.frameMs}ms</span>
          </span>
        </div>
        <div className="flex gap-3 mt-0.5">
          <span>
            <span className="text-white/60">SIM</span>{" "}
            <span>{snapshot.simStepsPerSec}/s</span>
          </span>
          <span>
            <span className="text-white/60">POP</span>{" "}
            <span>{snapshot.population}</span>
          </span>
        </div>
        {snapshot.p99FrameMs > snapshot.frameMs * 2 && (
          <div className="mt-1 text-[10px] text-amber-300">
            P99 {snapshot.p99FrameMs}ms (jitter)
          </div>
        )}
      </div>
    </div>
  );
}
