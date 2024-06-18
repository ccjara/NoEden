import { create } from 'zustand';
import { getStateDefault } from './getStateDefault.js';

export const useStore = create(() => {
  const stateDefault = getStateDefault();

  const persistedState = (() => {
    try {
      const stateStr = localStorage.getItem('state');
      if (!stateStr) {
        return null;
      }
      return JSON.parse(stateStr);
    } catch (e) {
      console.error(e);
      return null;
    }
  })();

  const mapSize = {
    ...stateDefault.mapSize,
    ...persistedState?.mapSize,
  };

  return {
    seed: Number(persistedState?.seed || 0),
    mapSize,
    pipeline: {
      ...stateDefault.pipeline,
      ...persistedState?.pipeline,
    },
    height: {
      map: new Uint8Array(mapSize.width * mapSize.height),
      options: {
        ...stateDefault.height.options,
        ...persistedState?.heightOptions,
      },
    },
    temperature: {
      map: new Float32Array(mapSize.width * mapSize.height),
      options: {
        ...stateDefault.temperature.options,
        ...persistedState?.temperatureOptions,
      },
    },
    humidity: {
      map: new Float32Array(mapSize.width * mapSize.height),
      options: {
        ...stateDefault.humidity.options,
        ...persistedState?.humidityOptions,
      },
    },
    precipitation: {
      map: new Float32Array(mapSize.width * mapSize.height),
      options: {
        ...stateDefault.precipitation.options,
        ...persistedState?.precipitationOptions,
      },
    },
    biome: {
      map: [],
      options: {
        ...stateDefault.biome.options,
        ...persistedState?.biomeOptions,
      },
    },
    visualizer: {
      ...stateDefault.visualizer,
      ...persistedState.visualizer,
    },
  };
});

let persistTimer = null;

const persist = (state) => {
  persistTimer && clearTimeout(persistTimer);
  persistTimer = setTimeout(() => {
    const data = {
      seed: state.seed,
      mapSize: state.mapSize,
      heightOptions: state.height.options,
      temperatureOptions: state.temperature.options,
      humidityOptions: state.humidity.options,
      precipitationOptions: state.precipitation.options,
      biomeOptions: state.biome.options,
      pipeline: state.pipeline,
      visualizer: state.visualizer,
    };

    try {
      localStorage.setItem('state', JSON.stringify(data));
    } catch (e) {
      console.error(e);
    } finally {
      persistTimer = null;
    }
  }, 1000);
};

useStore.subscribe(persist, ($) => $);
