import type { SimSettings } from "@/sim/types";

const SHARE_VERSION = 1;
const PREFIX = "PP1";

export type SharedWorld = {
  version: 1;
  seed: number;
  name?: string;
  settings: Partial<SimSettings>;
};

function encodeBase64Url(value: string): string {
  return btoa(unescape(encodeURIComponent(value)))
    .replace(/\+/g, "-")
    .replace(/\//g, "_")
    .replace(/=+$/g, "");
}

function decodeBase64Url(value: string): string {
  const padded = value.replace(/-/g, "+").replace(/_/g, "/").padEnd(
    value.length + ((4 - (value.length % 4)) % 4),
    "=",
  );
  return decodeURIComponent(escape(atob(padded)));
}

export function encodeWorldShare(world: SharedWorld): string {
  const payload = JSON.stringify(world);
  return `${PREFIX}.${encodeBase64Url(payload)}`;
}

export function decodeWorldShare(code: string): SharedWorld | null {
  try {
    const [prefix, payload] = code.trim().split(".");
    if (prefix !== PREFIX || !payload) return null;
    const parsed = JSON.parse(decodeBase64Url(payload)) as SharedWorld;
    if (parsed.version !== SHARE_VERSION) return null;
    if (!Number.isInteger(parsed.seed)) return null;
    return parsed;
  } catch {
    return null;
  }
}

export function createWorldShare(
  seed: number,
  settings: Partial<SimSettings>,
  name?: string,
): string {
  return encodeWorldShare({
    version: 1,
    seed: seed >>> 0,
    settings,
    name,
  });
}

export function shareToUrl(code: string): string {
  const url = new URL(window.location.href);
  url.searchParams.set("world", code);
  return url.toString();
}

export function readWorldFromUrl(): SharedWorld | null {
  const code = new URL(window.location.href).searchParams.get("world");
  return code ? decodeWorldShare(code) : null;
}
