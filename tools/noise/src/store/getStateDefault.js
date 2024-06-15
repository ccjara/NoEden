const WIDTH = 256;
const HEIGHT = 256;
const AREA = WIDTH * HEIGHT;

export const getStateDefault = () => ({
  seed: 0,
  mapSize: { width: WIDTH, height: HEIGHT },
  pipeline: {
    noiseHover: null,
  },
  height: {
    map: new Float32Array(AREA),
    options: {
      octaves: 4,
      frequency: 1,
      amplification: 1,
      persistence: 0.5,
      lacunarity: 2,
      offsetX: 0,
      offsetY: 0,
    },
  },
  temperature: {
    map: new Float32Array(AREA),
    options: {
      minTemperature: -60,
      maxTemperature: 50,
      minEquatorTemperature: 25,
      maxEquatorTemperature: 30,
      altitudeFactor: 0.2,
      poles: true,
    },
  },
  moisture: {
    map: new Float32Array(AREA),
    options: {
      temperatureMultiplier: 1.5,
      altitudeMultiplier: 0.05,
    },
  },
});
