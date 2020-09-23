#ifndef JARALYN_JOURNAL_HXX
#define JARALYN_JOURNAL_HXX

#include "../game.hxx"
#include "../event/inventory_event.hxx"
#include "../components/components.hxx"

/**
 * @brief Listens to game events and logs them for the player
 *
 * TODO: pagination, limits, filtering
 */
class j_journal {
private:
    std::vector<std::string> entries_;

    void on_gathering_completed(const j_gathering_completed_event& e);
public:
    /**
     * @brief Attaches the action log to game events
     */
    void attach(entt::dispatcher* dispatcher);

    /**
     * @brief Returns all entries which should be displayed
     */
    [[nodiscard]] const std::vector<std::string>& entries() const;
};

#endif
