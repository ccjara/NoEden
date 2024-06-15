import { generateNoise } from './generateNoise.js';
import { useStore } from './store/store.js';
import { generateTemperatureMap } from './generateTemperatureMap.js';

export const generateWorld = () => {
  const state = useStore.getState();

  const heightMap = generateNoise({
    octaves: state.height.options.octaves,
    frequency: state.height.options.frequency,
    amplification: state.height.options.amplification,
    persistence: state.height.options.persistence,
    lacunarity: state.height.options.lacunarity,
    offsetX: state.height.options.offsetX,
    offsetY: state.height.options.offsetY,
    width: state.mapSize.width,
    height: state.mapSize.height,
    seed: state.seed,
  });

  useStore.setState({
    height: {
      ...state.height,
      map: heightMap.data,
    },
  });

  generateTemperatureMap();
};
