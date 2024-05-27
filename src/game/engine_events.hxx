#ifndef JARALYN_ENGINE_EVENTS_HXX
#define JARALYN_ENGINE_EVENTS_HXX

#include "../events/event_manager.hxx"

class EngineEvents {
public:
    EngineEvents() = delete;

    static void init() {

    }

    static void shutdown() {
        em_.clear();
    }

    /**
     * @inheritdoc EventManager::on
     */
    template<typename E, typename Fn>
    static void on(Fn callable, i32 priority = 0) {
        em_.on<E>(callable, priority);
    }

    /**
     * @inheritdoc EventManager::on
     */
    template<typename E, typename Inst, MethodInvocable Fn>
    static void on(Inst* instance, Fn method, i32 priority = 0) {
        em_.on<E>(instance, method, priority);
    }

    /**
     * @inheritdoc EventManager::trigger
     */
    template<typename E, typename... Args>
    static void trigger(Args&& ...args) {
        em_.trigger<E>(std::forward<Args>(args)...);
    }
private:
    static inline EventManager em_;
};

#endif
