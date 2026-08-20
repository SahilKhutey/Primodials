import { describe, expect, it } from "vitest";
import { decodeWorldShare, readWorldFromUrl } from "@/lib/worldShare";
import { installMemoryBrowser } from "../fixtures/testStorage";

describe("Fault Injection — URL & World Sharing", () => {
  it("gracefully returns null for truncated or corrupted base64 share payload", () => {
    expect(decodeWorldShare("PP1.NOT_VALID_BASE64_!!!")).toBeNull();
    expect(decodeWorldShare("PP1.=====")).toBeNull();
    expect(decodeWorldShare("PP1.")).toBeNull();
    expect(decodeWorldShare("")).toBeNull();
  });

  it("rejects unknown version prefix", () => {
    expect(decodeWorldShare("PP999.eyJzZWVkIjoxMjM0NX0=")).toBeNull();
    expect(decodeWorldShare("MALICIOUS.PAYLOAD")).toBeNull();
  });

  it("handles corrupted JSON inside valid base64 payload", () => {
    const invalidJsonBase64 = Buffer.from("{seed: 123, not real json").toString("base64");
    expect(decodeWorldShare(`PP1.${invalidJsonBase64}`)).toBeNull();
  });

  it("clamps out-of-range numeric settings in URL payload without throwing", () => {
    const extremePayload = {
      seed: 9999999999,
      settings: {
        worldWidth: 99999999,
        maxPopulation: -500,
        mutationRate: 999,
        boundaryMode: "invalid_mode"
      }
    };
    const b64 = Buffer.from(JSON.stringify(extremePayload)).toString("base64");
    const decoded = decodeWorldShare(`PP1.${b64}`);

    expect(decoded).not.toBeNull();
    expect(decoded?.seed).toBe(9999999999 >>> 0);
    expect(decoded?.settings.worldWidth).toBeLessThanOrEqual(20000);
    expect(decoded?.settings.maxPopulation).toBeGreaterThanOrEqual(1);
    expect(decoded?.settings.mutationRate).toBeLessThanOrEqual(1);
    expect(["wrap", "reflect", "open"]).toContain(decoded?.settings.boundaryMode);
  });

  it("readWorldFromUrl returns null when URL parameter is missing or broken", () => {
    installMemoryBrowser("https://primordials.test/?world=CORRUPT_CODE");
    expect(readWorldFromUrl()).toBeNull();
  });
});
