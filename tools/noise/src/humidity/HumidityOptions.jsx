import { useStore } from '../store/store.js';
import { setHumidityOptions } from '../store/actions.js';
import { ControlFieldset } from '../ControlFieldset.jsx';

export const HumidityOptions = () => {
  const options = useStore((state) => state.humidity.options);

  return (
    <ControlFieldset title="Humidity Options">
      <label>
        <span>Temp. Multiplier</span>
        <input
          type="range"
          min={0}
          max={0.1}
          step={0.001}
          value={options.temperatureMultiplier}
          onChange={(e) =>
            setHumidityOptions(
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
          max={0.1}
          step={0.001}
          value={options.altitudeMultiplier}
          onChange={(e) =>
            setHumidityOptions(
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
