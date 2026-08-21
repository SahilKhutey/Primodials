import { existsSync, readFileSync, writeFileSync } from "node:fs";
import { resolve } from "node:path";

const root = process.cwd();
const pkg = JSON.parse(readFileSync(resolve(root, "package.json"), "utf8"));
const failures = [];
const warnings = [];

for (const file of [
  "README.md",
  "LICENSE",
  "CHANGELOG.md",
  "release/RELEASE_CANDIDATE_CHECKLIST.md",
]) {
  if (!existsSync(resolve(root, file))) failures.push(`Missing ${file}`);
}

if (!pkg.name) failures.push("package.json name is missing");
if (!pkg.version) failures.push("package.json version is missing");
if (!pkg.scripts?.test) failures.push("npm test script missing");

if (!pkg.scripts?.["package:all"]) {
  warnings.push("package:all is missing; use the repository's platform-specific packaging commands.");
}

const report = {
  product: "Polygonal Primordials",
  generatedAt: new Date().toISOString(),
  version: pkg.version ?? null,
  ok: failures.length === 0,
  failures,
  warnings,
};

const releaseDir = resolve(root, "release");
if (existsSync(releaseDir)) {
  writeFileSync(resolve(releaseDir, "publish-preflight.json"), JSON.stringify(report, null, 2));
}

console.log(JSON.stringify(report, null, 2));
if (failures.length) process.exit(1);
