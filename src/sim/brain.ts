import { Rng } from './rng';

// Lightweight feedforward neural network.
// Architecture: [nInputs] -> [nHidden] -> [nOutputs]
// Weights stored as flat Float32Array for speed.
// Layout: hiddenBias[nHidden] | hiddenW[nInputs*nHidden] | outBias[nOutputs] | outW[nHidden*nOutputs]

export type Brain = {
  nInputs: number;
  nHidden: number;
  nOutputs: number;
  weights: Float32Array; // full weight vector
};

export const N_INPUTS = 14;
export const N_OUTPUTS = 5;

export function brainSize(nInputs: number, nHidden: number, nOutputs: number): number {
  return nHidden + nInputs * nHidden + nOutputs + nHidden * nOutputs;
}

export function makeBrain(nInputs: number, nHidden: number, nOutputs: number, rng: Rng): Brain {
  const size = brainSize(nInputs, nHidden, nOutputs);
  const weights = new Float32Array(size);
  for (let i = 0; i < size; i++) weights[i] = rng.range(-1, 1);
  return { nInputs, nHidden, nOutputs, weights };
}

export function cloneBrain(b: Brain): Brain {
  return { ...b, weights: new Float32Array(b.weights) };
}

export function mutateBrain(b: Brain, rate: number, rng: Rng): Brain {
  const out = cloneBrain(b);
  const amt = 0.3;
  for (let i = 0; i < out.weights.length; i++) {
    if (rng.next() < rate) {
      out.weights[i] += rng.range(-amt, amt);
      out.weights[i] = Math.max(-3, Math.min(3, out.weights[i]));
    }
  }
  // Rare structural mutation: perturb a single weight strongly
  if (rng.next() < rate * 0.2) {
    const idx = rng.int(0, out.weights.length - 1);
    out.weights[idx] += rng.range(-1, 1);
    out.weights[idx] = Math.max(-3, Math.min(3, out.weights[idx]));
  }
  return out;
}

export function crossoverBrain(a: Brain, b: Brain, rng: Rng): Brain {
  if (a.nInputs !== b.nInputs || a.nHidden !== b.nHidden || a.nOutputs !== b.nOutputs) {
    return cloneBrain(a);
  }
  const child = cloneBrain(a);
  for (let i = 0; i < child.weights.length; i++) {
    child.weights[i] = rng.next() < 0.5 ? a.weights[i] : b.weights[i];
  }
  return child;
}

// Run inference. Returns outputs array (length nOutputs).
// Uses a pre-allocated hidden buffer to avoid GC pressure.
const hiddenBuf = new Float32Array(64);

export function evalBrain(b: Brain, inputs: Float32Array, outputs: Float32Array): void {
  const { nInputs, nHidden, nOutputs, weights } = b;
  let idx = 0;

  // Hidden layer: bias + weighted sum, then tanh
  for (let h = 0; h < nHidden; h++) {
    let sum = weights[idx++]; // hidden bias
    for (let i = 0; i < nInputs; i++) {
      sum += inputs[i] * weights[idx++];
    }
    hiddenBuf[h] = Math.tanh(sum);
  }

  // Output layer: bias + weighted sum from hidden, then tanh
  for (let o = 0; o < nOutputs; o++) {
    let sum = weights[idx++]; // output bias
    for (let h = 0; h < nHidden; h++) {
      sum += hiddenBuf[h] * weights[idx++];
    }
    outputs[o] = Math.tanh(sum);
  }
}

// Hidden layer size scales with intelligence tier
export function hiddenForIntel(intel: number): number {
  if (intel < 0.2) return 0;   // no brain — use instinct AI
  if (intel < 0.4) return 3;
  if (intel < 0.6) return 4;
  if (intel < 0.8) return 6;
  return 8;
}
