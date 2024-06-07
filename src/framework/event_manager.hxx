#ifndef NOEDEN_EVENT_MANAGER_HXX
#define NOEDEN_EVENT_MANAGER_HXX

template<typename T>
concept MethodInvocable = std::is_member_function_pointer<T>::value;

template<typename Event, typename EventType>
concept EventLike = requires {
    { Event::event_type } -> std::convertible_to<EventType>;
};

template<typename EventType>
class EventManager {
public:
    /**
     * @brief Registers a free function to be called if the given event is triggered.
     *
     * The function must return a boolean, indicating whether the event was `handled`
     * and should not propagate further to any other event handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike<EventType> Event, typename Fn>
    void on(Fn callable, i32 priority = 0) {
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        partition.add_callable(callable, priority);
    }

    /**
     * @brief Registers member function to be called if the given event is triggered.
     *
     * The function must return a boolean, indicating whether the event was `handled`
     * and should not propagate further to any other event handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike<EventType> Event, typename Inst, MethodInvocable Fn>
    void on(Inst* instance, Fn method, i32 priority = 0) {
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        partition.add_callable(
            [instance, method](Event& event) -> bool {
                return (instance->*method)(event);
            },
            priority
        );
    }

    /**
     * @brief Notifies all event listeners.
     *
     * Constructs an instance of the given event type in-place and calls all
     * registered handlers in order of their registered priority.
     *
     * Short-circuits if any handler returned `true`.
     */
    template<EventLike<EventType> Event, typename... Args>
    void trigger(Args&& ...args) {
        Event event(std::forward<Args>(args)...);
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        for (auto& handler : partition.event_handlers) {
            if (handler.callable(event)) {
                break;
            }
        }
    }

    /**
     * @brief Clears all event partitions
     */
    void clear() {
        partitions_.clear();
    }
private:
    template<EventLike<EventType> Event>
    struct EventHandler {
        std::function<bool(Event&)> callable {};
        i32 priority = 0;

        explicit EventHandler(std::function<bool(Event&)>&& callable, i32 priority) :
            callable(std::move(callable)),
            priority(priority) {
        }
    };

    struct BaseEventPartition {
        virtual ~BaseEventPartition() = default;
    };

    template<EventLike<EventType> Event>
    struct EventPartition : BaseEventPartition {
        explicit EventPartition(EventType event_type) : event_type(event_type) {
        }

        template<typename Callable>
        void add_callable(Callable callable, i32 priority) {
            event_handlers.emplace_back(callable, priority);
            // OPTIMIZE
            std::sort(
                event_handlers.begin(),
                event_handlers.end(),
                [](const EventHandler<Event>& a, const EventHandler<Event>& b) {
                    return a.priority > b.priority;
                }
            );
        }

        EventType event_type;
        std::vector<EventHandler<Event>> event_handlers {};
    };

    template<EventLike<EventType> Event>
    EventPartition<Event>& get_or_create_partition() {
        EventType type = Event::event_type;
        if (partitions_.find(type) == partitions_.end()) {
            partitions_[type] = std::make_unique<EventPartition<Event>>(type);
        }
        return *static_cast<EventPartition<Event>*>(partitions_[type].get());
    }

    std::unordered_map<EventType, std::unique_ptr<BaseEventPartition>> partitions_;
};

#endif
