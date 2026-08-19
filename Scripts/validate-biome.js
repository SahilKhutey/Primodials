#!/usr/bin/env node
/**
 * Primordials — Biome JSON Validator
 *
 * Usage:
 *   node scripts/validate-biome.js biomes/my-biome.json
 *
 * Exits 0 if valid, 1 if errors found.
 */

import fs from 'node:fs';
import path from 'node:path';

const BRAND_PALETTE = new Set([
  '#0F3D3E', '#0A0908', '#FF4FA3', '#F4F1DE', '#A8B5A0', '#1A4D4E',
  '#4ECDC4', '#FF6B6B', '#FFD93D', '#6BCB77', '#F18F01', '#D62246',
  '#1B998B', '#FFE66D', '#4A4E69', '#9B5DE5', '#FF9F1C', '#E63946',
  '#4D9DE0', '#E15554', '#048A81', '#C73E1D', '#4B1D3F', '#FFB627'
]);

const REQUIRED_TOP = ['id', 'version', 'name', 'description', 'author', 'tags', 'world', 'species', 'rules'];
const REQUIRED_SPECIES = ['id', 'name', 'type', 'color', 'shape', 'size', 'initial', 'behavior', 'stats'];
const VALID_TYPES = ['producer', 'grazer', 'predator', 'scavenger', 'omnivore'];
const VALID_SHAPES = ['circle', 'triangle', 'hexagon', 'square', 'star'];

const errors = [];
const warnings = [];

function fail(msg) {
  errors.push(msg);
  console.error(`❌ ${msg}`);
}

function warn(msg) {
  warnings.push(msg);
  console.warn(`⚠️ ${msg}`);
}

function pass(msg) {
  console.log(`✓ ${msg}`);
}

function isHexColor(c) {
  return typeof c === 'string' && /^#[0-9A-Fa-f]{6}$/.test(c);
}

function validate(file) {
  console.log(`\n🌿 Validating ${path.basename(file)}...\n`);
  let biome;

  try {
    biome = JSON.parse(fs.readFileSync(file, 'utf-8'));
  } catch (e) {
    fail(`Invalid JSON: ${e.message}`);
    return false;
  }

  // Top-level fields
  for (const field of REQUIRED_TOP) {
    if (!(field in biome)) fail(`Missing required field: '${field}'`);
  }

  if (!/^[a-z0-9-]+$/.test(biome.id || '')) {
    fail(`'id' must be kebab-case (lowercase, hyphens): "${biome.id}"`);
  }

  // World
  if (biome.world) {
    if (typeof biome.world.width !== 'number') fail('world.width must be a number');
    if (typeof biome.world.height !== 'number') fail('world.height must be a number');
  }

  // Species
  const speciesIds = new Set();
  let totalInitial = 0;

  (biome.species || []).forEach((s, i) => {
    for (const field of REQUIRED_SPECIES) {
      if (!(field in s)) fail(`species[${i}] missing '${field}'`);
    }

    if (speciesIds.has(s.id)) fail(`Duplicate species id: '${s.id}'`);
    speciesIds.add(s.id);

    if (!VALID_TYPES.includes(s.type)) {
      fail(`species[${i}] invalid type '${s.type}'. Must be one of: ${VALID_TYPES.join(', ')}`);
    }

    if (!VALID_SHAPES.includes(s.shape)) {
      fail(`species[${i}] invalid shape '${s.shape}'. Must be one of: ${VALID_SHAPES.join(', ')}`);
    }

    if (!isHexColor(s.color)) {
      fail(`species[${i}] color '${s.color}' is not a valid hex`);
    } else if (!BRAND_PALETTE.has(s.color.toUpperCase())) {
      warn(`species[${i}] color '${s.color}' is not from brand palette`);
    }

    if (s.initial > 5000) warn(`species[${i}] initial population ${s.initial} is very high`);
    totalInitial += s.initial || 0;
  });

  // Second pass: diet references
  (biome.species || []).forEach((s, i) => {
    const diet = s.behavior?.feeding?.diet || [];
    diet.forEach(d => {
      if (!speciesIds.has(d)) {
        fail(`species[${i}] diet references unknown species: '${d}'`);
      }
    });
  });

  // Carrying capacity
  if (biome.rules?.carryingCapacity != null && totalInitial > biome.rules.carryingCapacity * 2) {
    warn(`Total initial population (${totalInitial}) exceeds 2× carrying capacity (${biome.rules.carryingCapacity})`);
  }

  // Visuals
  if (biome.visuals) {
    if (biome.visuals.background && !isHexColor(biome.visuals.background)) {
      fail(`visuals.background must be a hex color`);
    }
    if (biome.visuals.gradient) {
      biome.visuals.gradient.forEach((c, i) => {
        if (!isHexColor(c)) fail(`visuals.gradient[${i}] '${c}' is not a hex`);
      });
    }
  }

  // Summary
  console.log(`\n${'─'.repeat(50)}`);
  if (errors.length === 0) {
    pass(`Schema valid`);
    pass(`All ${speciesIds.size} species have unique IDs`);
    pass(`All diet references resolve`);
    pass(`Total initial population: ${totalInitial}`);
    if (warnings.length === 0) {
      console.log('\n🎉 Biome is ready to submit!\n');
    } else {
      console.log(`\n✅ Biome is valid with ${warnings.length} warning(s).\n`);
    }
    return true;
  } else {
    console.log(`\n❌ ${errors.length} error(s) found. Please fix and re-validate.\n`);
    return false;
  }
}

const files = process.argv.slice(2);
if (files.length === 0) {
  console.error('Usage: node scripts/validate-biome.js <biome.json> [more...]');
  process.exit(1);
}

let allValid = true;
files.forEach(f => {
  if (!validate(f)) allValid = false;
});

process.exit(allValid ? 0 : 1);
