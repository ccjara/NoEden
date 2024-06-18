import styles from './Pipeline.module.css';
import { useMemo } from 'react';
import { TemperaturePipelineStage } from './temperature/TemperaturePipelineStage.jsx';
import { HeightPipelineStage } from './height/HeightPipelineStage.jsx';
import { HumidityPipelineStage } from './humidity/HumidityPipelineStage.jsx';
import { BiomePipelineStage } from './biome/BiomePipelineStage.jsx';
import { PrecipitationPipelineStage } from './precipitation/PrecipitationPipelineStage.jsx';

export const Separator = () => {
  return <div className={styles.separator}>&gt;</div>;
};

export const Pipeline = () => {
  const stages = [
    HeightPipelineStage,
    TemperaturePipelineStage,
    PrecipitationPipelineStage,
    HumidityPipelineStage,
    BiomePipelineStage,
  ];

  const sections = useMemo(() => {
    let sections = [];
    if (!stages.length) {
      return sections;
    }
    for (let stage of stages) {
      sections.push(stage);
      sections.push(Separator);
    }

    sections.pop();

    return sections.map((Section, index) => <Section key={index} />);
  }, [stages]);

  return (
    <fieldset>
      <legend>Pipeline</legend>
      <div className={styles.sections}>{sections}</div>
    </fieldset>
  );
};
