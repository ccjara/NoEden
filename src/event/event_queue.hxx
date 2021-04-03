#ifndef JARALYN_EVENT_QUEUE_HXX
#define JARALYN_EVENT_QUEUE_HXX

struct queue_consume_tag {};
struct queue_consume_immediate_tag : public queue_consume_tag {};
struct queue_consume_deferred_tag : public queue_consume_tag {};

/**
 * FIXME: Using the entt::dispatcher as a queue is an inefficient workaround.
 */
class j_event_queue {
public:
    j_event_queue(entt::dispatcher* dispatcher) : dispatcher_(dispatcher) {
        assert(dispatcher_);
    }

    /**
     * @brief Creates a copy for incoming game events for the implementing system
     *
     * Not really sure how to get rid of this function. I tried connecting
     * to the entt::dispatcher::enqueue<event_source> function pointer but got
     * template error nightmares.
     */
    template<typename event_source>
    inline void shovel(const event_source& e) {
        queue_.enqueue(e);
    }

    /**
     * @brief Attaches to the given game event and registers an executor
     *
     * Incoming events are stored aside and can be processed during the system's
     * update phase. You can process all events, or only those of the same type.
     * Refer to the entt::dispatcher::update() documentation.
     */
    template<typename event_source, auto executor, typename instance>
    void bind(instance&& i, queue_consume_tag consume_tag = queue_consume_deferred_tag {}) {
        if constexpr (std::is_same_v<queue_consume_tag, queue_consume_deferred_tag>) {
            // register the executor that will be called as the queue is processed
            queue_.sink<event_source>().connect<executor>(i);
            // setup event transcription
            dispatcher_->sink<event_source>().connect<&j_event_queue::shovel<event_source>>(i);
        } else {
            dispatcher_->sink<event_source>().connect<executor>(i);
        }
    }

    void process() {
        queue_.update();
    }
private:
    /**
     * @brief Contains all stored events which will are processed on demand
     *
     * Note: The executors must only be executed in the update method of the
     *       system.
     */
    entt::dispatcher queue_;
    entt::dispatcher* dispatcher_ { nullptr };
};

#endif
