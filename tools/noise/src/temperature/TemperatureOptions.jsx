import { useStore } from '../store/store.js';
import { ControlFieldset } from '../ControlFieldset.jsx';
import { setTemperatureOptions } from '../store/actions.js';

export const TemperatureOptions = () => {
  const options = useStore((state) => state.temperature.options);

  return (
    <ControlFieldset title="Temperature Options">
      <label>
        <span>Min Temperature °C</span>
        <input
          type="number"
          value={options.minTemperature}
          min={-100}
          max={100}
          step={1}
          onChange={(e) => {
            setTemperatureOptions(
              { ...options, minTemperature: Number(e.target.value) },
              true,
            );
          }}
        />
      </label>
      <label>
        <span>Max Temperature °C</span>
        <input
          type="number"
          value={options.maxTemperature}
          min={-100}
          max={100}
          step={1}
          onChange={(e) => {
            setTemperatureOptions(
              { ...options, maxTemperature: Number(e.target.value) },
              true,
            );
          }}
        />
      </label>
      <label>
        <span>Min Equator Temperature °C</span>
        <input
          type="number"
          value={options.minEquatorTemperature}
          min={-100}
          max={100}
          step={1}
          onChange={(e) =>
            setTemperatureOptions(
              { ...options, minEquatorTemperature: Number(e.target.value) },
              true,
            )
          }
        />
      </label>
      <label>
        <span>Max Equator Temperature °C</span>
        <input
          type="number"
          value={options.maxEquatorTemperature}
          min={-100}
          max={100}
          step={1}
          onChange={(e) =>
            setTemperatureOptions(
              { ...options, maxEquatorTemperature: Number(e.target.value) },
              true,
            )
          }
        />
      </label>
      <label>
        <span>Altitude Factor</span>
        <input
          type="number"
          value={options.altitudeFactor}
          min={0}
          max={1}
          step={0.01}
          onChange={(e) =>
            setTemperatureOptions(
              { ...options, altitudeFactor: Number(e.target.value) },
              true,
            )
          }
        />
      </label>
      <label>
        <input
          type="checkbox"
          checked={options.latitudeDropoff}
          onChange={(e) =>
            setTemperatureOptions(
              { ...options, latitudeDropoff: e.target.checked },
              true,
            )
          }
        />
        Latitude Dropoff
      </label>
    </ControlFieldset>
  );
};
