const fs = require('fs');
const path = require('path');

const distDir = path.join(__dirname, '../../dist');
const targetDir = path.join(__dirname, '../../dist-wallpaper-engine');

if (!fs.existsSync(distDir)) {
  console.error('Error: dist/ directory not found. Please run "npm run build" first.');
  process.exit(1);
}

fs.rmSync(targetDir, { recursive: true, force: true });
fs.mkdirSync(targetDir, { recursive: true });

// Copy dist contents to dist-wallpaper-engine
fs.cpSync(distDir, targetDir, { recursive: true });

// Copy project.json to dist-wallpaper-engine
const projectJsonSrc = path.join(__dirname, 'project.json');
if (fs.existsSync(projectJsonSrc)) {
  fs.copyFileSync(projectJsonSrc, path.join(targetDir, 'project.json'));
}

console.log('✓ Successfully packaged for Wallpaper Engine in dist-wallpaper-engine/');
