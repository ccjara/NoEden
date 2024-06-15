import styles from './Pipeline.module.css';
import { useMemo } from 'react';
import { TemperaturePipelineStage } from './temperature/TemperaturePipelineStage.jsx';
import { HeightPipelineStage } from './height/HeightPipelineStage.jsx';
import { MoisturePipelineStage } from './moisture/MoisturePipelineStage.jsx';

export const Separator = () => {
  return <div className={styles.separator}>&gt;</div>;
};

export const Pipeline = () => {
  const stages = [
    HeightPipelineStage,
    TemperaturePipelineStage,
    MoisturePipelineStage,
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
