import { useStore } from '../store/store.js';
import {
  setPrecipitationNoiseOptions,
  setPrecipitationOptions,
} from '../store/actions.js';
import { Fieldset } from '../lib/Fieldset.jsx';
import { NoiseControls } from '../NoiseControls.jsx';
import { Controls } from '../Controls.jsx';

export const PrecipitationOptions = () => {
  const options = useStore((state) => state.precipitation.options);

  return (
    <Fieldset title="Precipitation Options">
      <Controls>
        <label>
          <span>Altitude Multiplier</span>
          <input
            type="number"
            min={0}
            max={1}
            step={0.01}
            size={5}
            value={options.altitudeMultiplier}
            onChange={(e) =>
              setPrecipitationOptions(
                { altitudeMultiplier: +e.target.value },
                true,
              )
            }
          />
        </label>
      </Controls>

      <Fieldset title="Noise options">
        <NoiseControls
          options={options.noise}
          onChange={(o) => setPrecipitationNoiseOptions(o, true)}
        />
      </Fieldset>
    </Fieldset>
  );
};
