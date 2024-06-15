import { useStore } from './store.js';
import { generateWorld } from '../generateWorld.js';
import { generateTemperatureMap } from '../temperature/generateTemperatureMap.js';
import { getStateDefault } from './getStateDefault.js';
import { clamp } from '../lib/clamp.js';
import { generateMoistureMap } from '../moisture/generateMoistureMap.js';

export const reset = () => {
  useStore.setState(getStateDefault());
  generateWorld();
};

/**
 * @param {number} width
 * @param {number} height
 * @param {boolean} regenerate
 */
export const setMapSize = (width, height, regenerate) => {
  useStore.setState({ mapSize: { width, height } });

  regenerate && generateWorld();
};

/**
 * @param {number} seed
 * @param {boolean} regenerate
 */
export const setSeed = (seed, regenerate) => {
  useStore.setState({ seed });

  regenerate && generateWorld();
};

/**
 * @param {Float32Array} map
 */
export const setHeightMap = (map) => {
  useStore.setState({
    height: {
      ...useStore.getState().height,
      map,
    },
  });

  generateWorld();
};

/**
 * @param {object} options
 * @param {boolean} regenerate
 */
export const setHeightOptions = (options, regenerate) => {
  useStore.setState({
    height: { ...useStore.getState().height, options },
  });

  regenerate && generateWorld();
};

/**
 * @param {object} options
 * @param {boolean} regenerate
 */
export const setTemperatureOptions = (options, regenerate) => {
  if (options.minTemperature > options.maxTemperature) {
    options.minTemperature = options.maxTemperature;
  }
  if (options.minEquatorTemperature > options.maxEquatorTemperature) {
    options.minEquatorTemperature = options.maxEquatorTemperature;
  }
  options.minEquatorTemperature = clamp(
    options.minEquatorTemperature,
    options.minTemperature,
    options.maxTemperature,
  );
  options.maxEquatorTemperature = clamp(
    options.maxEquatorTemperature,
    options.minTemperature,
    options.maxTemperature,
  );

  useStore.setState({
    temperature: { ...useStore.getState().temperature, options },
  });

  if (!regenerate) {
    return;
  }

  generateTemperatureMap();
};

/**
 * @param {object} options
 * @param {boolean} regenerate
 */
export const setMoistureOptions = (options, regenerate) => {
  useStore.setState({
    moisture: { ...useStore.getState().moisture, options },
  });

  if (!regenerate) {
    return;
  }

  generateMoistureMap();
};

/**
 * @param {{ x: number, y: number } | null} noiseHover
 */
export const setPipelineNoiseHover = (noiseHover) => {
  useStore.setState({
    pipeline: {
      ...useStore.getState().pipeline,
      noiseHover,
    },
  });
};
