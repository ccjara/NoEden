import { useStore } from '../store/store.js';
import { clamp } from '../lib/clamp.js';

export function generateMoistureMap() {
  const state = useStore.getState();
  const { temperatureMultiplier, altitudeMultiplier } = state.moisture.options;

  const map = new Float32Array(state.mapSize.width * state.mapSize.height);
  const temperature = state.temperature.map;
  const altitude = state.height.map;

  for (let x = 0; x < state.mapSize.width; x++) {
    for (let y = 0; y < state.mapSize.height; y++) {
      const index = y * state.mapSize.width + x;

      let moisture = temperature[index] * temperatureMultiplier;

      moisture -= altitude[index] * altitudeMultiplier;

      map[index] = clamp(moisture, 0.0, 1.0);
    }
  }

  useStore.setState({
    moisture: {
      ...state.moisture,
      map,
    },
  });
}
