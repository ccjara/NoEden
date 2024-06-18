import { Visualizer } from './Visualizer.jsx';
import { TemperaturePanel } from './temperature/TemperaturePanel.jsx';
import { Pipeline } from './Pipeline.jsx';
import { HeightPanel } from './height/HeightPanel.jsx';
import { GeneralPanel } from './GeneralPanel.jsx';
import { useEffect } from 'react';
import { generateWorld } from './generateWorld.js';
import { HumidityPanel } from './humidity/HumidityPanel.jsx';
import { Fieldset } from './lib/Fieldset.jsx';
import { setVisualizerCollapsed } from './store/actions.js';
import { useStore } from './store/store.js';
import { PrecipitationPanel } from './precipitation/PrecipitationPanel.jsx';
import { Render } from './Render.jsx';

function App() {
  const visualizerCollapsed = useStore(($) => $.visualizer.collapsed);

  useEffect(() => {
    generateWorld();
  }, []);

  return (
    <>
      <div style={{ display: 'flex', gap: '1rem', alignItems: 'flex-start' }}>
        <GeneralPanel />

        <HeightPanel />

        <TemperaturePanel />

        <PrecipitationPanel />

        <HumidityPanel />
      </div>

      <Pipeline />

      <div style={{ display: 'flex', gap: '1rem' }}>
        <Fieldset
          title="Visualizer"
          collapsible
          collapsed={visualizerCollapsed}
          onCollapse={setVisualizerCollapsed}
        >
          <Visualizer />
        </Fieldset>
        <Fieldset title="Render">
          <Render />
        </Fieldset>
      </div>
    </>
  );
}

export default App;
