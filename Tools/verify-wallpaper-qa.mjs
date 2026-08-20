import { existsSync, readFileSync } from "node:fs";
import { resolve } from "node:path";

const root = process.cwd();
const weDir = resolve(root, "dist-wallpaper-engine");
const projectJsonPath = resolve(weDir, "project.json");

console.log("=== WALLPAPER ENGINE ARTIFACT VALIDATION ===");

const checks = [];

if (!existsSync(weDir)) {
  checks.push({ name: "dist-wallpaper-engine directory exists", ok: false });
} else {
  checks.push({ name: "dist-wallpaper-engine directory exists", ok: true });
}

if (existsSync(projectJsonPath)) {
  try {
    const project = JSON.parse(readFileSync(projectJsonPath, "utf8"));
    const titleOk = Boolean(project.title && project.title.includes("Primordials"));
    const fileOk = Boolean(project.file && existsSync(resolve(weDir, project.file)));
    const previewOk = Boolean(project.preview && existsSync(resolve(weDir, project.preview)));

    checks.push({ name: "project.json title is valid", ok: titleOk, detail: project.title });
    checks.push({ name: "project.json entrypoint HTML exists", ok: fileOk, detail: project.file });
    checks.push({ name: "project.json preview asset exists", ok: previewOk, detail: project.preview });
  } catch (e) {
    checks.push({ name: "project.json parse", ok: false, detail: e.message });
  }
} else {
  checks.push({ name: "project.json exists", ok: false });
}

console.table(checks);

const failed = checks.filter(c => !c.ok);
if (failed.length > 0) {
  console.error(`Wallpaper Engine QA validation failed with ${failed.length} error(s). Run npm run package:wallpaper first.`);
  process.exit(1);
} else {
  console.log("✓ All Wallpaper Engine packaging checks passed.");
}
