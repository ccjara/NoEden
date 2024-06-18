import { useStore } from '../store/store.js';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';
import { useMemo } from 'react';

const colorStops = [
  { stop: 0.0, color: [127, 63, 0, 255] },
  { stop: 0.1, color: [255, 127, 0, 255] },
  { stop: 0.2, color: [255, 255, 0, 255] },
  { stop: 0.3, color: [127, 255, 0, 255] },
  { stop: 0.4, color: [0, 255, 0, 255] },
  { stop: 0.5, color: [0, 127, 0, 255] },
  { stop: 0.6, color: [0, 127, 255, 255] },
  { stop: 0.7, color: [0, 0, 255, 255] },
  { stop: 0.8, color: [127, 0, 255, 255] },
  { stop: 0.9, color: [255, 0, 255, 255] },
  { stop: 1.0, color: [127, 0, 127, 255] },
];

const colorize = (value) =>
  colorStops.findLast(($) => value >= $.stop)?.color ?? [0, 0, 0, 255];

export const PrecipitationPipelineStage = () => {
  const { width, height } = useStore(($) => $.mapSize);
  const map = useStore(($) => $.precipitation.map);
  const hover = useStore(($) => $.pipeline.noiseHover);

  const precipitationValue = useMemo(() => {
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
      <legend>Precipitation</legend>
      <PipelineNoiseTexture
        data={map}
        width={width}
        height={height}
        colorize={colorize}
      />
      <footer>{precipitationValue}</footer>
    </fieldset>
  );
};
