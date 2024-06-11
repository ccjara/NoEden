import styles from './Tabs.module.css';
import classNames from 'classnames';
import { useStore } from './store/store.js';
import { setActiveTab } from './store/actions.js';

export const Tabs = ({ items }) => {
  const activeTab = useStore(($) => $.activeTab) ?? items[0]?.id;

  const activeItem = items.find((item) => item.id === activeTab);

  return (
    <div className={styles.tabs}>
      <div className={styles.tabButtons}>
        {items.map((item) => (
          <button
            key={item.id}
            onClick={() => setActiveTab(item.id)}
            className={classNames(styles.tabButton, {
              [styles.tabButtonActive]: activeTab === item.id,
            })}
          >
            {item.title}
          </button>
        ))}
      </div>
      <div className={styles.tabContent}>
        {activeItem && activeItem.content()}
      </div>
    </div>
  );
};
