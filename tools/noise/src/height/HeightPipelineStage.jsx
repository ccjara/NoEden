import { useStore } from '../store/store.js';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';
import { useMemo } from 'react';

export const HeightPipelineStage = () => {
  const { width, height } = useStore(($) => $.mapSize);
  const map = useStore(($) => $.height.map);
  const hover = useStore(($) => $.pipeline.noiseHover);

  const tempValue = useMemo(() => {
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
      <legend>Height</legend>
      <PipelineNoiseTexture data={map} width={width} height={height} />
      <footer>{tempValue}</footer>
    </fieldset>
  );
};
