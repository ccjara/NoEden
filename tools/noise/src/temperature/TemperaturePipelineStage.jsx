import { useStore } from '../store/store.js';
import { useMemo } from 'react';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';

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
    threshold: 10,
    label: 'Cold',
    color: [0, 127, 64, 255],
  },
  {
    threshold: 15,
    label: 'Cool',
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

const colorize = (value) => {
  const result = temperatureCategories.find(($) => value <= $.threshold);

  if (!result) {
    return [255, 255, 255, 255];
  }

  return result.color;
};

export const TemperaturePipelineStage = () => {
  const { width, height } = useStore((state) => state.mapSize);
  const map = useStore((state) => state.temperature).map;
  const hover = useStore((state) => state.pipeline.noiseHover);

  const tempValue = useMemo(() => {
    if (!hover) {
      return '-';
    }
    const value = map[hover.y * width + hover.x]?.toFixed(2);
    const category = temperatureCategories.find(($) => value <= $.threshold);
    const label = category ? category.label : 'Unknown';
    let color = category ? category.color : [255, 255, 255, 255];
    color = `rgba(${color.join(',')})`;

    return (
      <span>
        {value} °C | <span style={{ color }}>{label}</span>
      </span>
    );
  }, [map, hover, width]);

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
