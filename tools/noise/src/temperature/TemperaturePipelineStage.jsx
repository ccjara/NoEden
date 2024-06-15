import { NoiseTexture } from '../NoiseTexture.jsx';
import { useStore } from '../store/store.js';
import { useCallback, useMemo } from 'react';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';

// TMAX -50
// TMIN 60

// TNORM = T - TMIN / TMAX - TMIN
// T = TNORM * (TMAX - TMIN) + TMIN

const temperatureCategories = [
  {
    threshold: -30,
    label: 'Freezing',
    color: [0, 127, 255, 255],
  },
  {
    threshold: -10,
    label: 'Very Cold',
    color: [0, 255, 255, 255],
  },
  {
    threshold: 0,
    label: 'Cold',
    color: [0, 255, 0, 255],
  },
  {
    threshold: 20,
    label: 'Mild',
    color: [127, 255, 0, 255],
  },
  {
    threshold: 30,
    label: 'Warm',
    color: [255, 255, 0, 255],
  },
  {
    threshold: 40,
    label: 'Hot',
    color: [255, 127, 0, 255],
  },
  {
    threshold: 50,
    label: 'Very Hot',
    color: [255, 0, 0, 255],
  },
  {
    threshold: Number.MAX_VALUE,
    label: 'Scorching',
    color: [127, 0, 0, 255],
  },
];

const createColorizer = (options) => {
  const c = temperatureCategories.map(($) => [$.threshold, $.color]);

  return (value) => {
    const denormalized =
      value * (options.maxTemperature - options.minTemperature) +
      options.minTemperature;
    const result = c.find(([threshold]) => denormalized <= threshold);

    if (!result) {
      return [255, 255, 255, 255];
    }

    return result[1];
  };
};

export const TemperaturePipelineStage = () => {
  const { width, height } = useStore((state) => state.mapSize);
  const { map, options } = useStore((state) => state.temperature);
  const hover = useStore((state) => state.pipeline.noiseHover);

  const tempValue = useMemo(() => {
    if (!hover) {
      return '-';
    }
    const normalizedValue = map[hover.y * width + hover.x]?.toFixed(2);
    if (!normalizedValue) {
      return '-';
    }
    const denormalizedValue = (
      normalizedValue * (options.maxTemperature - options.minTemperature) +
      options.minTemperature
    ).toFixed(2);

    const category = temperatureCategories.find(
      (category) => denormalizedValue <= category.threshold,
    );

    const label = category ? category.label : 'Unknown';
    let color = category ? category.color : [255, 255, 255, 255];
    color = `rgba(${color.join(',')})`;

    return (
      <span>
        {normalizedValue} | {denormalizedValue} °C |{' '}
        <span style={{ color }}>{label}</span>
      </span>
    );
  }, [map, hover, width]);

  const colorize = useMemo(() => createColorizer(options), [options]);

  return (
    <fieldset>
      <legend>Temperature</legend>
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
