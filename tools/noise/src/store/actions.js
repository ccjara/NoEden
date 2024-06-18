import { useStore } from './store.js';
import { generateWorld } from '../generateWorld.js';
import { generateTemperatureMap } from '../temperature/generateTemperatureMap.js';
import { getStateDefault } from './getStateDefault.js';
import { clamp } from '../lib/clamp.js';
import { generateHumidityMap } from '../humidity/generateHumidityMap.js';
import { MAX_ALTITUDE, MIN_ALTITUDE } from '../lib/technicalLimits.js';
import { generatePrecipitationMap } from '../precipitation/generatePrecipitationMap.js';

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
 * @param {object} options
 * @param {boolean} regenerate
 */
export const setHeightOptions = (options, regenerate) => {
  const state = useStore.getState();

  useStore.setState({
    height: {
      ...state.height,
      options: {
        ...state.height.options,
        ...options,
      },
    },
  });

  regenerate && generateWorld();
};

/**
 * @param {object} options
 * @param {boolean} regenerate
 */
export const setAltitudeOptions = (options, regenerate) => {
  const state = useStore.getState();

  const altitude = {
    ...state.height.options.altitude,
    ...options,
  };

  altitude.min = clamp(altitude.min, MIN_ALTITUDE, MAX_ALTITUDE);
  altitude.max = clamp(altitude.max, MIN_ALTITUDE, MAX_ALTITUDE);

  if (altitude.min > altitude.max) {
    altitude.min = altitude.max;
  }

  altitude.ocean = clamp(altitude.ocean, MIN_ALTITUDE, MAX_ALTITUDE);
  altitude.shore = clamp(altitude.shore, MIN_ALTITUDE, MAX_ALTITUDE);
  altitude.plains = clamp(altitude.plains, MIN_ALTITUDE, MAX_ALTITUDE);
  altitude.hills = clamp(altitude.hills, MIN_ALTITUDE, MAX_ALTITUDE);

  if (altitude.ocean > altitude.shore) {
    altitude.shore = altitude.ocean;
  }
  if (altitude.shore > altitude.plains) {
    altitude.plains = altitude.shore;
  }
  if (altitude.plains > altitude.hills) {
    altitude.hills = altitude.plains;
  }

  useStore.setState({
    height: {
      ...state.height,
      options: {
        ...state.height.options,
        altitude,
      },
    },
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
export const setHumidityOptions = (options, regenerate) => {
  useStore.setState({
    humidity: { ...useStore.getState().humidity, options },
  });

  if (!regenerate) {
    return;
  }

  generateHumidityMap();
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

export const setVisualizerCollapsed = (collapsed) => {
  const state = useStore.getState();

  useStore.setState({
    ...state,
    visualizer: {
      ...state.visualizer,
      collapsed,
    },
  });
};

export const setPrecipitationOptions = (options, regenerate) => {
  const state = useStore.getState();

  useStore.setState({
    precipitation: {
      ...state.precipitation,
      options: {
        ...state.precipitation.options,
        ...options,
      },
    },
  });

  regenerate && generatePrecipitationMap();
};

export const setPrecipitationNoiseOptions = (options, regenerate) => {
  const state = useStore.getState();

  useStore.setState({
    precipitation: {
      ...state.precipitation,
      options: {
        ...state.precipitation.options,
        noise: {
          ...state.precipitation.options.noise,
          ...options,
        },
      },
    },
  });

  regenerate && generatePrecipitationMap();
};
