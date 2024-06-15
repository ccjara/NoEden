import { useStore } from '../store/store.js';
import { setHeightOptions } from '../store/actions.js';
import { ControlFieldset } from '../ControlFieldset.jsx';

export const HeightOptions = () => {
  const options = useStore((state) => state.height.options);

  return (
    <ControlFieldset title="Height Map Options">
      <label>
        <span>X</span>
        <input
          type="range"
          min={0}
          max={5000}
          step={1}
          value={options.offsetX}
          onChange={(e) =>
            setHeightOptions({ ...options, offsetX: +e.target.value }, true)
          }
        />
        <span>{options.offsetX}</span>
      </label>
      <label>
        <span>Y</span>
        <input
          type="range"
          min={0}
          max={50}
          step={1}
          value={options.offsetY}
          onChange={(e) =>
            setHeightOptions({ ...options, offsetY: +e.target.value }, true)
          }
        />
        <span>{options.offsetY}</span>
      </label>
      <label>
        <span>Octaves</span>
        <input
          type="range"
          min={1}
          max={10}
          step={1}
          value={options.octaves}
          onChange={(e) =>
            setHeightOptions({ ...options, octaves: +e.target.value }, true)
          }
        />
        <span>{options.octaves}</span>
      </label>
      <label>
        <span>Frequency</span>
        <input
          type="range"
          min={0.01}
          max={5}
          step={0.05}
          value={options.frequency}
          onChange={(e) =>
            setHeightOptions({ ...options, frequency: +e.target.value }, true)
          }
        />
        <span>{options.frequency}</span>
      </label>
      <label>
        <span>Amplification</span>
        <input
          type="range"
          min={0.1}
          max={5}
          step={0.05}
          value={options.amplification}
          onChange={(e) =>
            setHeightOptions(
              { ...options, amplification: +e.target.value },
              true,
            )
          }
        />
        <span>{options.amplification}</span>
      </label>
      <label>
        <span>Persistence</span>
        <input
          type="range"
          min={0.1}
          max={5}
          step={0.1}
          value={options.persistence}
          onChange={(e) =>
            setHeightOptions({ ...options, persistence: +e.target.value }, true)
          }
        />
        <span>{options.persistence}</span>
      </label>
      <label>
        <span>Lacunarity</span>
        <input
          type="range"
          min={0.1}
          max={10}
          step={0.1}
          value={options.lacunarity}
          onChange={(e) =>
            setHeightOptions({ ...options, lacunarity: +e.target.value }, true)
          }
        />
        <span>{options.lacunarity}</span>
      </label>
    </ControlFieldset>
  );
};
