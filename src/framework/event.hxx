#ifndef NOEDEN_EVENT_HXX
#define NOEDEN_EVENT_HXX

/**
 * @brief Base template for defining events to be used with the EventManager
 *
 * @tparam EventTypeValue - Specific event type value associated with this event
 */
template<auto EventTypeValue>
struct Event {
    /**
     * @brief Type alias for the event type, derived from the type of EventTypeValue
     */
    using EventType = decltype(EventTypeValue);

    /**
     * @brief Specific event type value used at runtime to identify this event
     */
    static constexpr EventType event_type = EventTypeValue;

    // static_assert(std::is_enum_v<EventType>, "EventTypeValue must be an enum type");
};

#endif
