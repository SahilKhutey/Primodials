import { spawnSync } from "node:child_process";

const groups = [
  ["unit", "vitest", "run", "tests/unit"],
  ["simulation", "vitest", "run", "tests/simulation"],
  ["integration", "vitest", "run", "tests/integration"],
  ["fault-injection", "vitest", "run", "tests/fault-injection"],
  ["compatibility", "vitest", "run", "tests/compatibility"],
  ["publishing", "vitest", "run", "tests/publishing"],
  ["release", "vitest", "run", "tests/release"],
  ["all", "vitest", "run", "tests"],
];

let failed = false;

for (const [name, command, ...args] of groups) {
  console.log(`\n=== TEST GROUP: ${name.toUpperCase()} ===`);
  const result = spawnSync(
    "npx",
    [command, ...args],
    {
      stdio: "inherit",
      shell: true,
    },
  );

  if (result.status !== 0) {
    failed = true;
    console.error(`FAILED: ${name}`);
    break;
  }

  console.log(`PASSED: ${name}`);
}

process.exit(failed ? 1 : 0);
