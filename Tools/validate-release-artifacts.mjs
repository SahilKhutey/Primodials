import { existsSync, readFileSync, readdirSync } from "node:fs";
import { resolve } from "node:path";

const root = process.cwd();
console.log("=== LAYER 10: RELEASE ARTIFACT INTEGRITY AUDIT ===");

const checks = [];

// 1. Check dist (Web retail build)
const distDir = resolve(root, "dist");
if (existsSync(distDir)) {
  const indexHtml = existsSync(resolve(distDir, "index.html"));
  const assetsDir = resolve(distDir, "assets");
  const hasAssets = existsSync(assetsDir) && readdirSync(assetsDir).length > 0;
  checks.push({ name: "Web Dist index.html", ok: indexHtml, detail: resolve(distDir, "index.html") });
  checks.push({ name: "Web Dist compiled JS/CSS assets", ok: hasAssets, detail: `${assetsDir} (${hasAssets ? readdirSync(assetsDir).length : 0} files)` });
} else {
  checks.push({ name: "Web Dist directory", ok: false, detail: "Run npm run build first" });
}

// 2. Check dist-wallpaper (Wallpaper build)
const wpDir = resolve(root, "dist-wallpaper");
if (existsSync(wpDir)) {
  const indexHtml = existsSync(resolve(wpDir, "index.html"));
  checks.push({ name: "Wallpaper Dist index.html", ok: indexHtml, detail: resolve(wpDir, "index.html") });
} else {
  checks.push({ name: "Wallpaper Dist directory", ok: false, detail: "Run npm run build:wallpaper first" });
}

// 3. Check dist-wallpaper-engine (Workshop Package)
const weDir = resolve(root, "dist-wallpaper-engine");
if (existsSync(weDir)) {
  const projectJson = existsSync(resolve(weDir, "project.json"));
  checks.push({ name: "Wallpaper Engine Workshop project.json", ok: projectJson, detail: resolve(weDir, "project.json") });
} else {
  checks.push({ name: "Wallpaper Engine Workshop directory", ok: false, detail: "Run npm run package:wallpaper first" });
}

// 4. Check release metadata
const buildInfoPath = resolve(root, "release", "build-info.json");
const preflightPath = resolve(root, "release", "preflight.json");
checks.push({ name: "Release build-info.json", ok: existsSync(buildInfoPath), detail: buildInfoPath });
checks.push({ name: "Release preflight.json", ok: existsSync(preflightPath), detail: preflightPath });

console.table(checks);

const failures = checks.filter(c => !c.ok);
if (failures.length > 0) {
  console.error(`\nArtifact validation failed with ${failures.length} issue(s).`);
  process.exit(1);
} else {
  console.log("\n✓ All release artifacts verified and ready for commercial distribution.");
}
