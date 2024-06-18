import { useStore } from '../store/store.js';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';
import { useMemo } from 'react';
import { createHeightColorizer } from './createHeightColorizer.js';

export const HeightPipelineStage = () => {
  const { width, height } = useStore(($) => $.mapSize);
  const map = useStore(($) => $.height.map);
  const options = useStore(($) => $.height.options);
  const hover = useStore(($) => $.pipeline.noiseHover);
  const { altitude: altitudeOptions } = options;

  const colorize = useMemo(
    () => createHeightColorizer(altitudeOptions),
    [altitudeOptions],
  );

  const tempValue = useMemo(() => {
    if (!hover) {
      return '-';
    }
    const altitude = map[hover.y * width + hover.x];
    let label = '';

    if (altitude <= altitudeOptions.ocean) {
      label = 'Ocean';
    } else if (altitude <= altitudeOptions.shore) {
      label = 'Shore';
    } else if (altitude <= altitudeOptions.plains) {
      label = 'Plains';
    } else if (altitude <= altitudeOptions.hills) {
      label = 'Hills';
    } else {
      label = 'Mountains';
    }

    return (
      <span>
        {altitude ?? 0} - {label}
      </span>
    );
  }, [map, width, hover]);

  return (
    <fieldset>
      <legend>Altitude</legend>
      <PipelineNoiseTexture
        data={map}
        width={width}
        height={height}
        colorize={colorize}
      />
      <footer>{tempValue}</footer>
    </fieldset>
  );
};
