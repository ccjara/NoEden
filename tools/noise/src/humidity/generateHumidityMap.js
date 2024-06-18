import { useStore } from '../store/store.js';
import { clamp } from '../lib/clamp.js';
import { generateBiomeMap } from '../biome/generateBiomeMap.js';
import { generatePrecipitationMap } from '../precipitation/generatePrecipitationMap.js';

export function generateHumidityMap() {
  const state = useStore.getState();
  const { temperatureMultiplier, altitudeMultiplier } = state.humidity.options;

  const map = new Float32Array(state.mapSize.width * state.mapSize.height);
  const temperatureMap = state.temperature.map;
  const temperatureOptions = state.temperature.options;
  const altitudeMap = state.height.map;
  const altitudeOptions = state.height.options.altitude;

  for (let x = 0; x < state.mapSize.width; x++) {
    for (let y = 0; y < state.mapSize.height; y++) {
      const index = y * state.mapSize.width + x;

      // TODO: needs to be the actual temperature chosen (by randomness) not the min temp
      const standardTemp = temperatureOptions.minEquatorTemperature;
      const temperature = temperatureMap[index];
      const altitude = altitudeMap[index];
      const temperatureEffect = 0.6 * (1 - 0.02 * (standardTemp - temperature));
      const altitudeEffect = 1; // 0.6 * (1 - altitudeMultiplier * altitude);

      map[index] = clamp(temperatureEffect * altitudeEffect, 0.0, 1.0);
    }
  }

  useStore.setState({
    humidity: {
      ...state.humidity,
      map,
    },
  });

  generatePrecipitationMap();
}
