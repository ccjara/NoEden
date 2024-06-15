import { useStore } from '../store/store.js';
import alea from 'alea';
import { clamp } from '../lib/clamp.js';
import { generateMoistureMap } from '../moisture/generateMoistureMap.js';

export function generateTemperatureMap() {
  const state = useStore.getState();
  const prng = alea(state.seed);
  const options = state.temperature.options;

  const normalizeTemp = (t) =>
    (t - options.minTemperature) /
    (options.maxTemperature - options.minTemperature);

  const equatorTemp = normalizeTemp(
    prng() * (options.maxEquatorTemperature - options.minEquatorTemperature) +
      options.minEquatorTemperature,
  );

  const map = new Float32Array(state.mapSize.width * state.mapSize.height);
  const halfMapHeight = state.mapSize.height * 0.5;

  for (let x = 0; x < state.mapSize.width; x++) {
    for (let y = 0; y < state.mapSize.height; y++) {
      const index = y * state.mapSize.width + x;
      const normalizedLat = options.poles
        ? 1 - Math.abs(y - halfMapHeight) / halfMapHeight
        : 1;
      const height = state.height.map[index];

      const altitudeEffect = -options.altitudeFactor * height;
      const finalTemperature = equatorTemp * normalizedLat + altitudeEffect;

      map[index] = clamp(finalTemperature, 0.0, 1.0);
    }
  }

  useStore.setState({
    temperature: {
      ...state.temperature,
      map,
    },
  });

  generateMoistureMap();
}
