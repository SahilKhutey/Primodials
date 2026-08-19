import { readFileSync } from "node:fs";
import { spawnSync } from "node:child_process";

// This CLI is intentionally a wrapper placeholder: browser/Vite aliases are project-specific.
// Use `npm run soak` after wiring this to your repository's test environment.
// The script fails loudly rather than pretending the soak test ran.
const result = spawnSync("npm", ["run", "test", "--", "run", "soak"], {
  stdio: "inherit",
  shell: process.platform === "win32",
});

if (result.status !== 0) {
  process.exit(result.status ?? 1);
}
