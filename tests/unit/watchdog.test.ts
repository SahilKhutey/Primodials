import { describe, expect, it, vi, beforeEach } from "vitest";
import { RuntimeWatchdog } from "@/lib/runtimeWatchdog";

describe("RuntimeWatchdog", () => {
  beforeEach(() => {
    vi.useFakeTimers();
    vi.setSystemTime(0);
  });

  it("detects sustained slow frames", () => {
    const watchdog = new RuntimeWatchdog(100, 2000, 3);

    expect(watchdog.frame(120, true)).toBeNull();
    expect(watchdog.frame(120, true)).toBeNull();

    const event = watchdog.frame(120, true);
    expect(event?.type).toBe("slow-frame");
    expect(watchdog.isUnhealthy()).toBe(true);
  });

  it("reports recovery", () => {
    const watchdog = new RuntimeWatchdog(100, 2000, 1);
    watchdog.frame(150, true);

    expect(watchdog.isUnhealthy()).toBe(true);

    const event = watchdog.frame(10, true);
    expect(event?.type).toBe("recovered");
    expect(watchdog.isUnhealthy()).toBe(false);
  });
});
