import { useStore } from '../store/store.js';
import alea from 'alea';
import { createNoise2D } from 'simplex-noise';
import { floodFill } from '../lib/floodFill.js';
import { generateNoise } from '../generateNoise.js';
import { createDistanceMap } from './createDistanceMap.js';
import { generateTemperatureMap } from '../temperature/generateTemperatureMap.js';

export function generateHeightMap() {
  const state = useStore.getState();
  const options = state.height.options;
  const prng = alea(state.seed);
  const getNoise = createNoise2D(prng);

  const data = new Float32Array(state.mapSize.width * state.mapSize.height);

  for (let y = 0; y < state.mapSize.height; ++y) {
    for (let x = 0; x < state.mapSize.width; ++x) {
      let frequency = options.frequency;
      let maxAmp = 0;
      let amplitude = 1;
      let value = 0;

      let globalX = (x + options.offsetX) / state.mapSize.width;
      let globalY = (y + options.offsetY) / state.mapSize.height;

      for (let octave = 0; octave < options.octaves; octave++) {
        value += amplitude * getNoise(globalX * frequency, globalY * frequency);

        maxAmp += amplitude;

        frequency *= options.lacunarity;
        amplitude *= options.persistence;
      }

      data[y * state.mapSize.width + x] = value / maxAmp;
    }
  }

  const heightMap = new Uint8Array(state.mapSize.width * state.mapSize.height);

  const OCEAN = 0;
  const LAND = 10;

  for (let y = 0; y < state.mapSize.height; y++) {
    for (let x = 0; x < state.mapSize.width; x++) {
      const i = y * state.mapSize.width + x;
      heightMap[i] = data[i] > 0.25 ? LAND : OCEAN;
    }
  }

  const distanceMap = createDistanceMap(
    heightMap,
    state.mapSize.width,
    state.mapSize.height,
  );
  const maxDistance = Math.max(...distanceMap);

  // apply noise for elevation on landmass
  const elevationNoise = generateNoise({
    octaves: 4,
    frequency: 3,
    amplification: 2,
    persistence: 0.5,
    lacunarity: 2,
    offsetX: prng() * 1000,
    offsetY: prng() * 1000,
    width: state.mapSize.width,
    height: state.mapSize.height,
    seed: state.seed,
  });

  for (let y = 0; y < state.mapSize.height; y++) {
    for (let x = 0; x < state.mapSize.width; x++) {
      const i = y * state.mapSize.width + x;
      if (heightMap[i] !== LAND) {
        continue;
      }
      const distanceFactor = Math.sqrt(distanceMap[i] / maxDistance);
      heightMap[i] += elevationNoise[i] * options.altitude.max * distanceFactor;
    }
  }

  useStore.setState({
    height: {
      ...state.height,
      map: heightMap,
    },
  });

  generateTemperatureMap();
}
