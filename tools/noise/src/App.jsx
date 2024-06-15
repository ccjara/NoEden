import styles from './App.module.css';
import { Visualizer } from './Visualizer.jsx';
import { TemperaturePanel } from './temperature/TemperaturePanel.jsx';
import { Pipeline } from './Pipeline.jsx';
import { HeightPanel } from './height/HeightPanel.jsx';
import { GeneralPanel } from './GeneralPanel.jsx';
import { useEffect } from 'react';
import { generateWorld } from './generateWorld.js';
import { MoisturePanel } from './moisture/MoisturePanel.jsx';

function App() {
  useEffect(() => {
    generateWorld();
  }, []);

  return (
    <>
      <div style={{ display: 'flex', gap: '1rem', alignItems: 'flex-start' }}>
        <GeneralPanel />

        <HeightPanel />

        <TemperaturePanel />

        <MoisturePanel />
      </div>

      <Pipeline />

      <fieldset>
        <legend>Visualizer</legend>
        <Visualizer />
      </fieldset>
    </>
  );
}

export default App;
