export function createDistanceMap(heightMap, width, height) {
  const OCEAN = 0;
  const MAX_DISTANCE = width + height;

  let distanceMap = new Array(width * height).fill(MAX_DISTANCE);

  // Initialize distance map with 0 for ocean cells
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const i = y * width + x;
      if (heightMap[i] === OCEAN) {
        distanceMap[i] = 0;
      }
    }
  }

  // Use a flood fill algorithm (BFS) to calculate distances
  let queue = [];
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const i = y * width + x;
      if (distanceMap[i] === 0) {
        queue.push([x, y]);
      }
    }
  }

  while (queue.length > 0) {
    let [cx, cy] = queue.shift();
    const currentIndex = cy * width + cx;
    const currentDistance = distanceMap[currentIndex];

    const neighbors = [
      [cx + 1, cy],
      [cx - 1, cy],
      [cx, cy + 1],
      [cx, cy - 1],
    ];

    for (const [nx, ny] of neighbors) {
      if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        const neighborIndex = ny * width + nx;
        if (distanceMap[neighborIndex] > currentDistance + 1) {
          distanceMap[neighborIndex] = currentDistance + 1;
          queue.push([nx, ny]);
        }
      }
    }
  }

  return distanceMap;
}
