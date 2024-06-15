import { useStore } from './store/store.js';
import { ControlFieldset } from './ControlFieldset.jsx';
import { useCallback, useEffect, useRef } from 'react';
import { reset, setMapSize, setSeed } from './store/actions.js';

export const GeneralPanel = () => {
  const seedRef = useRef(null);
  const widthRef = useRef(null);
  const heightRef = useRef(null);
  const seed = useStore(($) => $.seed);
  const mapSize = useStore(($) => $.mapSize);

  useEffect(() => {
    if (seedRef.current) {
      seedRef.current.value = seed;
    }
    if (widthRef.current) {
      widthRef.current.value = mapSize.width;
    }
    if (heightRef.current) {
      heightRef.current.value = mapSize.height;
    }
  }, [mapSize, seed]);

  const onPressApply = useCallback(() => {
    const width = parseInt(widthRef.current.value);
    const height = parseInt(heightRef.current.value);
    const seed = parseInt(seedRef.current.value);
    setMapSize(width, height, false);
    setSeed(seed, true);
  }, []);

  const onPressReset = useCallback(() => {
    reset();
  }, []);

  return (
    <ControlFieldset title="General">
      <div
        style={{
          display: 'flex',
          gap: '1rem',
          justifyContent: 'space-between',
        }}
      >
        <label>
          <span style={{ width: '2rem' }}>Width</span>
          <input
            ref={widthRef}
            type="number"
            defaultValue={mapSize.width}
            style={{ width: '3rem' }}
          />
        </label>
        <label>
          Height
          <input
            ref={heightRef}
            type="number"
            min={32}
            max={512}
            defaultValue={mapSize.height}
            style={{ width: '3rem' }}
          />
        </label>
      </div>

      <label>
        <span style={{ width: '2rem' }}>Seed</span>
        <input ref={seedRef} type="number" defaultValue={seed} />
      </label>

      <button type="submit" onClick={onPressApply}>
        Apply
      </button>

      <hr />

      <button type="reset" onClick={onPressReset}>
        Reset to defaults
      </button>
    </ControlFieldset>
  );
};
