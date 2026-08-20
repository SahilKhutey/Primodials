import { execFileSync } from "node:child_process";
import { existsSync, readFileSync, writeFileSync } from "node:fs";
import { resolve } from "node:path";

const root = process.cwd();
const pkg = JSON.parse(readFileSync(resolve(root, "package.json"), "utf8"));

function git(args) {
  try {
    return execFileSync("git", args, { cwd: root, stdio: ["ignore", "pipe", "ignore"], encoding: "utf8" }).trim();
  } catch {
    return null;
  }
}

const releaseDir = resolve(root, "release");
if (!existsSync(releaseDir)) throw new Error("release directory not found");

const buildInfo = {
  product: "Polygonal Primordials",
  packageName: pkg.name ?? null,
  packageVersion: pkg.version ?? null,
  commit: git(["rev-parse", "HEAD"]),
  branch: git(["branch", "--show-current"]),
  dirty: Boolean(git(["status", "--porcelain"])),
  builtAt: new Date().toISOString(),
  node: process.version,
};

writeFileSync(resolve(releaseDir, "build-info.json"), JSON.stringify(buildInfo, null, 2));
console.log(JSON.stringify(buildInfo, null, 2));
