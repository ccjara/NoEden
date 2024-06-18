import { MAX_ALTITUDE, MIN_ALTITUDE } from '../lib/technicalLimits.js';

const WIDTH = 256;
const HEIGHT = 256;

export const getStateDefault = () => ({
  seed: 0,
  mapSize: { width: WIDTH, height: HEIGHT },
  pipeline: {
    noiseHover: null,
  },
  height: {
    options: {
      island: false,

      octaves: 4,
      frequency: 1,
      amplification: 1,
      persistence: 0.5,
      lacunarity: 2,
      offsetX: 0,
      offsetY: 0,

      altitude: {
        ocean: 9,
        shore: 19,
        plains: 49,
        hills: 79,
        min: MIN_ALTITUDE,
        max: MAX_ALTITUDE,
      },
    },
  },
  temperature: {
    options: {
      minTemperature: -60,
      maxTemperature: 50,
      minEquatorTemperature: 25,
      maxEquatorTemperature: 30,
      altitudeFactor: 0.2,
      latitudeDropoff: true,
    },
  },
  humidity: {
    options: {
      temperatureMultiplier: 1.5,
      altitudeMultiplier: 0.05,
    },
  },
  precipitation: {
    options: {
      altitudeMultiplier: 0.01,

      noise: {
        octaves: 4,
        frequency: 1,
        amplification: 1,
        persistence: 0.5,
        lacunarity: 2,
        offsetX: 0,
        offsetY: 0,
      },
    },
  },
  biome: {
    map: [],
    options: {},
  },
  visualizer: {
    collapsed: false,
  },
});
