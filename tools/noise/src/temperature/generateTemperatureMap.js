import { useStore } from '../store/store.js';
import alea from 'alea';
import { generateHumidityMap } from '../humidity/generateHumidityMap.js';
import { clamp } from '../lib/clamp.js';

export function generateTemperatureMap() {
  const state = useStore.getState();
  const prng = alea(state.seed);
  const options = state.temperature.options;
  const altitudeOptions = state.height.options.altitude;

  const equatorTemp =
    prng() * (options.maxEquatorTemperature - options.minEquatorTemperature) +
    options.minEquatorTemperature;

  const map = new Float32Array(state.mapSize.width * state.mapSize.height);
  const halfMapHeight = state.mapSize.height * 0.5;

  const mountainAltitude = altitudeOptions.hills + 1;

  for (let x = 0; x < state.mapSize.width; x++) {
    for (let y = 0; y < state.mapSize.height; y++) {
      const index = y * state.mapSize.width + x;
      const altitude = state.height.map[index];

      // calculate latitude drop
      let latitudeDrop = 0;
      if (options.latitudeDropoff) {
        const normalizedLat = Math.abs(y - halfMapHeight) / halfMapHeight;

        latitudeDrop = equatorTemp * normalizedLat;
      }

      // calculate altitude drop which is more extreme in mountains
      let altitudeDrop;
      const linearAltitudeDrop = options.altitudeFactor * altitude;

      if (altitude > altitudeOptions.hills) {
        altitudeDrop =
          linearAltitudeDrop +
          (altitude - altitudeOptions.hills) * options.altitudeFactor * 7.77;
      } else {
        altitudeDrop = linearAltitudeDrop;
      }

      map[index] = clamp(
        equatorTemp - (altitudeDrop + latitudeDrop),
        options.minTemperature,
        options.maxTemperature,
      );
    }
  }

  useStore.setState({
    temperature: {
      ...state.temperature,
      map,
    },
  });

  generateHumidityMap();
}
