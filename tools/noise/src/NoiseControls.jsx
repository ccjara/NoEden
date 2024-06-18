import { Controls } from './Controls.jsx';

export const NoiseControls = ({ options, onChange }) => {
  return (
    <Controls>
      <label>
        <span>X</span>
        <input
          type="range"
          min={0}
          max={5000}
          step={1}
          value={options.offsetX}
          onChange={(e) => onChange({ ...options, offsetX: +e.target.value })}
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
          onChange={(e) => onChange({ ...options, offsetY: +e.target.value })}
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
          onChange={(e) => onChange({ ...options, octaves: +e.target.value })}
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
          onChange={(e) => onChange({ ...options, frequency: +e.target.value })}
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
            onChange({ ...options, amplification: +e.target.value }, true)
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
            onChange({ ...options, persistence: +e.target.value })
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
            onChange({ ...options, lacunarity: +e.target.value })
          }
        />
        <span>{options.lacunarity}</span>
      </label>
    </Controls>
  );
};
