import { useStore } from '../store/store.js';
import { generateBiomeMap } from '../biome/generateBiomeMap.js';
import { generateNoise } from '../generateNoise.js';

export function generatePrecipitationMap() {
  const state = useStore.getState();
  const { altitudeMultiplier, noise: noiseOptions } =
    state.precipitation.options;

  const map = generateNoise({
    octaves: noiseOptions.octaves,
    frequency: noiseOptions.frequency,
    amplification: noiseOptions.amplification,
    persistence: noiseOptions.persistence,
    lacunarity: noiseOptions.lacunarity,
    offsetX: noiseOptions.offsetX,
    offsetY: noiseOptions.offsetY,
    width: state.mapSize.width,
    height: state.mapSize.height,
    seed: state.seed,
  });

  const temperatureMap = state.temperature.map;
  const altitudeMap = state.height.map;

  /*
  for (let x = 0; x < state.mapSize.width; x++) {
    for (let y = 0; y < state.mapSize.height; y++) {
      const index = y * state.mapSize.width + x;

      map[index] -= temperatureMap * altitudeMap[index];
    }
  }
   */

  useStore.setState({
    precipitation: {
      ...state.precipitation,
      map,
    },
  });

  generateBiomeMap();
}
