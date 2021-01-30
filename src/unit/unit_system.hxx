#ifndef JARALYN_UNIT_SYSTEM_HXX
#define JARALYN_UNIT_SYSTEM_HXX

#include "../game.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"
#include "../event/inventory_event.hxx"
#include "../system.hxx"

/**
 * @brief Manages all unit-like entities
 */
class j_unit_system : public j_system<j_unit_system> {
private:
    /**
     * @brief Act on a gathering attempt
     *
     * As of now places the first found item at the notified position in the
     * entity's inventory.
     */
    void task_pickup_item(const j_gathering_started_event& e);
public:
    void on_load() override;
    void update(uint32_t delta_time) override;
};

#endif
