import { useStore } from './store/store.js';
import { useEffect, useRef } from 'react';
import { createHeightColorizer } from './height/createHeightColorizer.js';

export const Render = () => {
  const mapSize = useStore((x) => x.mapSize);
  const heightMap = useStore((x) => x.height.map);
  const altitudeOptions = useStore((x) => x.height.options.altitude);

  const charWidth = 8;
  const charHeight = 14;

  const cw = mapSize.width * charWidth;
  const ch = mapSize.height * charHeight;

  const canvasRef = useRef(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }
    const ctx = canvas.getContext('2d');
    if (!ctx) {
      return;
    }
    ctx.clearRect(0, 0, cw, ch);
    ctx.font = '16px PxPlus IBM VGA8';
    const colorize = createHeightColorizer(altitudeOptions);

    for (let y = 0; y < mapSize.height; ++y) {
      for (let x = 0; x < mapSize.width; ++x) {
        const i = y * mapSize.width + x;
        const altitude = heightMap[i];
        const [r, g, b] = colorize(altitude);

        let ch = '#';

        if (altitude <= altitudeOptions.ocean) {
          ch = '≈';
        } else if (altitude <= altitudeOptions.shore) {
          ch = '⌠';
        } else if (altitude <= altitudeOptions.plains) {
          ch = '#';
        } else if (altitude <= altitudeOptions.hills) {
          ch = '∆';
        } else {
          ch = '▲';
        }

        ctx.fillStyle = `rgb(${r},${g},${b})`;

        ctx.fillText(ch, x * charWidth, y * charHeight);
      }
    }
  }, [cw, ch, heightMap, altitudeOptions]);

  return <canvas ref={canvasRef} width={cw} height={ch}></canvas>;
};
