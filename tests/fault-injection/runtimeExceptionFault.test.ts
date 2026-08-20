import { describe, expect, it } from "vitest";
import { previousRunWasUnclean } from "@/lib/crashRecovery";
import { installMemoryBrowser } from "../fixtures/testStorage";

describe("Fault Injection — Runtime Recovery & Flags", () => {
  it("correctly flags unclean runs when crash flag indicates clean: false", () => {
    const storage = installMemoryBrowser();
    storage.setItem(
      "polygonal-primordials.runtime.flag.v1",
      JSON.stringify({ startedAt: new Date().toISOString(), clean: false })
    );

    expect(previousRunWasUnclean()).toBe(true);
  });

  it("correctly flags clean runs when clean: true", () => {
    const storage = installMemoryBrowser();
    storage.setItem(
      "polygonal-primordials.runtime.flag.v1",
      JSON.stringify({ startedAt: new Date().toISOString(), clean: true })
    );

    expect(previousRunWasUnclean()).toBe(false);
  });

  it("handles corrupted crash flag JSON safely and returns false", () => {
    const storage = installMemoryBrowser();
    storage.setItem("polygonal-primordials.runtime.flag.v1", "CORRUPTED_VALUE");

    expect(previousRunWasUnclean()).toBe(false);
  });
});
