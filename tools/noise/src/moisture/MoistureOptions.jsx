import { useStore } from '../store/store.js';
import { setMoistureOptions } from '../store/actions.js';
import { ControlFieldset } from '../ControlFieldset.jsx';

export const MoistureOptions = () => {
  const options = useStore((state) => state.moisture.options);

  return (
    <ControlFieldset title="Moisture Options">
      <label>
        <span>Temp. Multiplier</span>
        <input
          type="range"
          min={0}
          max={5}
          step={0.05}
          value={options.temperatureMultiplier}
          onChange={(e) =>
            setMoistureOptions(
              { ...options, temperatureMultiplier: +e.target.value },
              true,
            )
          }
        />
        <span>{options.temperatureMultiplier}</span>
      </label>
      <label>
        <span>Alt. Multiplier</span>
        <input
          type="range"
          min={0}
          max={5}
          step={0.05}
          value={options.altitudeMultiplier}
          onChange={(e) =>
            setMoistureOptions(
              { ...options, altitudeMultiplier: +e.target.value },
              true,
            )
          }
        />
        <span>{options.altitudeMultiplier}</span>
      </label>
    </ControlFieldset>
  );
};
