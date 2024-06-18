import { useStore } from '../store/store.js';
import { setAltitudeOptions, setHeightOptions } from '../store/actions.js';
import { Fieldset } from '../lib/Fieldset.jsx';
import { Controls } from '../Controls.jsx';
import { NoiseControls } from '../NoiseControls.jsx';

export const HeightOptions = () => {
  const options = useStore((state) => state.height.options);

  return (
    <Fieldset title="Height Map Options">
      <Controls>
        <label>
          <input
            type="checkbox"
            checked={options.island}
            onChange={(e) =>
              setHeightOptions({ island: e.target.checked }, true)
            }
          />
          <span>Island</span>
        </label>
      </Controls>

      <Fieldset title="Altitudes">
        <Controls>
          <label title="Min altitude to exist in the generated world">
            <span>Min</span>
            <input
              type="number"
              size={4}
              value={options.altitude.min}
              step={1}
              onChange={(e) =>
                setAltitudeOptions({ min: +e.target.value }, true)
              }
            />
          </label>
          <label title="Max altitude to exist in the generated world">
            <span>Max</span>
            <input
              type="number"
              value={options.altitude.max}
              step={1}
              size={4}
              onChange={(e) =>
                setAltitudeOptions({ max: +e.target.value }, true)
              }
            />
          </label>

          <label title="Max altitude for Ocean designation">
            <span>Max Ocean</span>
            <input
              type="number"
              value={options.altitude.ocean}
              step={1}
              size={4}
              onChange={(e) =>
                setAltitudeOptions({ ocean: +e.target.value }, true)
              }
            />
          </label>

          <label title="Max altitude for Shore designation">
            <span>Max Shore</span>
            <input
              type="number"
              value={options.altitude.shore}
              step={1}
              size={4}
              onChange={(e) =>
                setAltitudeOptions({ shore: +e.target.value }, true)
              }
            />
          </label>

          <label title="Max altitude for Plains designation">
            <span>Max Plains</span>
            <input
              type="number"
              value={options.altitude.plains}
              step={1}
              size={4}
              onChange={(e) =>
                setAltitudeOptions({ plains: +e.target.value }, true)
              }
            />
          </label>

          <label title="Max altitude for Hills designation. Any value above this threshold will be considered Mountain">
            <span>Max Hills</span>
            <input
              type="number"
              value={options.altitude.hills}
              step={1}
              size={4}
              onChange={(e) =>
                setAltitudeOptions({ hills: +e.target.value }, true)
              }
            />
          </label>
        </Controls>
      </Fieldset>

      <Fieldset title="Noise options">
        <NoiseControls
          options={options}
          onChange={(o) => setHeightOptions(o, true)}
        />
      </Fieldset>
    </Fieldset>
  );
};
