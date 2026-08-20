import { existsSync, readFileSync, writeFileSync } from "node:fs";
import { resolve } from "node:path";

const root = process.cwd();
const releaseDir = resolve(root, "release");
const requiredPaths = [
  "package.json",
  "README.md",
  "LICENSE",
  "src/App.tsx",
  "src/components/SimCanvas.tsx",
  "src/components/WallpaperDock.tsx",
  "src/sim/simulation.ts",
];

const failures = [];

for (const item of requiredPaths) {
  if (!existsSync(resolve(root, item))) failures.push(`Missing required path: ${item}`);
}

const pkgPath = resolve(root, "package.json");

if (existsSync(pkgPath)) {
  try {
    const pkg = JSON.parse(readFileSync(pkgPath, "utf8"));
    const scripts = pkg.scripts ?? {};

    for (const script of ["build", "build:wallpaper", "package:wallpaper", "typecheck", "lint"]) {
      if (!scripts[script]) failures.push(`Missing package script: ${script}`);
    }

    if (!scripts.test) {
      failures.push("Missing package script: test — add the Phase 1 Vitest setup before RC release.");
    }
  } catch {
    failures.push("package.json could not be parsed");
  }
}

if (!existsSync(releaseDir)) failures.push("Missing release directory");

const report = {
  product: "Polygonal Primordials",
  generatedAt: new Date().toISOString(),
  ok: failures.length === 0,
  failures,
};

if (existsSync(releaseDir)) {
  writeFileSync(resolve(releaseDir, "preflight.json"), JSON.stringify(report, null, 2));
}

console.log(JSON.stringify(report, null, 2));

if (failures.length) process.exit(1);
