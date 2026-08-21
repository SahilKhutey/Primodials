import { readFileSync, writeFileSync } from "node:fs";
import { resolve } from "node:path";

const pkg = JSON.parse(readFileSync(resolve(process.cwd(), "package.json"), "utf8"));

const manifest = {
  product: "Polygonal Primordials",
  version: pkg.version ?? null,
  shortDescription: "A living artificial ecosystem that evolves on your desktop.",
  genres: ["Simulation", "Sandbox"],
  primaryPlatforms: ["Windows"],
  launchChannels: ["Wallpaper Engine", "itch.io", "Steam"],
  claimsPolicy: "Only publish features verified in the exact release artifact.",
  createdAt: new Date().toISOString(),
};

writeFileSync(
  resolve(process.cwd(), "release/store-manifest.json"),
  JSON.stringify(manifest, null, 2),
);

console.log(JSON.stringify(manifest, null, 2));
