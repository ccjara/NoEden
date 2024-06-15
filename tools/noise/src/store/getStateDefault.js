export const getStateDefault = () => ({
  seed: 0,
  mapSize: { width: 256, height: 256 },
  pipeline: {
    noiseHover: null,
  },
  height: {
    map: new Float32Array(256 * 256),
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
    map: new Float32Array(256 * 256),
    options: {
      minTemperature: -60,
      maxTemperature: 50,
      minEquatorTemperature: 25,
      maxEquatorTemperature: 30,
      altitudeFactor: 0.2,
      poles: true,
    },
  },
});
