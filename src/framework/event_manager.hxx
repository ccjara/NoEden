#ifndef NOEDEN_EVENT_MANAGER_HXX
#define NOEDEN_EVENT_MANAGER_HXX

template<typename T>
concept MethodInvocable = std::is_member_function_pointer<T>::value;

template<typename Event, typename EventType>
concept EventLike = requires {
    { Event::event_type } -> std::convertible_to<EventType>;
};

enum class EventResult {
    /**
     * @brief Continue propagating the event to other handlers
     */
    Continue,

    /**
     * @brief Halt propagation of the event to other handlers
     */
    Halt,
};

using ListenerId = uintptr_t;

template<typename Event>
struct Subscription;

template<typename EventType>
class EventManager {
public:
    /**
     * @brief Registers a free function to be called if the given event is triggered.
     *
     * The function must return an EventResult, indicating whether the event may propagate further to other handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike<EventType> Event, typename Fn>
    [[nodiscard("Returned subscription instance must be stored otherwise the handler will be removed immediately after this call.")]]
    Subscription<Event> on(Fn callable, i32 priority = 0) {
        const Id id = generate_id();
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        partition.add_callable(id, callable, priority);
        return Subscription<Event>(this, id);
    }

    /**
     * @brief Registers a method to be called if the given event is triggered.
     *
     * The function must return an EventResult, indicating whether the event may propagate further to other handlers.
     *
     * The higher the priority the earlier the handler will be called.
     */
    template<EventLike<EventType> Event, typename Inst, MethodInvocable Fn>
    [[nodiscard("Returned subscription instance must be stored otherwise the handler will be removed immediately after this call.")]]
    Subscription<Event> on(Inst* instance, Fn method, i32 priority = 0) {
        const Id id = generate_id();
        EventPartition<Event>& partition = get_or_create_partition<Event>();
        partition.add_callable(
            id,
            [instance, method](Event& event) -> EventResult {
                return (instance->*method)(event);
            },
            priority
        );
        return Subscription<Event>(this, id);
    }

    template<EventLike<EventType> Event>
    void off(Subscription<Event>& subscription) {
        auto partition = get_partition(Event::event_type);
        if (!partition) {
            return;
        }
        partition->remove_listener(subscription.id());
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
            if (handler.callable(event) == EventResult::Halt) {
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

    /**
     * @brief Returns the number of event handlers in a partition
     */
    size_t handler_count(EventType type) const {
        auto partition = get_partition(type);
        if (!partition) {
            return 0;
        }
        return partition->handler_count();
    }
private:
    template<EventLike<EventType> Event>
    struct EventHandler {
        ListenerId id = 0;
        std::function<EventResult(Event&)> callable {};
        i32 priority = 0;

        explicit EventHandler(ListenerId id, std::function<EventResult(Event&)>&& callable, i32 priority) :
            id(id),
            callable(std::move(callable)),
            priority(priority) {
        }
    };

    struct BaseEventPartition {
        virtual ~BaseEventPartition() = default;

        virtual void remove_listener(ListenerId id) = 0;

        virtual size_t handler_count() const = 0;
    };

    template<EventLike<EventType> Event>
    struct EventPartition : BaseEventPartition {
        explicit EventPartition(EventType event_type) : event_type(event_type) {
        }

        template<typename Callable>
        void add_callable(Id id, Callable callable, i32 priority) {
            event_handlers.emplace_back(id, callable, priority);
            // OPTIMIZE
            std::sort(
                event_handlers.begin(),
                event_handlers.end(),
                [](const EventHandler<Event>& a, const EventHandler<Event>& b) {
                    return a.priority > b.priority;
                }
            );
        }

        void remove_listener(ListenerId id) override {
            event_handlers.erase(
                std::remove_if(
                    event_handlers.begin(),
                    event_handlers.end(),
                    [id](const EventHandler<Event>& handler) -> bool {
                        return handler.id == id;
                    }
                ),
                event_handlers.end()
            );
        }

        virtual size_t handler_count() const override {
            return event_handlers.size();
        }

        EventType event_type;
        std::vector<EventHandler<Event>> event_handlers {};
    };

    template<EventLike<EventType> Event>
    EventPartition<Event>& get_or_create_partition() {
        EventType type = Event::event_type;
        auto [it, inserted] = partitions_.emplace(type, nullptr);
        if (inserted) {
            it->second = std::make_unique<EventPartition<Event>>(type);
        }
        return *static_cast<EventPartition<Event>*>(it->second.get());
    }

    BaseEventPartition* get_partition(EventType type) const {
        auto it = partitions_.find(type);
        if (it == partitions_.end()) {
            return nullptr;
        }
        return static_cast<BaseEventPartition*>(it->second.get());
    }

    Id generate_id() {
        return next_id_++;
    }

    std::unordered_map<EventType, std::unique_ptr<BaseEventPartition>> partitions_;
    std::atomic<Id> next_id_ = 1;
};

template<typename Event>
class Subscription {
public:
    using EventManagerType = EventManager<typename Event::EventType>;

    Subscription(EventManagerType* event_manager, Id id)
        : event_manager_(event_manager), id_(id) {
        assert(event_manager);
        assert(id);
    }

    Id id() const {
        return id_;
    }

    ~Subscription() {
        if (event_manager_ && id_) {
            event_manager_->off(*this);
            id_ = 0;
        }
    }

    Subscription() = default;

    Subscription& operator=(Subscription&& other) noexcept {
        if (this != &other) {
            if (event_manager_ && id_ && (event_manager_ != other.event_manager_ || id_ != other.id_)) {
                event_manager_->off(*this);
            }
            event_manager_ = other.event_manager_;
            id_ = other.id_;
            other.id_ = 0;
        }
        return *this;
    }

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;
private:
    EventManagerType* event_manager_ = nullptr;
    Id id_ = 0;
};

#endif
