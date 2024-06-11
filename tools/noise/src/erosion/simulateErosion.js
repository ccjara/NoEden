import { useStore } from '../store/store.js';

export function simulateErosion() {
  const state = useStore.getState();
  const { sim, options } = state.erosion;
  const altitudeOptions = state.height.options.altitude;
  const width = state.mapSize.width;
  const height = state.mapSize.height;
  const { erosionRate, evaporationRate, depositionRate } = options;

  let waterMap = new Float32Array(width * height);
  let sedimentMap = new Float32Array(width * height);
  const heightMap = new Float32Array(state.height.map);

  for (let y = 1; y < height - 1; ++y) {
    for (let x = 1; x < width - 1; ++x) {
      const index = y * width + x;
      if (heightMap[index] <= altitudeOptions.ocean) {
        continue;
      }
      waterMap[y * width + x] = 1;
    }
  }

  for (let iter = 0; iter < sim.iteration; ++iter) {
    // Simulate water flow and erosion
    const newWaterMap = new Float32Array(width * height);
    const newSedimentMap = new Float32Array(width * height);
    const heightChangeMap = new Float32Array(width * height);

    for (let y = 1; y < height - 1; ++y) {
      for (let x = 1; x < width - 1; ++x) {
        const index = y * width + x;
        let totalFlow = 0;
        const flow = [0, 0, 0, 0]; // N, E, S, W

        // Calculate flow to neighbors
        flow[0] = Math.max(
          0,
          heightMap[index] +
            waterMap[index] -
            (heightMap[index - width] + waterMap[index - width]),
        );
        flow[1] = Math.max(
          0,
          heightMap[index] +
            waterMap[index] -
            (heightMap[index + 1] + waterMap[index + 1]),
        );
        flow[2] = Math.max(
          0,
          heightMap[index] +
            waterMap[index] -
            (heightMap[index + width] + waterMap[index + width]),
        );
        flow[3] = Math.max(
          0,
          heightMap[index] +
            waterMap[index] -
            (heightMap[index - 1] + waterMap[index - 1]),
        );

        for (let i = 0; i < 4; ++i) {
          totalFlow += flow[i];
        }

        if (totalFlow > 0) {
          for (let i = 0; i < 4; ++i) {
            const amount = flow[i] / totalFlow;
            let neighborIndex;
            switch (i) {
              case 0:
                neighborIndex = index - width;
                break; // North
              case 1:
                neighborIndex = index + 1;
                break; // East
              case 2:
                neighborIndex = index + width;
                break; // South
              case 3:
                neighborIndex = index - 1;
                break; // West
            }
            newWaterMap[neighborIndex] += amount * waterMap[index];
            newSedimentMap[neighborIndex] += amount * sedimentMap[index];
          }

          // Erode terrain
          const erosionAmount = erosionRate * totalFlow;
          heightChangeMap[index] -= erosionAmount;
          newSedimentMap[index] += erosionAmount;
        }
      }
    }

    // Apply height changes and deposit sediment
    for (let y = 1; y < height - 1; ++y) {
      for (let x = 1; x < width - 1; ++x) {
        const index = y * width + x;
        heightMap[index] += heightChangeMap[index];
        const sedimentAmount = depositionRate * newSedimentMap[index];
        heightMap[index] += sedimentAmount;
        newSedimentMap[index] -= sedimentAmount;
      }
    }

    waterMap = newWaterMap;
    sedimentMap = newSedimentMap;
  }

  useStore.setState({
    erosion: {
      ...state.erosion,
      sim: {
        ...sim,
        waterMap,
        heightMapResult: heightMap,
        sedimentMapResult: sedimentMap,
      },
    },
  });
}
