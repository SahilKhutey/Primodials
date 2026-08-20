import { afterEach, describe, expect, it } from "vitest";
import { installMemoryBrowser } from "../fixtures/testStorage";

afterEach(() => {
  Reflect.deleteProperty(globalThis, "window");
  Reflect.deleteProperty(globalThis, "localStorage");
});

describe("safeStorage", () => {
  it("reads and writes valid JSON", async () => {
    installMemoryBrowser();
    const { safeGetJSON, safeSetJSON } = await import("@/lib/safeStorage");

    expect(safeSetJSON("x", { value: 123 })).toBe(true);
    expect(safeGetJSON<{ value: number }>("x")).toEqual({ value: 123 });
  });

  it("returns null for malformed JSON", async () => {
    const storage = installMemoryBrowser();
    storage.setItem("bad", "{not-json");
    const { safeGetJSON } = await import("@/lib/safeStorage");

    expect(safeGetJSON("bad")).toBeNull();
  });

  it("handles storage failures safely", async () => {
    const storage = installMemoryBrowser();
    storage.setItem("bad", "x");
    Object.defineProperty(storage, "getItem", {
      configurable: true,
      value: () => {
        throw new Error("storage blocked");
      },
    });

    const { safeGetJSON, safeGetString } = await import("@/lib/safeStorage");
    expect(safeGetJSON("bad")).toBeNull();
    expect(safeGetString("bad")).toBeNull();
  });
});
