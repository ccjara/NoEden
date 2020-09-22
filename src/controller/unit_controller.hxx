#ifndef JARALYN_UNIT_CONTROLLER_HXX
#define JARALYN_UNIT_CONTROLLER_HXX

#include "../game.hxx"
#include "../event/gathering_event.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"
#include "controller.hxx"

/**
 * @brief Acts on any unit in the world
 */
class j_unit_controller : public j_controller {
private:
    /**
     * @brief Act on a gathering attempt
     *
     * As of now places the first found item at the notified position in the
     * entity's inventory.
     */
    void on_gathering_started_event(const j_gathering_started_event& e);
public:
    void on_create() override;
    void on_destroy() override;
};

#endif
