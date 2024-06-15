import { createNoise2D } from 'simplex-noise';
import alea from 'alea';
import { clamp } from './lib/clamp.js';

export const defaultNoiseOptions = {
  octaves: 4,
  frequency: 1,
  persistence: 0.5,
  lacunarity: 2,
  offsetX: 0,
  offsetY: 0,
  width: 256,
  height: 256,
  amplification: 1,
  seed: 0,
};

export function generateNoise(options) {
  options = { ...defaultNoiseOptions, ...options };

  const getNoise = createNoise2D(alea(options.seed));

  const data = new Float32Array(options.width * options.height);

  let minValue = Infinity;
  let maxValue = -Infinity;

  for (let y = 0; y < options.height; ++y) {
    for (let x = 0; x < options.width; ++x) {
      let frequency = options.frequency;
      let amplitude = 1.0;
      let value = 0;

      let globalX = (x + options.offsetX) / options.width;
      let globalY = (y + options.offsetY) / options.height;

      for (let octave = 0; octave < options.octaves; octave++) {
        value += amplitude * getNoise(globalX * frequency, globalY * frequency);

        frequency *= options.lacunarity;
        amplitude *= options.persistence;
      }

      minValue = Math.min(minValue, value);
      maxValue = Math.max(maxValue, value);

      data[y * options.width + x] = value;
    }
  }

  // normalize to [0, 1]
  for (let i = 0; i < data.length; i++) {
    data[i] = (data[i] - minValue) / (maxValue - minValue);

    if (options.amplification !== 1) {
      data[i] = Math.pow(data[i], options.amplification);
    }

    // data[i] *= Math.abs(Math.sin(data[i] * Math.PI * 1.5));
  }

  return { data, minValue, maxValue };
}
