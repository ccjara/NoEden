const directions = [
  [1, 0],
  [-1, 0],
  [0, 1],
  [0, -1],
];

export function floodFill(data, width, height, search, replace) {
  const queue = [];

  // add border search cells to the queue
  for (let x = 0; x < width; ++x) {
    if (data[x] === search) queue.push([x, 0]);
    if (data[(height - 1) * width + x] === search) queue.push([x, height - 1]);
  }
  for (let y = 0; y < height; ++y) {
    if (data[y * width] === search) queue.push([0, y]);
    if (data[y * width + width - 1] === search) queue.push([width - 1, y]);
  }

  // process the queue
  while (queue.length > 0) {
    const [x, y] = queue.shift();
    const i = y * width + x;

    if (data[i] !== search) continue;

    data[i] = replace;

    for (const [dx, dy] of directions) {
      const nx = x + dx;
      const ny = y + dy;

      if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        const ni = ny * width + nx;
        if (data[ni] === search) {
          queue.push([nx, ny]);
        }
      }
    }
  }
}
