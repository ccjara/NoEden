import { ControlFieldset } from '../ControlFieldset.jsx';
import { useStore } from '../store/store.js';
import {
  applyErosionSimulation,
  setErosionOptions,
  setErosionSimulation,
} from '../store/actions.js';
import { NoiseTexture } from '../NoiseTexture.jsx';
import { useCallback, useMemo } from 'react';
import { Fieldset } from '../lib/Fieldset.jsx';
import { generateHeightMap } from '../height/generateHeightMap.js';
import { createHeightColorizer } from '../height/createHeightColorizer.js';
import styles from './ErosionPanel.module.css';

export const ErosionPanel = () => {
  const mapSize = useStore((state) => state.mapSize);
  const options = useStore(($) => $.erosion.options);
  const sim = useStore(($) => $.erosion.sim);
  const altitudeOptions = useStore(($) => $.height.options.altitude);

  const colorizeHeightResult = useMemo(
    () => createHeightColorizer(altitudeOptions),
    [altitudeOptions],
  );

  const colorizeSim = useCallback((value) => {
    return [0, 0, Math.floor(value * 255), 255];
  }, []);

  const colorizeSediment = useCallback((value) => {
    const v = Math.floor(value * 255);
    return [v, v, v, 255];
  }, []);

  return (
    <div>
      <ControlFieldset title="Erosion Options">
        <label>
          <span>Max Iterations</span>
          <input
            type="range"
            min={0}
            max={100}
            step={1}
            value={options.iterations}
            onChange={(e) =>
              setErosionOptions({ iterations: +e.target.value }, true)
            }
          />
          <span>{options.iterations}</span>
        </label>

        <label>
          <span>Erosion Rate</span>
          <input
            type="range"
            min={0}
            max={1}
            step={0.01}
            value={options.erosionRate}
            onChange={(e) =>
              setErosionOptions({ erosionRate: +e.target.value }, true)
            }
          />
          <span>{options.erosionRate}</span>
        </label>

        <label>
          <span>Evaporation Rate</span>
          <input
            type="range"
            min={0}
            max={1}
            step={0.01}
            value={options.evaporationRate}
            onChange={(e) =>
              setErosionOptions({ evaporationRate: +e.target.value }, true)
            }
          />
          <span>{options.evaporationRate}</span>
        </label>

        <label>
          <span>Deposit Rate</span>
          <input
            type="range"
            min={0}
            max={1}
            step={0.01}
            value={options.depositionRate}
            onChange={(e) =>
              setErosionOptions({ depositionRate: +e.target.value }, true)
            }
          />
          <span>{options.depositionRate}</span>
        </label>
      </ControlFieldset>

      <Fieldset title="Simulation">
        <div style={{ display: 'flex', gap: '1rem' }}>
          <label>
            <span>Iteration</span>
            <input
              type="range"
              min={0}
              max={options.iterations}
              step={1}
              value={sim.iteration}
              onChange={(e) =>
                setErosionSimulation({ iteration: +e.target.value }, true)
              }
            />
            <span>{sim.iteration}</span>
          </label>

          <button type="button" onClick={applyErosionSimulation}>
            Apply
          </button>

          <button type="button" onClick={() => generateHeightMap()}>
            Reset Height Map
          </button>
        </div>

        <div className={styles.simTextures}>
          <Fieldset title="Water Map">
            <NoiseTexture
              data={sim.waterMap}
              width={mapSize.width}
              height={mapSize.height}
              colorize={colorizeSim}
            />
          </Fieldset>

          <Fieldset title="Sediment Map">
            <NoiseTexture
              data={sim.sedimentMapResult}
              width={mapSize.width}
              height={mapSize.height}
              colorize={colorizeSediment}
            />
          </Fieldset>

          <Fieldset title="Height Map Result">
            <NoiseTexture
              data={sim.heightMapResult}
              width={mapSize.width}
              height={mapSize.height}
              colorize={colorizeHeightResult}
            />
          </Fieldset>
        </div>
      </Fieldset>
    </div>
  );
};
