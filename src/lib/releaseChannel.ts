export type ReleaseChannel = "development" | "qa" | "beta" | "stable";

export const RELEASE_CHANNEL: ReleaseChannel =
  (import.meta.env.VITE_RELEASE_CHANNEL as ReleaseChannel | undefined) ?? "development";

export const IS_STABLE = RELEASE_CHANNEL === "stable";
export const IS_QA = RELEASE_CHANNEL === "qa";
export const IS_BETA = RELEASE_CHANNEL === "beta";

export function isReleaseChannel(value: string): value is ReleaseChannel {
  return value === "development" || value === "qa" || value === "beta" || value === "stable";
}
