#ifndef NOEDEN_EVENTS_HXX
#define NOEDEN_EVENTS_HXX

/**
 * @brief Represents a game specific event type
 */
enum class GameEventType;

/**
 * @brief Represent a gameplay agnostic engine event type
 */
enum class EngineEventType;

/**
 * @brief Provides access to the event managers
 */
struct Events {
    explicit Events(EventManager<GameEventType>* game, EventManager<EngineEventType>* engine) :
        game(game),
        engine(engine) {
        assert(game);
        assert(engine);
    }

    /**
     * @brief Manages game (-play) events.
     */
    EventManager<GameEventType>* const game = nullptr;

    /**
     * @brief Manages engine events.
     */
    EventManager<EngineEventType>* const engine = nullptr;
};

#endif
