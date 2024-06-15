import { useEffect, useRef } from 'react';

const defaultColorize = (value) => {
  value = Math.floor(value * 255);
  return [value, value, value, 255];
};

export const NoiseTexture = ({
  data,
  width,
  height,
  scale = 1,
  colorize = defaultColorize,
  onMouseMove,
  onMouseOut,
}) => {
  const canvasRef = useRef(null);

  useEffect(() => {
    if (!canvasRef.current || !data.length) {
      return;
    }
    const ctx = canvasRef.current.getContext('2d');
    if (!ctx) {
      return;
    }
    const texture = new Uint8ClampedArray(width * height * 4);

    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        const i = y * width + x;
        const [r, g, b, a] = colorize(data[i]);

        texture[i * 4] = r;
        texture[i * 4 + 1] = g;
        texture[i * 4 + 2] = b;
        texture[i * 4 + 3] = a;
      }
    }

    ctx.putImageData(new ImageData(texture, width, height), 0, 0);
  }, [data]);

  return (
    <canvas
      ref={canvasRef}
      id="canvas"
      width={width + 'px'}
      height={height + 'px'}
      style={{
        transform: `scale(${scale})`,
        imageRendering: 'pixelated',
        transformOrigin: 'top left',
      }}
      onMouseMove={onMouseMove}
      onMouseOut={onMouseOut}
    />
  );
};
