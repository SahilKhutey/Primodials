import { spawnSync } from "node:child_process";

const commands = [
  ["typecheck", "npm", ["run", "typecheck"]],
  ["lint", "npm", ["run", "lint"]],
  ["test-matrix", "node", ["Tools/run-test-matrix.mjs"]],
  ["benchmark", "node", ["Tools/run-benchmark.mjs"]],
  ["web-build", "npm", ["run", "build"]],
  ["wallpaper-build", "npm", ["run", "build:wallpaper"]],
  ["wallpaper-package", "npm", ["run", "package:wallpaper"]],
  ["wallpaper-qa", "node", ["Tools/verify-wallpaper-qa.mjs"]],
  ["release-preflight", "node", ["Tools/validate-release.mjs"]],
  ["build-info", "node", ["Tools/create-build-info.mjs"]],
  ["artifact-audit", "node", ["Tools/validate-release-artifacts.mjs"]],
  ["release-manifest", "node", ["Tools/generate-release-manifest.mjs"]],
];

for (const [name, command, args] of commands) {
  console.log(`\n=== RELEASE GATE: ${name.toUpperCase()} ===`);

  const result = spawnSync(
    command,
    args,
    {
      stdio: "inherit",
      shell: true,
    },
  );

  if (result.status !== 0) {
    console.error(`RELEASE GATE FAILED: ${name}`);
    process.exit(result.status ?? 1);
  }
}

console.log("\nALL RELEASE GATES PASSED");
