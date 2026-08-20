import { spawnSync } from "node:child_process";

const commands = [
  ["typecheck", "npm", ["run", "typecheck"]],
  ["lint", "npm", ["run", "lint"]],
  ["test-matrix", "node", ["tools/run-test-matrix.mjs"]],
  ["benchmark", "node", ["tools/run-benchmark.mjs"]],
  ["web-build", "npm", ["run", "build"]],
  ["wallpaper-build", "npm", ["run", "build:wallpaper"]],
  ["wallpaper-package", "npm", ["run", "package:wallpaper"]],
  ["wallpaper-qa", "node", ["tools/verify-wallpaper-qa.mjs"]],
  ["release-preflight", "node", ["tools/validate-release.mjs"]],
  ["build-info", "node", ["tools/create-build-info.mjs"]],
  ["artifact-audit", "node", ["tools/validate-release-artifacts.mjs"]],
  ["release-manifest", "node", ["tools/generate-release-manifest.mjs"]],
];

for (const [name, command, args] of commands) {
  console.log(`\n=== RELEASE GATE: ${name.toUpperCase()} ===`);

  const result = spawnSync(
    command,
    args,
    {
      stdio: "inherit",
      shell: process.platform === "win32",
    },
  );

  if (result.status !== 0) {
    console.error(`RELEASE GATE FAILED: ${name}`);
    process.exit(result.status ?? 1);
  }
}

console.log("\nALL RELEASE GATES PASSED");
