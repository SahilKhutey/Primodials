import { createHash } from "node:crypto";
import { existsSync, readFileSync, readdirSync, statSync, writeFileSync } from "node:fs";
import { join, relative, resolve } from "node:path";

const root = process.cwd();
const artifactsDir = resolve(root, "release", "artifacts");
if (!existsSync(artifactsDir)) {
  throw new Error("release/artifacts does not exist. Put final distributables there first.");
}

const files = [];

function walk(dir) {
  for (const name of readdirSync(dir)) {
    const path = join(dir, name);
    const stat = statSync(path);
    if (stat.isDirectory()) walk(path);
    else {
      const bytes = readFileSync(path);
      files.push({
        path: relative(root, path).replaceAll("\\", "/"),
        size: stat.size,
        sha256: createHash("sha256").update(bytes).digest("hex"),
      });
    }
  }
}

walk(artifactsDir);

const manifest = {
  product: "Polygonal Primordials",
  generatedAt: new Date().toISOString(),
  artifactCount: files.length,
  files,
};

writeFileSync(resolve(root, "release", "manifest.json"), JSON.stringify(manifest, null, 2));
console.log(`Generated release manifest for ${files.length} artifact(s).`);
