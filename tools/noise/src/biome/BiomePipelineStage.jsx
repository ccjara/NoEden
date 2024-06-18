import { useStore } from '../store/store.js';
import { PipelineNoiseTexture } from '../PipelineNoiseTexture.jsx';
import { useMemo } from 'react';
import { biomeCategories, BiomeType } from './biomeCategories.js';

const colorize = (value) => {
  return biomeCategories[value]?.color ?? [0, 0, 0, 255];
};

export const BiomePipelineStage = () => {
  const { width, height } = useStore(($) => $.mapSize);
  const map = useStore(($) => $.biome.map);
  const hover = useStore(($) => $.pipeline.noiseHover);

  const biomeValue = useMemo(() => {
    if (!hover) {
      return '-';
    }
    const biome =
      biomeCategories[map[hover.y * width + hover.x] ?? BiomeType.None];
    if (!biome) {
      return '-';
    }
    return (
      <span style={{ color: `rgba(${biome.color.join(',')})` }}>
        {biome.label}
      </span>
    );
  }, [map, width, hover]);

  return (
    <fieldset>
      <legend>Biome</legend>
      <PipelineNoiseTexture
        data={map}
        width={width}
        height={height}
        colorize={colorize}
      />
      <footer>{biomeValue}</footer>
    </fieldset>
  );
};
