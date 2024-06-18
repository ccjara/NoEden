import styles from './Controls.module.css';

export const Controls = ({ children }) => {
  return <div className={styles.controls}>{children}</div>;
};
