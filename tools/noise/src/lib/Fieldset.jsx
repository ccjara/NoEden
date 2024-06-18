import styles from './Fieldset.module.css';
import { useCallback } from 'react';

export const Fieldset = ({
  children,
  title,
  collapsible = false,
  collapsed = false,
  onCollapse,
}) => {
  const onCollapseInternal = useCallback(() => {
    onCollapse && onCollapse(!collapsed);
  }, [onCollapse, collapsed]);

  return (
    <fieldset>
      <legend className={styles.legend}>
        {collapsible ? (
          <button
            type="button"
            style={{ border: 'none' }}
            onClick={onCollapseInternal}
          >
            <span>{title}</span>
            <span>{collapsed ? '+' : '-'}</span>
          </button>
        ) : (
          <span>{title}</span>
        )}
      </legend>
      {(!collapsible || !collapsed) && children}
      {collapsible && collapsed && '...'}
    </fieldset>
  );
};
