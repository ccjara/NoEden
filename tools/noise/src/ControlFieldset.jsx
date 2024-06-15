import styles from './ControlFieldset.module.css';

export const ControlFieldset = ({ children, title }) => {
  return (
    <fieldset className={styles.fieldset}>
      {!!title && <legend>{title}</legend>}
      {children}
    </fieldset>
  );
};
