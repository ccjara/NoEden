import { NoiseTexture } from './NoiseTexture.jsx';
import { useCallback } from 'react';
import { setPipelineNoiseHover } from './store/actions.js';
import { useStore } from './store/store.js';
import styles from './PipelineNoiseTexture.module.css';

export const PipelineNoiseTexture = ({ data, width, height, colorize }) => {
  const hover = useStore((state) => state.pipeline.noiseHover);

  const trackHover = useCallback((e) => {
    const rect = e.target.getBoundingClientRect();
    setPipelineNoiseHover({
      x: Math.floor(e.clientX - rect.x),
      y: Math.floor(e.clientY - rect.y),
    });
  }, []);

  const untrackHover = useCallback(() => {
    setPipelineNoiseHover(null);
  }, []);

  return (
    <div className={styles.root}>
      <NoiseTexture
        data={data}
        width={width}
        height={height}
        colorize={colorize}
        onMouseMove={trackHover}
        onMouseOut={untrackHover}
      />
      {!!hover && (
        <div className={styles.hoverCrosshair}>
          <div style={{ left: `${hover.x}px` }} />
          <div style={{ top: `${hover.y}px` }} />
        </div>
      )}
    </div>
  );
};
