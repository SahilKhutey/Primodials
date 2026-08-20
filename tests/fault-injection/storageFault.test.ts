import { afterEach, describe, expect, it, vi } from "vitest";
import { safeGetJSON, safeSetJSON, safeGetString, safeSetString } from "@/lib/safeStorage";
import { saveLocalSnapshot, loadLocalSnapshot } from "@/sim/localSnapshot";
import { Simulation } from "@/sim/simulation";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { installMemoryBrowser } from "../fixtures/testStorage";

afterEach(() => {
  Reflect.deleteProperty(globalThis, "window");
  Reflect.deleteProperty(globalThis, "localStorage");
});

describe("Fault Injection — Storage Layer", () => {
  it("survives QuotaExceededError during snapshot save without crashing", () => {
    const storage = installMemoryBrowser();
    const sim = new Simulation(123, DEFAULT_SETTINGS);

    // Mock setItem throwing QuotaExceededError
    storage.setItem = vi.fn().mockImplementation(() => {
      const err = new Error("QuotaExceededError");
      err.name = "QuotaExceededError";
      throw err;
    });

    expect(() => saveLocalSnapshot(sim)).not.toThrow();
    expect(saveLocalSnapshot(sim)).toBe(false);
  });

  it("handles throwing getItem in safeGetJSON and returns null", () => {
    const storage = installMemoryBrowser();
    storage.getItem = vi.fn().mockImplementation(() => {
      throw new Error("SecurityError: Access to localStorage is denied");
    });

    expect(() => safeGetJSON("test-key")).not.toThrow();
    expect(safeGetJSON("test-key")).toBeNull();
    expect(safeGetString("test-key")).toBeNull();
  });

  it("handles throwing setItem in safeSetJSON and returns false", () => {
    const storage = installMemoryBrowser();
    storage.setItem = vi.fn().mockImplementation(() => {
      throw new Error("SecurityError: Access to localStorage is denied");
    });

    expect(() => safeSetJSON("test-key", { test: 123 })).not.toThrow();
    expect(safeSetJSON("test-key", { test: 123 })).toBe(false);
    expect(safeSetString("test-key", "raw")).toBe(false);
  });

  it("recovers gracefully from corrupted JSON in snapshot key", () => {
    const storage = installMemoryBrowser();
    storage.setItem("polygonal-primordials.snapshot.v1", "INVALID_NOT_JSON{[[{");

    expect(() => loadLocalSnapshot()).not.toThrow();
    expect(loadLocalSnapshot()).toBeNull();
  });
});
