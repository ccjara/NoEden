import { useStore } from '../store/store.js';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';
import { useMemo } from 'react';
import { interpolateColor } from '../lib/interpolateColor.js';

const colorize = (value) => {
  const colorStops = [
    { stop: 0.0, color: [255, 0, 0, 255] },
    { stop: 0.1, color: [255, 127, 0, 255] },
    { stop: 0.3, color: [255, 255, 0, 255] },
    { stop: 0.6, color: [0, 128, 0, 255] },
    { stop: 1.0, color: [0, 0, 255, 255] },
  ];

  return interpolateColor(value, colorStops);
};

export const MoisturePipelineStage = () => {
  const { width, height } = useStore(($) => $.mapSize);
  const map = useStore(($) => $.moisture.map);
  const hover = useStore(($) => $.pipeline.noiseHover);

  const moistureValue = useMemo(() => {
    if (!hover) {
      return '-';
    }
    const normalizedValue = map[hover.y * width + hover.x]?.toFixed(2);
    if (!normalizedValue) {
      return '-';
    }

    return <span>{normalizedValue}</span>;
  }, [map, width, hover]);

  return (
    <fieldset>
      <legend>Moisture</legend>
      <PipelineNoiseTexture
        data={map}
        width={width}
        height={height}
        colorize={colorize}
      />
      <footer>{moistureValue}</footer>
    </fieldset>
  );
};
