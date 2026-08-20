import { chromium } from "@playwright/test";
import { mkdirSync, writeFileSync } from "node:fs";
import { spawn } from "node:child_process";

const duration = Number(process.env.PP_BENCHMARK_SECONDS ?? 10);
const width = Number(process.env.PP_WIDTH ?? 1920);
const height = Number(process.env.PP_HEIGHT ?? 1080);
const baseURL = process.env.PP_BASE_URL ?? "http://127.0.0.1:4173";

mkdirSync("benchmarks", { recursive: true });

let serverProcess = null;
try {
  await fetch(`${baseURL}/`);
} catch {
  console.log("Starting preview server on port 4173...");
  serverProcess = spawn(
    process.platform === "win32" ? "npm.cmd" : "npm",
    ["run", "preview", "--", "--host", "127.0.0.1", "--port", "4173"],
    { stdio: "ignore", shell: process.platform === "win32" },
  );
  for (let i = 0; i < 30; i++) {
    try {
      await fetch(`${baseURL}/`);
      break;
    } catch {
      await new Promise((r) => setTimeout(r, 500));
    }
  }
}

try {
  const browser = await chromium.launch();
  const page = await browser.newPage({ viewport: { width, height } });

  await page.goto(`${baseURL}/?wallpaper=1`);
  await page.addScriptTag({ path: "benchmarks/browser-benchmark.js" });
  await page.evaluate(() => window.__PP_BENCHMARK__.start());

  await new Promise((resolve) => setTimeout(resolve, duration * 1000));

  const result = await page.evaluate(() => window.__PP_BENCHMARK__.stop());
  const report = {
    product: "Polygonal Primordials",
    baseURL,
    durationSeconds: duration,
    viewport: { width, height },
    ...result,
  };

  const output = process.env.PP_BENCHMARK_OUTPUT ?? "benchmarks/browser-result.json";
  writeFileSync(output, JSON.stringify(report, null, 2));
  console.log(JSON.stringify(report, null, 2));

  await browser.close();
} finally {
  if (serverProcess) {
    serverProcess.kill();
  }
}
