import { useStore } from '../store/store.js';
import { BiomeType } from './biomeCategories.js';

export function generateBiomeMap() {
  const state = useStore.getState();

  const biomeMap = [];
  const temperatureMap = state.temperature.map;
  const altitudeMap = state.height.map;
  const humidityMap = state.humidity.map;

  const oceanAltitude = state.height.options.altitude.ocean;
  const shoreAltitude = state.height.options.altitude.shore;
  const mountainAltitude = state.height.options.altitude.hills + 1;

  for (let x = 0; x < state.mapSize.width; x++) {
    for (let y = 0; y < state.mapSize.height; y++) {
      const index = y * state.mapSize.width + x;
      const altitude = altitudeMap[index];
      const temp = temperatureMap[index];
      const humidity = humidityMap[index];

      if (altitude <= oceanAltitude) {
        biomeMap[index] = BiomeType.Ocean;
        continue;
      }
      if (altitude <= shoreAltitude) {
        biomeMap[index] = BiomeType.Coastal;
        continue;
      }
      if (altitude >= mountainAltitude) {
        biomeMap[index] = BiomeType.Alpine;
        continue;
      }

      if (temp > 20 && humidity > 0.5) {
        biomeMap[index] = BiomeType.Rainforest;
      } else if (temp > 25 && humidity > 0.4) {
        biomeMap[index] = BiomeType.Savanna;
      } else if (temp > 15 && humidity > 0.5) {
        biomeMap[index] = BiomeType.TemperateForest;
      } else if (temp > 10 && humidity > 0.3) {
        biomeMap[index] = BiomeType.Grassland;
      } else if (temp > 30 && humidity < 0.3) {
        biomeMap[index] = BiomeType.Desert;
      } else if (temp > 20 && humidity < 0.3) {
        biomeMap[index] = BiomeType.Steppe;
      } else if (temp < 10 && humidity > 0.4) {
        biomeMap[index] = BiomeType.Taiga;
      } else if (temp < 10 && humidity < 0.3) {
        biomeMap[index] = BiomeType.Tundra;
      } else if (humidity > 0.8) {
        biomeMap[index] = BiomeType.Swamp;
      } else if (humidity > 0.6) {
        biomeMap[index] = BiomeType.Marsh;
      }
    }
  }

  useStore.setState({
    biome: {
      ...state.biome,
      map: biomeMap,
    },
  });
}
