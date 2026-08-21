import { existsSync, readFileSync } from "node:fs";
import { spawnSync } from "node:child_process";

const [channel = process.env.ITCH_CHANNEL || "windows-beta", buildPath = process.env.ITCH_BUILD_PATH] =
  process.argv.slice(2);

const target = process.env.ITCH_TARGET;

if (!buildPath) {
  console.error("Usage: node tools/publish-itch.mjs <channel> <build-path>");
  process.exit(2);
}

if (!existsSync(buildPath)) {
  console.error(`Build path does not exist: ${buildPath}`);
  process.exit(2);
}

if (!target) {
  console.error("Missing ITCH_TARGET, e.g. yourname/polygonal-primordials");
  process.exit(2);
}

const pkg = JSON.parse(readFileSync("package.json", "utf8"));
const version = pkg.version ?? "unknown";

console.log(`Publishing ${version} to ${target}:${channel}`);

const result = spawnSync(
  "butler",
  ["push", buildPath, `${target}:${channel}`, "--userversion", version],
  { stdio: "inherit", shell: process.platform === "win32" },
);

process.exit(result.status ?? 1);
