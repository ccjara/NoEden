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
      return JSON.parse(localStorage.getItem('state'));
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
      noiseHover: {
        ...stateDefault.pipeline.noiseHover,
        ...persistedState?.pipeline?.noiseHover,
      },
    },
    height: {
      map: new Float32Array(mapSize.width * mapSize.height),
      options: {
        ...stateDefault.height.options,
        ...persistedState?.height?.options,
      },
    },
    temperature: {
      map: new Float32Array(mapSize.width * mapSize.height),
      options: {
        ...stateDefault.temperature.options,
        ...persistedState?.temperature?.options,
      },
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
